/**
 * edhd.c
 *
 * QDFS disk image editing tool
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

FILE* mbr;
FILE* temp;
FILE* img;

char c;
char mbr_buffer[512];
char input_buffer[512];

char *fsTableStr;

typedef struct record{
	char name[8]; //8
	uint64_t offset; // +8
	uint64_t size; // +8
	uint32_t attribute; // +4
	uint32_t timestamp; // +4
} record; // 32 bytes

record *fs_table, *tmp_table;

void finalize();
void rm_worker(const char*);
void cp_worker(FILE*, const char*);

void filename_padding(char* name){
	// file name has ONLY 16 bytes
	int i = 0;
	for(i = 0; i < 8; i++){
		if(name[i] < 32 || name[i] > 126){
			name[i] = ' ';
		}
	}
}

void init(){
	fs_table = (record*) malloc(sizeof(record));
	tmp_table = (record*) malloc(sizeof(record));
	if(fs_table == NULL || tmp_table == NULL){
		printf("Unable to allocate memory!\n");
		finalize();
		exit(-1);
	}
	mbr = fopen("mbr.bin", "r+b"); // RW, binary mode
	if(mbr == NULL){
		printf("mbr.bin not found.\n");
	}
	img = fopen("hd.img", "r+b"); // RW, binary mode
	if(img == NULL){
		printf("hd.img not found. Creating a new one.\n");
		img = fopen("hd.img", "w+b");
		fsTableStr = (char*) malloc(sizeof(char) * 8);
		strcpy(fsTableStr, "#REC_TBL");
		filename_padding(fsTableStr);
		int i = 0;
		// create an 1.44M floppy image
		for(i = 0; i < 1440 * 1024; i++){
			fputc(0, img);
		}
		printf("%d bytes written.\n", i);
		// create the fs table
		strncpy(fs_table->name, fsTableStr, 8);
		fs_table->offset = 0;
		fs_table->size = sizeof(record) * 256;
		fs_table->attribute = 0xDB;
		fs_table->timestamp = 0;
		rewind(img);
		fseek(img, 512, SEEK_SET);
		fwrite(fs_table, sizeof(record), 1, img);
		printf("Wrote 256 records.\n");
		rewind(img);
		free(fsTableStr);
	}else{
		// load fs table from the image
		fseek(img, 512, SEEK_SET);
		fread(fs_table, sizeof(record), 1, img);
	}
}

void finalize(){
	free(fs_table);
	free(tmp_table);
	fclose(img);
	if(mbr != NULL) fclose(mbr);
}

uint16_t attribute_gen(uint8_t permission, uint8_t ownerNtype){
	// owner has only 4 bits!
	// type has only 2 bits...
	uint16_t attr = 0;
	attr = permission;
	attr = attr << 8;
	attr += ownerNtype;
	return attr;
}

void ls(){
	// now we have the record section size
	// just read it along the track
	uint64_t ptr = 0;
	size_t counter = 0;
	printf("attr\t|name    |offset\t|size\t\t|time\n");
	for(ptr = 0; ptr < fs_table->size; ptr+=sizeof(record)){
		fseek(img, 512 + ptr, SEEK_SET);
		fread(tmp_table, sizeof(record), 1, img);
		if(strnlen(tmp_table->name, 8) < 1) continue;
		printf("%u\t|%8.8s|%u\t\t|%u\t\t|%u\n",
			tmp_table->attribute,
			tmp_table->name,
			tmp_table->offset,
			tmp_table->size,
			tmp_table->timestamp);
		++counter;
	}
	printf("%u/%u records are used.\n",
			counter,
			fs_table->size / sizeof(record));
}

uint64_t get_empty_record(record* r){
	uint64_t i;
	for(i = 0; i < fs_table->size; i+=sizeof(record)){
		fseek(img, 512 + i, SEEK_SET);
		fread(r, sizeof(record), 1, img);
		if(strnlen(r->name, 8) < 1){
			// this is it!
			return (512 + i);
		}
	}
	return 0;
}

int test_conflict(const char* name){
	record *r = (record*) malloc(sizeof(record));
	char *varName = (char*) malloc(sizeof(char) * 16);
	if(r == NULL){
		printf("Cannot allocate memory!");
		finalize();
		exit(-1);
	}
	strncpy(varName, name, 8);
	filename_padding(varName);
	size_t i = 0;
	for(i = 0; i < fs_table->size; i+=sizeof(record)){
		fseek(img, 512 + i, SEEK_SET);
		fread(r, sizeof(record), 1, img);
		if(strnlen(r->name, 8) < 1) continue;
		if(strcmp(r->name, varName) == 0){
			free(r);
			free(varName);
			return 1;
		}
	}
	free(r);
	free(varName);
	return 0;
}

uint64_t get_fit_offset(uint64_t size){
	record *r = malloc(sizeof(record));
	record *rNext = malloc(sizeof(record));
	if(r == NULL || rNext == NULL){
		printf("Unable to allocate memory!");
		finalize();
		exit(-1);
	}
	size_t i, j;
	uint64_t lastFileEnd, nextFileStart;
	int flagHasNextFile;
	for(i = 0; i < fs_table->size; i+=sizeof(record)){
		// outer loop
		fseek(img, 512 + i, SEEK_SET);
		fread(r, sizeof(record), 1, img);
		if(strnlen(r->name, 8) < 1) continue;
		lastFileEnd = r->offset + r->size;
		flagHasNextFile = 0;
		for(j = i + sizeof(record);
			j < fs_table->size;
			j+=sizeof(record)){
			// inner loop
			fseek(img, 512 + j, SEEK_SET);
			// get next vaild record
			// once get it, just use it
			fread(rNext, sizeof(record), 1, img);
			if(strnlen(rNext->name, 8) < 1) continue;
			nextFileStart = rNext->offset;
			flagHasNextFile = 1;
			break;
		}
		if(flagHasNextFile){
			if(nextFileStart - lastFileEnd < size){
				continue;
			}
			else{
				free(rNext);
				free(r);
				return lastFileEnd;
			}
		}
		else{
			free(rNext);
			free(r);
			return lastFileEnd;
		}
	}
	printf("Disk full.\n");
	free(rNext);
	free(r);
	return 0;
}

void cp_worker(FILE* file, const char* name){
	FILE* hfp = file;
	printf("Copying file...\n");
	uint64_t recordAddr = get_empty_record(tmp_table);
	fsTableStr = (char*) malloc(sizeof(char) * 8);
	strncpy(fsTableStr, name, 8);
	filename_padding(fsTableStr);
	strncpy(tmp_table->name, fsTableStr, 8);
	fseek(hfp, 0, SEEK_END);
	tmp_table->offset = get_fit_offset((uint32_t) ftell(hfp));
	tmp_table->size = (uint32_t) ftell(hfp);
	tmp_table->attribute = 0; // FIXME: debug use
	tmp_table->timestamp = (uint32_t) time(NULL);
	fseek(img, recordAddr, SEEK_SET);
	fwrite(tmp_table, sizeof(record), 1, img);
	fseek(img, 512 + fs_table->size + tmp_table->offset, SEEK_SET);
	size_t fileSize = 0;
	fseek(hfp, 0, SEEK_SET);
	while(!feof(hfp)){
		fputc(fgetc(hfp), img);
		++fileSize;
	}
	printf("Copied %u bytes\n", fileSize);
	free(fsTableStr);
}

void host_cp(const char* host, const char* slave){
	FILE *hfp;
	hfp = fopen(host, "r");
	if(hfp == NULL){
		printf("Cannot open this file!");
		return;
	}
	if(strlen(slave) == 0){
		printf("You have to name this file.\n");
		printf("Abort.\n");
		return;
	}
	if(test_conflict(slave)){
		printf("Well, this file does exist.\n");
		printf("Rename it?[Y/n]");
		char choice = getchar();
		if(choice == 0
			|| 'y' == choice
			|| 'Y' == choice){
			printf("Then name a new name: ");
			scanf("%s", input_buffer);
			if(test_conflict(input_buffer)){
				printf("I don't think it's wise to do this.\n");
				printf("Abort.\n");
				return;
			}
			else{
				cp_worker(hfp, input_buffer);
				return;
			}
		}
		else if('n' == choice
				|| 'N' == choice){
			printf("Overwriting this file...\n");
			// delete the old file then copy a new one
			rm_worker(input_buffer);
			cp_worker(hfp, input_buffer);
		}
		else{
			printf("It's not an vaild answer.\n");
			printf("Abort.\n");
			return;
		}
	}else{
		cp_worker(hfp, slave);
	}
	fclose(hfp);
	fflush(img);
}

void cp(){
	// copy
	// actually, this "copy" is a soft link (in unix)
	// since when we erasing file, we don't actually fill the disk with
	// zero, the "copy" would stay intact
	// UNLESS
	// there's a safe_rm option...
	char* src;
	char* dest;
	src = malloc(sizeof(char) * 255);
	dest = malloc(sizeof(char) * 255);
	printf("Source: ");
	scanf("%s", src);
	printf("Dest: ");
	scanf("%s", dest);

	free(src);
	free(dest);
}

void mkdir(){

}

void rm_worker(const char* name){
	int foundFlag = 0;
	char* fileName = malloc(sizeof(char) * 16);
	strncpy(fileName, name, 8);
	filename_padding(fileName);
	uint64_t ptr = 0;
	for(ptr = 0; ptr < fs_table->size; ptr+=sizeof(record)){
		fseek(img, 512 + ptr, SEEK_SET);
		fread(tmp_table, sizeof(record), 1, img);
		if(strncmp(fileName, tmp_table->name, 8) == 0){
			// this is it
			size_t i = 0;
			for(i = 0; i < sizeof(record); i++){
				fseek(img, 512 + ptr, SEEK_SET);
				fputc(0, img); // erase this record
			}
			fflush(img);
			foundFlag = 1;
			printf("File record erased.\n");
			break; // no need to clean up the data section
		}
	}
	if(!foundFlag){
		printf("File not found!\n");
	}
	free(fileName);
}

void rm(const char* name){
	// delete
	int foundFlag = 0;
	rm_worker(name);
}

void concat(){

}

void updateMBR(){
	// concat
	rewind(img);
	rewind(mbr);
	fread(mbr_buffer, 512, 1, mbr);
	printf("Written %u bytes.",
			fwrite(mbr_buffer, 512, 1, img));
}

int main(int argc, char** argv){
	init();
	while((c = getopt(argc, argv, "lur:p")) != -1){
		switch(c){
			case 'l':
				ls();
				break;
			case 'c':
				cp();
				break;
			case 'p':
				host_cp(argv[2], argv[3]);
				break;
			case 'r':
				rm(optarg);
				break;
			case 'u':
				updateMBR();
				break;
			default:
				break;
		}
	}
	finalize();
	return 0;
}
