#ifndef _GDT_H_
#define _GDT_H_

#include <stdint.h>

#define GDT_PRESENT 0x90
#define GDT_SYSTEM 0x10
#define GDT_CODE 0x18
#define GDT_DATA 0x10
#define GDT_STACK 0x14
#define GDT_CONFIRM 0x14
#define GDT_DATA_RO 0x10
#define GDT_DATA_RW 0x12
#define GDT_CODE_R 0x12
#define GDT_ACCESSED 0x11

#define GDT_4K 0x8
#define GDT_32 0x4

typedef
struct gdtr{
	uint16_t size;
	uint32_t offset;
} __attribute__((packed))
gdtr;

typedef
struct gdt{
	uint16_t limit0015;
	uint16_t base0015;
	uint8_t base1623;
	uint8_t prop;
	uint8_t limit1619flg;
	uint8_t base2431;
} __attribute__((packed))
gdt;

extern void encode(gdt*, uint32_t, uint32_t, uint8_t, uint8_t);

#endif

