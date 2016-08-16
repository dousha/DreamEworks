#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

extern void init_8259a();

extern void *int_dummy();
extern void *int_kbd();
extern void *int_timer();
extern void *int_task_tick();
extern void *int_user_0x80();

extern void int_kbd_hwnd();
extern void int_timer_hwnd();
extern void int_task_tick_hwnd();
extern void int_user_0x80_hwnd();

#endif

