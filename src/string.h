#ifndef _STRING_H_
#define _STRING_H_

#include <stdint.h>
#include <stddef.h>

extern size_t strlen(const char*);
extern void strcpy(const char*, char*);
extern void strncpy(const char*, char*, size_t);
extern void strfilp(char*);
extern char to_char(int);
extern size_t strcato(const char*, char*, size_t);

#endif

