#include "string.h"

size_t strlen(const char* s){
	register size_t i = 0;
	while(s[i] != 0) i++;
	return i;
}

void strcpy(const char* src, char* dest){
	size_t i = 0;
	while(src[i] != 0){
		dest[i] = src[i];
		i++;
	}
}

void strncpy(const char* src, char* dest, size_t n){
	size_t i = 0;
	while(src[i] != 0 && i < n){
		dest[i] = src[i];
		i++;
	}
}

void strfilp(char* s){
	size_t len = strlen(s);
	size_t i = 0;
	for(i = 0; i < len / 2; i++){
		char j = s[i];
		s[i] = s[len - i - 1];
		s[len - i - 1] = j;
	}
}

char to_char(int num){
	char c = 0;
	if(num > 9){
		c = 'a' + (num - 10);
	}
	else{
		c = '0' + num;
	}
	return c;
}

size_t strcato(const char* src, char* dest, size_t offset){
	size_t i = 0;
	while(src[i] != 0){
		dest[offset + i] = src[i];
		i++;
	}
	dest[offset + i] = 0;
	return offset + i;
}

