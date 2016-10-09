#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stddef.h>

#define NAME_LEN 16

typedef
struct file_record{
	char name[NAME_LEN]; // 16
	uint16_t segment; // + 2
	uint32_t offset; // + 4
	uint32_t size; // + 4
	uint16_t attr; // + 2
	uint32_t timestamp; // + 4
} __attribute__((packed))
file_record; // 32 bytes

file_record* disk_info;
file_record* fs_table;
file_record* tmp_table;

FILE* disk;

uint64_t proper_segset(uint32_t size){
	uint16_t seg = 0;
	uint32_t off = 0;
}

void delete(const char* path){

}

int update(const char* mbr){
	FILE* fp = fopen(mbr, "rb");
	if(fp == NULL){
		printf("[FATAL] MBR not found or cannot open.\n");
		return 1;
	}
	rewind(disk);
	fwrite(disk, 512, 1, fp);
	return 0;
}

void host_cp(const char* src, const char* dest){

}

int print_attribute(uint16_t attr){
	if(!(attr & 0x8000)) return 0; // p--- ---- ---- ----, present
	if(!(attr & 0x1000)) return 0; // ---v ---- ---- ----, visible
		// note that 'visible' above is not for user files
	if(attr & 0x4000) printf("X"); // -X-- ---- ---- ----, fixed
	if(attr & 0x2000) printf("s"); // --s- ---- ---- ----, special
	if(attr & 0x0800) printf("d"); // ---- d--- ---- ----, device
	if(attr & 0x0400) printf("l"); // ---- -l-- ---- ----, link
	if(attr & 0x0200) printf("f"); // ---- --f- ---- ----, folder
	
	if(attr & 0x0100) printf("r"); // ---- ---r ---- ----, ring0 read
	if(attr & 0x0080) printf("w");
	if(attr & 0x0040) printf("x");
	if(attr & 0x0020) printf("r"); // ---- ---- --r- ----, ring1 read
	if(attr & 0x0010) printf("w");
	if(attr & 0x0008) printf("x");
	if(attr & 0x0004) printf("r"); // ---- ---- ---- -r--, ring3 read
	if(attr & 0x0002) printf("w");
	if(attr & 0x0001) printf("x");
	return 1;
}

void ls(){
	// fs record can be empty
	rewind(disk);
	fseek(disk, 512, SEEK_SET);
	size_t i = 0;
	for(i = 0; i < fs_table->offset; i+=sizeof(file_record)){
		fread(tmp_table, sizeof(file_record), 1, disk);
		if(tmp_table->offset = 0) continue;
		if(print_attribute(tmp_table->attr)){
			printf("%16.16s\t%u\t%u\t%u\t%d",
					tmp_table->name,
					tmp_table->segment,
					tmp_table->offset,
					tmp_table->size,
					tmp_table->timestamp);
		}
		fseek(disk, sizeof(file_record), SEEK_CUR);
	}
}

void create_disk(FILE* disk, uint64_t size){
	size_t i = 0;
	for(i = 0; i < size; i++){
		fputc(0, disk);
	}
	printf("%d bytes written.\n", i);
	// create fs table
	disk_info->name = "#DISK_INFO";
	disk_info->segment = (uint16_t) ((size >> 32) & 0xffff);
	disk_info->offset = (uint32_t) (size & 0xffffffff);
	disk_info->size = 0;
	disk_info->attr = 0x99a4; // p--s d--r w-r- -r--
	disk_info->timestamp = 0;
	
	fs_table->name = "#FILE_RECORD";
	fs_table->segment = 0;
	if((size / 5760) > 0xffffffff){
		fs_table->offset = 0xffffffff;
	}else{
		fs_table->offset = (size / 5760);
	}
	fs_table->size = 0;
	fs_table->attr = 0x99b4; // p--s d--r w-rw -r--
	fs_table->timestamp = time(NULL);
	
	rewind(disk);
	fseek(disk, 512, SEEK_SET);
	fwrite(disk_info, sizeof(file_record), 1, disk);
	fwrite(fs_table, sizeof(file_record), 1, disk);
	rewind(disk);
}

int main(int argc, char** argv){
	disk_info = malloc(sizeof(file_record));
	fs_table = malloc(sizeof(file_record));
	tmp_table = malloc(sizeof(file_record));
	disk = fopen("hd.img", "r+b"); // compability
	if(disk == NULL){
		printf("Cannot open file `hd.img`. Creating a new one.\n");
		disk = fopen("hd.img", "w+b");
		// create a 1.44M floppy image
		create_disk(1440*1024);
	}
	if(argc > 1){
		// run in command mode
		switch(argv[1][1]){
			/// \-[dupl], delete, update, paste, list
			case 'd':
				delete(argv[2]);
				break;
			case 'u':
				if(update(argv[2])) exit(-1);
				break;
			case 'p':
				host_cp(argv[2], argv[3]);
				break;
			case 'l':
				ls();
				break;
			default:
				printf("Invalid option.\n");
				return -1;
				break; // dead code
		}
	}
	free(tmp_table);
	free(fs_table);
	free(disk_info);
}

