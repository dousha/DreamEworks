#include "screen.h"
#include "hybintf.h"
#include "memutils.h"
#include "utils.h"
#include "idt.h"
#include "interrupts.h"
#include "memory.h"

static char *buf;

/// @brief The grand entrance of kernel
/// Called by stage2.asm:jmp c_kern
void c_kern(){
	term_init();
	// setup memory manager
	size_t size = mem_probe();
	mem_init(size);
	// try to alloc buf
	buf = (char*) malloc(20 * sizeof(char));
	itoa(buf, size, 10);
	puts("There's "); puts(buf); puts(" bytes avaliable on HihgMem\n");
	// init idt
	init_idt((void*) io_idtr_read());
	init_8259a();
	puts("Enable interrupts.\n");
	io_sti();
	for(;;){
		io_hlt();
	}
}
