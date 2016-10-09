#include "gdt.h"

void encode(gdt* g, uint32_t base, uint32_t limit, uint8_t prop, uint8_t flag){
	g->limit0015 = (uint16_t) (limit & 0xFFFF);
	g->base0015 = (uint16_t) (base & 0xFFFF);
	g->base1623 = (uint8_t) ((base >> 16) & 0xff);
	g->prop = prop;
	g->limit1619flg = (flag | ((limit >> 12) & 0xf0));
	g->base2431 = (uint16_t) (base >> 22);
}

