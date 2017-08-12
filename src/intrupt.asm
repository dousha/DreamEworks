global int_dummy
global int_kbd, int_timer, int_task_tick, int_user_0x80
global init_8259a, init_pit
global int_e_div, int_e_dbg, int_e_nmi, int_e_bp, int_e_overflow
global int_e_bound, int_e_undef, int_e_no_math, int_e_double_fault
global int_e_aux_bound, int_e_invalid_tss, int_e_not_present, int_e_stack_segment
global int_e_general_protection, int_e_page_fault, int_e_reserved
global int_e_math_fault, int_e_align_check, int_e_machine_check
global int_e_sse_fault

extern int_kbd_hwnd, int_timer_hwnd, int_task_tick_hwnd, int_user_0x80_hwnd
extern int_exception_hwnd

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
	mov al, 0b11111100 ; kbd
			;0b11111110 ; timer interrupt only
			;0b11111111 ; no interrupt
	out 0x21, al ; main, ocw1
	call io_delay

	mov al, 0b11111111 ; no interrupt
	out 0xa1, al
	call io_delay

	pop ax
	ret

;init_pit:
;	push eax
;	push edx
;	xor eax, eax
;	mov al, 0b00110100
;	mov dx, 0x43
;	out dx, al
;	mov ax, 0x04aa
;	mov dx, 0x40
;	out dx, ax
;	pop edx
;	pop eax
;	ret

init_pit: ; 11932, 100Hz (approx)
	push eax
	push edx
	xor eax, eax
	xor edx, edx
	mov dx, 0x43
	mov ax, 0x34
	out dx, al
	mov dx, 0x40
	mov al, 0x9c
	out dx, al
	mov al, 0x2e
	out dx, al
	pop edx
	pop eax
	ret

int_e_div:
	push 0xffffffff
	push 0
	jmp int_ex
int_e_dbg:
	push 0xffffffff
	push 1
	jmp int_ex
int_e_nmi:
	push 0xffffffff
	push 2
	jmp int_ex
int_e_bp:
	push 0xffffffff
	push 3
	jmp int_ex
int_e_overflow:
	push 0xffffffff
	push 4
	jmp int_ex
int_e_bound:
	push 0xffffffff
	push 5
	jmp int_ex
int_e_undef:
	push 0xffffffff
	push 6
	jmp int_ex
int_e_no_math:
	push 0xffffffff
	push 7
	jmp int_ex
int_e_double_fault:
	push 8
	jmp int_ex
int_e_aux_bound:
	push 0xffffffff
	push 9
	jmp int_ex
int_e_invalid_tss:
	push 10
	jmp int_ex
int_e_not_present:
	push 11
	jmp int_ex
int_e_stack_segment:
	push 12
	jmp int_ex
int_e_general_protection:
	push 13
	jmp int_ex
int_e_page_fault:
	push 14
	jmp int_ex
int_e_reserved:
	push 0xffffffff
	push 15
	jmp int_ex
int_e_math_fault:
	push 0xffffffff
	push 16
	jmp int_ex
int_e_align_check:
	push 17
	jmp int_ex
int_e_machine_check:
	push 0xffffffff
	push 18
	jmp int_ex
int_e_sse_fault:
	push 0xffffffff
	push 19
	jmp int_ex
int_ex:
	call int_exception_hwnd
	add esp, 4 * 2
	cli
	hlt

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
