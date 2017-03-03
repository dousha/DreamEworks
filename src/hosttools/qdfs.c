#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <Stdef.h>

#define NAME_LEN 16

typedef
struct file_record{
	char name[NAME_LEN]; // 16
	uint16_t segment;
	uint32_t offset;
	uint32_t size;
	uint16_t attr;
	uint32_t time;
} __attribute__((packed))
file_record;

// f[0] -> disk property
// f[1] -> filesystem property

uint64_t address(uint16_t seg, uint32_t off){
	uint64_t adr = 0;
	adr = seg * UINT32_MAX + off;
	return adr;
}

void disk_init(FILE* fp, size_t size){
	// size is in KiB
	// 5% of disk volume is used
	size_t length = size / 20;
	length = (length > (sizeof(file_record) * UINT32_MAX) >> 10)?
				(sizeof(file_record) * UINT32_MAX) >> 10:size;
	fseek(fp, 512, SEEK_SET);
	// init disk space
	size_t i = 0;
	for(i = 0; i < (length << 10); i++){
		fputc(fp, 0);
	}
	printf("%d KiB used for fs table.\n", length);
	// create fs info
	file_record f0, f1;
	if(size < UINT32_MAX){
		f0.segment = 0;
		f0.offset = size;
	}else{
		// segmentation is required
		// addr = segment * UINT32_MAX + offset
		f0.segment = size % UINT32_MAX;
		f0.offset = size - f0.segment;
	}
	strcpy(f0.name, "#DISK_PROP");
	f0.size = 0;
	f0.attr = 0;
	f0.time = 0;

	f1.segment = 0;
	f1.offset = 0;
	f1.size = (length << 10) / sizeof(file_record);
	strcpy(f1.name, "#FS_TABLE");
	f1.attr = 0;
	f1.time = 0;
	
	// flush to disk
	fwrite(&f0, sizeof(file_record), 1, fp);
	fwrite(&f1, sizeof(file_record), 1, fp);
	fflush();
}

uint64_t get_pos(size_t n){
	// always try to find the last entry
	// if the file doesn't fit
	// and there's space left out in the front
	// just perform a defrag
}

int host_write(FILE* fp, size_t n){

}

int file_write(void* file, size_t n){

}

