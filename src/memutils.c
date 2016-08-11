#include "screen.h"
#include "memutils.h"
#include "utils.h"

/// @brief Fill memory by byte (8 bits)
/// @param start Starting address
/// @param n Filling bytes count
/// @param v Filling value
/// @warning User applications shall use `memfill` instead of this.
void memfill_b(uint8_t* start, size_t n, uint8_t v){
	size_t count = 0;
	for(count = 0; count < n; count++){
		start[count] = v;
	}
}

/// @brief Fill memory by word (16 bits)
/// @param start Starting address
/// @param n Filling words count
/// @param v Fillling value
/// @warning User applications shall use `memfill` instead of this.
void memfill_w(uint16_t* start, size_t n, uint16_t v){
	size_t count = 0;
	for(count = 0; count < n; count++){
		start[count] = v;
	}
}

void memmove_b(uint8_t* from, uint8_t* to, size_t n){
	if(from == to) return;
	size_t i = 0;
	if(from > to){
		for(i = 0; i < n; i++){
			to[i] = from[i];
		}
	}else{
		for(i = n; i > 0; i--){
			to[i] = from[i];
		}
	}
}

/// @brief Probe system memory
/// Especially for HighMem (> 1MB)
/// @return Avaliable size starting from 0x100000
/// @warning This function containes a magic variable which seems
/// unused, but it affects the probing result. Might be an optmizing
/// glitch or so.
/// DO NOT REMOVE IT! Or probing will fail.
uint64_t mem_probe(){
	// the probing process is actually done by bootloader
	// memory information were stored in 0x7e00
	int entryCount = *((int*) 0x1000);z
	entryCount -= 0x7e00;
	entryCount /= 20;
	entryCount++;
	char buf[20];
	mem_record* curMemRec = (mem_record*) 0x7e00;
	uint64_t base = 0; ///< MAGIC! DO NOT TOUCH!
	uint64_t size = 0;
	puts("Probing memory...\n");
	itoa(buf, entryCount, 10);
	puts("Entry count: "); puts(buf); putchar('\n');
	while(entryCount > 0){
		itoa(buf, curMemRec->base, HEX);
		puts(buf); putchar('\t');
		itoa(buf, curMemRec->size, HEX);
		puts(buf); putchar('\t');
		switch(curMemRec->type){
			case 1:
				puts("Free\n");
				base = curMemRec->base;
				size = curMemRec->size;
				break;
			default:
				puts("Used\n");
				break;
		}
		++curMemRec;
		--entryCount;
	}
	/// MAGIC! DO NOT TOUCH!
	// size -= base;
	return size;
}
