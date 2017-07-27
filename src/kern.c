#include "screen.h"
#include "hybintf.h"
#include "memutils.h"
#include "utils.h"
#include "idt.h"
#include "interrupts.h"
#include "memory.h"
#include "keyboard.h"
#include "task.h"

static char *buf;

void delay_test(){
	puts("fuck yeah!\n");
}

/// @brief The grand entrance of kernel
/// Called by stage2.asm:jmp c_kern
void c_kern(){
	// setup screen
	term_init();
	// setup memory manager
	size_t size = mem_probe();
	mem_init(size);
	// try to alloc buf
	buf = (char*) malloc(20 * sizeof(char));
	itoa(buf, size, 10);
	puts("[Kern] Free mem = "); puts(buf); puts("\n");
	itoa(buf, get_base(), 16);
	puts("[Kern] Base addr = "); puts(buf); puts("\n");
	itoa(buf, get_total_records(), 10);
	puts("[Kern] Record count = "); puts(buf); puts("\n");
	// init idt
	init_idt((void*) io_idtr_read());
	// init 8259a chips
	init_8259a();
	puts("[Kern] Enable interrupts.\n");
	// and pits, this enables system to work with delays
	init_pit();
	puts("[Kern] Enable PIT.\n");
	// the taskmgr
	itoa(buf, task_init(size), 10);
	puts("[Kern] "); puts(buf); puts(" bytes used for taskmgr.\n");
	// put a delay for test
	delay_create(500, &delay_test);
	// then keyboard
	kbd_init();
	// and the shell
	// but the shell hasn't be done yet
	//kbd_bind(init_shell());
	for(;;){
		io_cli();
		if(kbd_status()){
			io_stihlt();
		}
		else{
			kbd_process();
		}
	}
	kbd_finalize();
	free(buf);
}
