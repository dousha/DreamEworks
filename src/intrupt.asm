global int_dummy
global int_kbd, int_timer, int_task_tick, int_user_0x80
global init_8259a

extern int_kbd_hwnd, int_timer_hwnd, int_task_tick_hwnd, int_user_0x80_hwnd

[section .text]
io_delay:
	nop
	nop
	nop
	nop
	ret

init_8259a:
	push ax
	xor ax, ax
	; icw1
	; 7
	; +-+-+-+-+-+-+-+-+
	; |R|R|R|1|L|A|S|I|
	; +-+-+-+-+-+-+-+-+
	; R - Unused
	; L - LTIM, 0:edge, 1:high
	; A - Unused
	; S - SNGL, 0:ICW3 present, 1:ICW3 not present
	; I - ICW4, 0:ICW4 not present, must be 1 in 8086
	mov al, 0b00010001
	out 0x20, al ; main, icw1
	call io_delay

	out 0xa0, al ; slave, icw1
	call io_delay

	mov al, 0x20 ; irq0 -> 0x20
	out 0x21, al ; main, icw2
	call io_delay

	mov al, 0x28 ; irq8 -> 0x28
	out 0xa1, al ; slave, icw2
	call io_delay

	; icw3
	; master
	; 7         0
	; +----+----+
	; |SSSS|SSSS|
	; +----+----+
	; if any of pins above is connected to a slave 8259 chip, set it to 1
	mov al, 0x04 ; ir2 -> slave 8259
	out 0x21, al ; main, icw3
	call io_delay
	
	; icw3
	; slave
	; 7
	; +----+----+
	; |0000|0DDD|
	; +----+----+
	;
	mov al, 0x02 ; ir2 -> master 8259
	out 0xa1, al ; slave, icw3
	call io_delay

	mov al, 0x01
	out 0x21, al ; main, icw4
	call io_delay

	out 0xa1, al ; slave, icw4
	call io_delay
	
	; ocw1: int 27 ~ 20
	; clk: int 20
	; kbd: int 21
	; com2: int 23
	; com1: int 24
	; lpt2: int 25
	; flp: int 26
	; ???: int 27
	mov al, 0b11111101 ; kbd
			;0b11111110 ; timer interrupt only
			;0b11111111 ; no interrupt
	out 0x21, al ; main, ocw1
	call io_delay
	
	mov al, 0b11111111 ; no interrupt
	out 0xa1, al
	call io_delay

	pop ax
	ret

int_dummy:
	; just do absolutely nothing, ok?
	iretd

int_kbd:
	pushad
	call int_kbd_hwnd
	popad
	iretd

int_timer:
	pushad
	cld
	call int_timer_hwnd
	popad
	iretd

int_task_tick:
	iret

int_user_0x80:
	pushad
	cld
	call int_user_0x80_hwnd
	popad
	iret
