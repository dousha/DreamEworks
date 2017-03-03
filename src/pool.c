#include "memory.h"
#include "pool.h"
#include "interrupts.h"

pool* pool_create(size_t n){
	pool* p = malloc(sizeof(pool));
	if(p == NULL) int_user_0x80_hwnd();
	p->pol = malloc(sizeof(uint8_t) * n);
	p->len = n;
	p->r = 0;
	p->w = 0;
	return p;
}

void pool_free(pool* p){
	free(p->pol);
	free(p);
}

void pool_write(pool* p, uint8_t n){
	(p->pol)[p->w] = n;
	if(++(p->w) == p->len)
		p->w = 0;
}

uint8_t pool_read(pool* p){
	uint8_t i = pool_peak(p);
	pool_drop(p);
	return i;
}

uint8_t pool_peak(pool* p){
	return (p->pol)[p->r];
}

void pool_drop(pool* p){
	if(++(p->r) == p->len) p->r = 0;
}

uint8_t pool_is_empty(pool* p){
	return (uint8_t) (p->w == p->r);
}

