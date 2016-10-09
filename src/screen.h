#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <stdint.h>

#include "vga.h"

extern void clrscr();
extern uint32_t calc_offset();
extern uint16_t make_vga_entry(uint8_t, uint8_t);
extern void set_pos(uint16_t, uint16_t);
extern void set_color(enum VGA_COLOR, enum VGA_COLOR);
extern void putchar(uint8_t);
extern void puts(const char*);
extern void term_init();

#endif
