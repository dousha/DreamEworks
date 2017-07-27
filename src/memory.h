#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>
#include <stddef.h>

typedef
struct mem_table{
	uint32_t offset;
	size_t size;
}
mem_table;

typedef
struct mem_manager{
	mem_table* tables;
	size_t records;
	size_t records_free;
	size_t memory_size;
	size_t free_memory_size;
	size_t base_addr;
}
mem_manager;

extern void mem_init(size_t);
extern void* malloc(size_t);
extern void free(void*);

extern size_t get_base();
extern size_t get_total_records();
extern void dbg_print_mm();

#endif
