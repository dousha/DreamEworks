#include <stdint.h>
#include <stddef.h>
#include "vga.h"
#include "memutils.h"

uint16_t* vram;
uint8_t curColor;
volatile size_t curX, curY;

uint8_t make_color(enum VGA_COLOR fg, enum VGA_COLOR bg){
	return fg | bg << 4;
}

uint32_t calc_offset(uint8_t x, uint8_t y){
	return (((uint32_t) y * SCREEN_WIDTH) + (uint32_t) x);
}

uint16_t make_vga_entry(uint8_t color, uint8_t c){
	uint16_t char16 = c;
	uint16_t color16 = color;
	return char16 | color16 << 8;
}

void set_pos(uint16_t newx, uint16_t newy){
	if(newx < SCREEN_WIDTH && newy < SCREEN_HEIGHT){
		curX = newx;
		curY = newy;
	}
	else{
		curX = 0;
		curY = 0;
	}
}

void set_color(enum VGA_COLOR fg, enum VGA_COLOR bg){
	curColor = make_color(fg, bg);
}

void roll_screen(){
	memmove_b((uint8_t*) 0xb8000 + SCREEN_WIDTH * 2,
				(uint8_t*) 0xb8000,
				SCREEN_WIDTH * (SCREEN_HEIGHT - 1) * 2);
	memfill_b((uint8_t*) 0xb8000
				+ SCREEN_WIDTH * (SCREEN_HEIGHT - 1) * 2,
				SCREEN_WIDTH * 2,
				0);
}

void putchar(char c){
	switch(c){
		case '\n':
			curX = 0;
			if(++curY == SCREEN_HEIGHT){
				roll_screen();
				curY = SCREEN_HEIGHT - 1;
			}
			break;
		case '\t':
			if(curX % 8 == 0) curX += 8;
			else curX += (8 - curX % 8);
			if(curX > SCREEN_WIDTH){
				if(++curY == SCREEN_HEIGHT){
					roll_screen();
					curY = SCREEN_HEIGHT - 1;
				}
				curX = 0;
			}
			break;
		case '\b':
			if(curX == 0) break;
			else{
				--curX;
				putchar(' ');
				--curX;
			}
			break;
		case '\r':
			curX = 0;
			break;
		default:
			vram[calc_offset(curX, curY)] =
			make_vga_entry(curColor, c);
			if(++curX == SCREEN_WIDTH){
				curX = 0;
				if(++curY == SCREEN_HEIGHT){
					roll_screen();
					curY = SCREEN_HEIGHT - 1;
				}
			}
			break;
	}
}

void puts(const char* str){
	size_t i = 0;
	while(str[i] != 0){
		putchar(str[i]);
		i++;
	}
}

void clrscr(){
	// TODO: support multiple modes
	memfill_w(vram, SCREEN_WIDTH * SCREEN_HEIGHT, make_vga_entry(curColor, ' '));
	set_pos(0, 0);
}

void term_init(){
	curX = 0;
	curY = 0;
	vram = (uint16_t*) 0xb8000;
	curColor = make_color(COLOR_L_GREY, COLOR_BLACK);
	clrscr();
}
