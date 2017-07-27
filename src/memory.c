#include "memutils.h"
#include "memory.h"
#include "utils.h"
#include "screen.h"
#include "interrupts.h"
#include "hybintf.h"

static mem_manager* mm;

void mem_init(size_t size){
	// size is the highmem size
	// mm is always placed at 0x100000
	// memory manager would take 0.5% of highmem for itself and memory table
	size_t tableLen = size / 200;
	size_t tableSize = (tableLen - sizeof(mem_manager)) / sizeof(mem_table);
	// populate the memory
	mm = (mem_manager*) 0x100000;
	mm->memory_size = size;
	mm->base_addr = 0x100000 + tableLen;
	mm->free_memory_size = size - tableLen;
	mm->records = tableSize;
	mm->records_free = tableSize;
	mm->tables = (mem_table*) (0x100000 + sizeof(mem_manager));
	// fill the tables
	memfill_b((uint8_t*) mm->tables,
			mm->records * sizeof(mem_table),
			0);
}

void* malloc(size_t size){
	// allocate space
	// true, malloc is an expesive operation
	if(size > mm->free_memory_size) return NULL;
	if(mm->records_free == 0) return NULL;
	size_t i = 0, j = 0;
	uint32_t lastOffset = 0;
	for(i = 0; i < mm->records; i++){
		if(mm->tables[i].offset != 0){
			if(mm->tables[i+1].offset == 0){
				// this is the place?
				for(j = i; j < mm->records; j++){
					// seek for the next record
					if(mm->tables[j].offset != 0) break;
				}
				if(j == mm->records - 1){
					// hit the bottom of the list
					// just alloc it
					mm->tables[i+1].offset = 
						mm->tables[i].offset
						+ mm->tables[i].size;
					mm->tables[i+1].size = size;
					lastOffset = mm->tables[i+1].offset;
					break;
				}
				if(mm->tables[j].offset 
					- (mm->tables[i].offset
						+ mm->tables[i].size
					) < size
				){
					i = j;
					continue; // no it's not
				}
				if(i + 2 == j){
					// this is the only table between them
					// if the size doesn't fit just right
					// move the rest of tables by 1 table
					// but if the table is full it have to take up all the space
					if(mm->records_free == 1){
						// the only lasting spot
						mm->tables[i+1].offset =
							mm->tables[i].offset
							+ mm->tables[i].size;
						mm->tables[i+1].size = 
							mm->tables[i+2].offset
							- mm->tables[i+1].offset;
						lastOffset = mm->tables[i+1].offset;
						break;
					}else{
						if(mm->tables[j].offset 
							- (
								mm->tables[i].offset 
								+ mm->tables[i].size
							) == 0 // if just fit
						){
							// just alloc it
							mm->tables[i+1].offset =
								mm->tables[i].offset
								+ mm->tables[i].size;
							mm->tables[i+1].size = size;
							lastOffset = mm->tables[i+1].offset;
							break;
						}
						else{
							// move tables
							memmove_b((uint8_t*) mm->tables + i + 2, 
									(uint8_t*) mm->tables + i + 3,
									mm->records - mm->records_free - i - 2);
							// alloc the space
							mm->tables[i+1].offset = 
								mm->tables[i].offset
								+ mm->tables[i].size;
							mm->tables[i+1].size = size;
							lastOffset = mm->tables[i+1].offset;
							break;
						}
					}
				}else{
					// just alloc it
					mm->tables[i+1].offset = 
						mm->tables[i].offset 
						+ mm->tables[i].size;
					mm->tables[i+1].size = size;
					lastOffset = mm->tables[i+1].offset;
					break;
				}
			}
			else{
				continue;
			}
		}else{
			// this is the very first table
			mm->tables[i].offset = mm->base_addr;
			mm->tables[i].size = size;
			lastOffset = mm->base_addr;
			break;
		}
	}
	mm->free_memory_size -= size;
	--mm->records_free;
	return (void*) lastOffset;
}

void free(void* ptr){
	// free space
	if(ptr == NULL) return;
	uint32_t offset = (uint32_t) ptr;
	size_t i = 0;
	for(i = 0; i < mm->records; i++){
		if(mm->tables[i].offset == offset){
			// this is it
			mm->tables[i].offset = 0;
			++(mm->records_free);
			break;
		}
    }
	// don't care if ptr is a fake one (not malloced one)
	// since fake one doesn't affect anything
	// and for pointer safety...
    ptr = NULL;
	// we've done that for you
}

size_t get_base(){
	return mm->base_addr;
}

size_t get_total_records(){
	return mm->records;
}

void dbg_print_mm(){
	// print memory status
	clrscr();
	set_pos(0, 0);
	char* buf = (char*) malloc(sizeof(char) * 32); // lol
	
	puts("HighMem: ");
	itoa(buf, mm->memory_size - mm->free_memory_size, 10);
	puts(buf); puts("/");
	itoa(buf, mm->memory_size, 10);
	puts(buf); puts("\n");
	puts("Records: ");
	itoa(buf, mm->records - mm->records_free, 10);
	puts(buf); puts("/");
	itoa(buf, mm->records, 10);
	puts(buf); puts("\n");
	
	mem_table* tbl = mm->tables;
	puts("Offset:Size\n");
	size_t i = 0;
	for(i = 0; i < mm->records; i++){ // XXX
		if(tbl[i].offset == 0) continue;
		itoa(buf, tbl[i].offset, 16);
		puts(buf);
		puts(":");
		itoa(buf, tbl[i].size, 16);
		puts(buf);
		puts("\n");
	}

	free(buf);
}

