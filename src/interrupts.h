#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

extern void init_8259a();
extern void init_pit();

extern void* int_e_div();
extern void* int_e_dbg();
extern void* int_e_nmi();
extern void* int_e_bp();
extern void* int_e_overflow();
extern void* int_e_bound();
extern void* int_e_undef();
extern void* int_e_no_math();
extern void* int_e_double_fault();
extern void* int_e_aux_bound();
extern void* int_e_invalid_tss();
extern void* int_e_not_present();
extern void* int_e_stack_segment();
extern void* int_e_general_protection();
extern void* int_e_page_fault();
extern void* int_e_reserved();
extern void* int_e_math_fault();
extern void* int_e_align_check();
extern void* int_e_machine_check();
extern void* int_e_sse_fault();

extern void *int_dummy();
extern void *int_kbd();
extern void *int_timer();
extern void *int_task_tick();
extern void *int_user_0x80();

extern void int_kbd_hwnd();
extern void int_timer_hwnd();
extern void int_task_tick_hwnd();
extern void int_user_0x80_hwnd();
extern void int_exception_hwnd(int vec, int err, int eip, int cs, int eflags);

#endif

