#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>
#include <stddef.h>

typedef
struct mem_table{
	uint32_t offset;
	size_t length;
}
mem_table;

typedef
struct mem_manager{
	mem_table* tables;
	size_t records;
	size_t memory_size;
}
mem_manager;

extern void mem_init(size_t);
extern void* malloc(size_t);
extern void free(void*);

#endif
