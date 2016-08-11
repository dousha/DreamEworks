#ifndef _HYBIRD_INTERFACE_H_
#define _HYBIRD_INTERFACE_H_

#include <stdint.h>

/// the corresponing code is hybintf.asm

extern void write_graphics(uint16_t, uint32_t);
extern void memfill_l(uint32_t, uint32_t, uint32_t);

extern void io_hlt();

#endif

