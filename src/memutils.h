#ifndef _MEM_UTILS_H_
#define _MEM_UTILS_H_

#include <stdint.h>
#include <stddef.h>

typedef
struct mem_record{
	uint64_t base;
	uint64_t size;
	uint32_t type;
} __attribute__((packed))
mem_record;

extern void memfill_b(uint8_t*, size_t, uint8_t);
extern void memfill_w(uint16_t*, size_t, uint16_t);
extern void memmove_b(uint8_t*, uint8_t*, size_t);

extern uint64_t mem_probe();

#endif

