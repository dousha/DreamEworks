#include "console.h"
#include "hybintf.h"
#include "utils.h"

void int_kbd_hwnd(){
	uint8_t code = io_in8(0x60);
	
	io_out8(0x20, 0x20);
}

void int_timer_hwnd(){
	putchar('C');
	io_out8(0x20, 0x20);
}

void int_user_0x80_hwnd(){
	putchar('8');
}

