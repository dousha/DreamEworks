#include "screen.h"
#include "hybintf.h"
#include "utils.h"
#include "keyboard.h"
#include "interrupts.h"
#include "task.h"

static const char* exception_msg[] = {
	"#DE - Division error",
	"#DB - Debugging break",
	"#NMI - Non Maskable Interrupt",
	"#BP - Break point",
	"#OF - Overflown",
	"#BR - Bound violation",
	"#UD - Undefined command",
	"#NM - No math co-processor",
	"#DF - Double fault",
	"#XB - Aux processor violation",
	"#TS - Invalid TSS",
	"#NP - Segment is not present",
	"#SS - Stack segment error",
	"#GP - General protection",
	"#PF - Page fault",
	"#RS - Reserved, it shouldn't happen",
	"#MF - x87FPU Float point error",
	"#AC - Align check error",
	"#MC - Machine check error",
	"#XF - SIMD Float point error"
};

void int_kbd_hwnd(){
	uint8_t code = io_in8(0x60);
	kbd_push(code);
	io_out8(0x20, 0x20);
}

void int_timer_hwnd(){
	++((task_man_get())->uptime);
	delay_poll();
	io_out8(0x20, 0x20);
	task_tick();
}

void int_task_tick_hwnd(){

}

void int_user_0x80_hwnd(){
	putchar('d');
}

void int_exception_hwnd(int vec, int err, int eip, int cs, int eflags){
	// The following functions called are guranteed to work
	// when error occured
	set_color(COLOR_WHITE, COLOR_BLUE);
	clrscr();
	char buf[16];
	puts("[INTR] Fatal exception occured\n");
	puts(exception_msg[vec]); putchar('\n');
	itoa(buf, eip, 16);
	puts("EIP = "); puts(buf); putchar('\n');
	itoa(buf, cs, 16);
	puts("CS = "); puts(buf); putchar('\n');
	itoa(buf, eflags, 16);
	puts("EFLAGS = "); puts(buf); putchar('\n');
	if(err != -1){
		itoa(buf, err, 16);
		puts("ERR = "); puts(buf); putchar('\n');
	}
}
