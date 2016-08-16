#include "memutils.h"
#include "memory.h"
#include "utils.h"
#include "screen.h"

static mem_manager* mm = 0;

void mem_init(size_t size){
	// size is the highmem size
	// mm is always placed at 0x100000
	// memory manager would take 5% of highmem for itself and memory table
	size_t tableLen = size / 20;
	size_t tableSize = (tableLen - sizeof(mem_manager)) / sizeof(mem_table);
	// populate the memory
	mm = (mem_manager*) 0x100000;
	mm->memory_size = size;
	mm->free_memory_size = size - tableLen;
	mm->records = tableSize;
	mm->tables = (mem_table*) (0x100000 + sizeof(mem_manager));
	// fill the tables
	memfill_b((uint8_t*) mm->tables,
			mm->records * sizeof(mem_table),
			0);
}

void* malloc(size_t size){
	// allocate space
	if(size > mm->free_memory_size) return NULL;
	size_t i = mm->records - 1;
	uint32_t lastOffset = 0;
	for(i = mm->records - 1; i > 0; i--){
		if(mm->tables[i].offset != 0){
			lastOffset = mm->tables[i].offset + mm->tables[i].size;
			++i;
			mm->tables[i].offset = lastOffset;
			mm->tables[i].size = size;
			mm->free_memory_size -= size;
			break;
		}
	}
	return (void*) lastOffset;
}

void free(void* ptr){
	// free space
	uint32_t offset = (uint32_t) ptr;
	size_t i = 0;
	for(i = 0; i < mm->records; i++){
		if(mm->tables[i].offset == offset){
			// this is it
			mm->tables[i].offset = 0;
			break;
		}
    }
    ptr = NULL;
}

