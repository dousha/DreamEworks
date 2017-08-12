#ifndef _HYBIRD_INTERFACE_H_
#define _HYBIRD_INTERFACE_H_

#include <stdint.h>

/// the corresponing code is hybintf.asm

extern void io_hlt(void);
extern void io_sti(void);
extern void io_cli(void);
extern void io_stihlt(void);

extern uint8_t io_in8(uint16_t);
extern uint16_t io_in16(uint16_t);
extern uint32_t io_in32(uint16_t);

/// it would break if I explicitly use
/// uintx_t
/// trust me
/// maybe it's because of compiler optimizations
/// which made everything aligned to 4bytes
/// so the stack goes wried?
extern void io_out8(int, int);
extern void io_out16(int, int);
extern void io_out32(int, int);

extern uint32_t io_eflags_read(void);
extern void io_eflags_write(uint32_t);
extern uint32_t io_idtr_read(void);
extern void io_idtr_write(uint32_t);
extern uint32_t io_gdtr_read(void);
extern void io_gdtr_write(uint32_t);
extern void io_load_tss(uint32_t);
extern uint32_t io_current_esp(void);

extern void io_int_user(void);

extern void far_jump(int, int);
extern void test_jump();

extern void exp_ud(void);
extern void int_dbg(void);

#endif
