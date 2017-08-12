#include "hybintf.h"
#include "keyboard.h"
#include "utils.h"
#include "memory.h"
#include "screen.h"
#include "shell.h"

static char* buf;
static char* line;

void shell_init(){
	// clrscr();
	cursor_follow();
	buf = (char*) malloc(sizeof(char) * 8);
	line = (char*) malloc(sizeof(char) * 82);
}

void shell_finalize(){
	free(buf);
}

void shell_loop(){
	for(;;){
		io_cli();
		if(kbd_status()){
			io_stihlt();
		}else{
			kbd_process();
		}
	}
}

void shell_submit(){
	// dbg
}

void shell_keyin(uint8_t mode, uint8_t code){
	if(mode & STATE_CTRL) putchar('^');
	if(mode & STATE_ALT) putchar('[');
	if(charmap[code * 3] == '\n'){
		shell_submit();
		return;
	}
	if(mode & STATE_SHIFT){
		putchar(charmap[code * 3 + 1]);
	}else{
		putchar(charmap[code * 3]);
	}
	cursor_follow();
}
