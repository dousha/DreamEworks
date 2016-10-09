#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

// LQSN PASC
#define STATE_CTRL 0x1
#define STATE_SHIFT 0x2
#define STATE_ALT 0x4
#define STATE_CAPS 0x8
#define STATE_NUM 0x10
#define STATE_SCROLL 0x20
#define STATE_SYSRQ 0x40
#define STATE_LOTUS 0x80

#define KEY_ESC 0x01
#define KEY_CTRL 0x1d
#define KEY_SHIFT 0x2a
#define KEY_ALT 0x38
#define KEY_LOTUS 0x5b
#define KEY_CONTEXT 0x5c
#define KEY_SYSRQ 0x54
#define KEY_CAPS 0x3a
#define KEY_NUM 0x45
#define KEY_SCROLL 0x46
#define KEY_DUP 0x48
#define KEY_DDOWN 0x4a
#define KEY_DLEFT 0x49
#define KEY_DRIGHT 0x4b

#define KEY_RIGHT 0x10
#define KEY_FUNCTION 0x20

typedef
struct keyboard{
	uint8_t buf[32];
	uint8_t readptr, writeptr;
	uint8_t key_state;
} __attribute__((packed))
keyboard;

extern void kbd_init();
extern void kbd_finalize();
extern void kbd_bind(void (*callback)(uint8_t, char));
extern void kbd_push(uint8_t);
extern int kbd_status();
extern void kbd_process();
extern char key2char(uint8_t);

static const char charmap[] = {
// charmap
// norm, shift, e0 *
0, 0, 0, 
KEY_ESC, KEY_ESC, 0, // esc
'1', '!', 0,
'2', '@', 0,
'3', '#', 0,
'4', '$', 0,
'5', '%', 0,
'6', '^', 0,
'7', '&', 0,
'8', '*', 0,
'9', '(', 0,
'0', ')', 0,
'-', '_', 0,
'=', '+', 0,
'\b', '\b', 0, // backspace
'\t', '\t', 0, // tab
'q', 'Q', 0,
'w', 'W', 0,
'e', 'E', 0,
'r', 'R', 0,
't', 'T', 0,
'y', 'Y', 0,
'u', 'U', 0,
'i', 'I', 0,
'o', 'O', 0,
'p', 'P', 0,
'[', '{', 0,
']', '}', 0,
'\n', '\n', 0, // enter
KEY_CTRL, KEY_CTRL, KEY_CTRL | KEY_RIGHT,
'a', 'A', 0,
's', 'S', 0,
'd', 'D', 0,
'f', 'F', 0,
'g', 'G', 0,
'h', 'H', 0,
'j', 'J', 0,
'k', 'K', 0,
'l', 'L', 0,
';', ':', 0,
'\'', '\"', 0,
'`', '~', 0,
KEY_SHIFT, KEY_SHIFT, KEY_SHIFT | KEY_RIGHT,
'\\', '|', 0,
'z', 'Z', 0,
'x', 'X', 0,
'c', 'C', 0,
'v', 'V', 0,
'b', 'B', 0,
'n', 'N', 0,
'm', 'M', 0,
',', '<', 0,
'.', '>', 0,
'/', '?', 0,
KEY_SHIFT | KEY_RIGHT , KEY_SHIFT | KEY_RIGHT, KEY_SHIFT | KEY_RIGHT,
'*','*',0, // *
KEY_ALT, KEY_ALT, KEY_ALT | KEY_RIGHT,
' ', ' ', 0, // space
KEY_CAPS, KEY_CAPS, 0,
0, 0, 0, // F1
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0, // F10
KEY_NUM, KEY_NUM, 0,
KEY_SCROLL, KEY_SCROLL, 0,
0, 0, 0, // home
0, 0, 0, // up
0, 0, 0, // pgup
0, 0, 0, // -
0, 0, 0, // left
0, 0, 0, // mid
0, 0, 0, // right
0, 0, 0, // plus
0, 0, 0, // end
0, 0, 0, // down
0, 0, 0, // pgdown
0, 0, 0, // insert
0, 0, 0, // dot
0, 0, 0, // enter
0, 0, 0, // ???
0, 0, 0, // ???
0, 0, 0, // F11
0, 0, 0, // F12
0, 0, 0, // left unused...
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
0, 0, 0,
'\\', '|', 0,
0, 0, 0,
0, 0, 0,
};

#endif

