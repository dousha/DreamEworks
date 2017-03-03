#ifndef _POOL_H_
#define _POOL_H_

#include <stdint.h>
#include <stddef.h>

typedef
struct pool{
	uint32_t len;
	uint8_t* pol;
	uint32_t r;
	uint32_t w;
}
pool;

extern pool* pool_create(size_t);
extern void pool_free(pool*);
extern void pool_write(pool*, uint8_t);
extern uint8_t pool_read(pool*);
extern uint8_t pool_is_empty(pool*);
extern uint8_t pool_peak(pool*);
extern void pool_drop(pool*);

#endif

