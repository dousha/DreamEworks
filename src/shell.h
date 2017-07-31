#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdint.h>

// it's actually not a shell
// just a dummy keyboard handler

typedef
struct shell{
	char* screen;
	char* line;
}
shell;

extern void shell_init();
extern void shell_keyin(uint8_t, uint8_t);
extern void shell_loop();
extern void shell_submit();
extern void shell_update();
extern void shell_finalize();
extern shell* shell_get();

#endif

