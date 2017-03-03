#include "hybintf.h"
#include "keyboard.h"
#include "utils.h"
#include "memory.h"
#include "screen.h"
#include "shell.h"

void shell_init(){
	//clrscr();
	set_cursor(0, 0);
}

void shell_loop(){
	for(;;){
		io_cli();
		if(kbd_status()){
			io_stihlt();
		}
		else{
			kbd_process();
		}
	}
}

void shell_keyin(uint8_t mod, char c){
	if(!(mod & STATE_SHIFT))
		putchar(c);
}

