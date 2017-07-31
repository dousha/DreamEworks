#include <stdint.h>

#include "keyboard.h"
#include "memory.h"
#include "screen.h"

static keyboard* kbd;
static void (*kbdProcessBind)(uint8_t, uint8_t);

void kbd_init(){
	kbd = (keyboard*) malloc(sizeof(keyboard));
	kbd->readptr = 0;
	kbd->writeptr = 0;
	kbdProcessBind = NULL;
}

void kbd_finalize(){
	free(kbd);
}

void kbd_push(uint8_t code){
	kbd->buf[kbd->writeptr] = code;
	if(++kbd->writeptr == 32) kbd->writeptr = 0;
}

int kbd_status(){
	return kbd->readptr == kbd->writeptr;
}

void kbd_bind(void (*callback)(uint8_t, uint8_t)){
	kbdProcessBind = callback;
}

void kbd_process(){
	if(kbdProcessBind == NULL){
		// discard input
		// because keyboard stage 2 processor
		// is not present
		if(++kbd->readptr == 32) kbd->readptr = 0;
		return;
	}
	if(kbd->buf[kbd->readptr] > 0x80){
		if(kbd->buf[kbd->readptr] == 0xe0){
			// sth wried
			// do not take any further action
			// yet
		}
		// release code
		kbd->buf[kbd->readptr] -= 0x80;
		switch(kbd->buf[kbd->readptr]){
			case KEY_CTRL:
				kbd->key_state ^= STATE_CTRL;
				break;
			case KEY_LEFT_SHIFT:
			case KEY_RIGHT_SHIFT:
				kbd->key_state ^= STATE_SHIFT;
				break;
			case KEY_ALT:
				kbd->key_state ^= STATE_ALT;
				break;
			default:
				break;
		}
	}else{
		switch(kbd->buf[kbd->readptr]){
			case KEY_CTRL:
				kbd->key_state |= STATE_CTRL;
				break;
			case KEY_LEFT_SHIFT:
			case KEY_RIGHT_SHIFT:
				kbd->key_state |= STATE_SHIFT;
				break;
			case KEY_ALT:
				kbd->key_state |= STATE_ALT;
				break;
			default:
				kbdProcessBind(kbd->key_state, kbd->buf[kbd->readptr]);
				break;
		}
	}
	if(++kbd->readptr == 32) kbd->readptr = 0;
}

