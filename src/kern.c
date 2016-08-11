#include "screen.h"
#include "hybintf.h"
#include "memutils.h"
#include "utils.h"

const char* info = "dsys v0.01\n";
char buf[20];

void c_kern(){
	term_init();
	puts(info);
	size_t size = mem_probe();
	itoa(buf, size, 10);
	puts("There's "); puts(buf); puts(" bytes avaliable on HihgMem\n");

	for(;;){
		io_hlt();
	}
}
