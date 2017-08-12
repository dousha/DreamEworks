; hybird interface

extern IDTR0
extern GDTR0
extern SELECTOR_TSS
extern SELECTOR_USER_TSS

global io_hlt, io_cli, io_sti, io_stihlt
global io_out8, io_out16, io_out32
global io_in8, io_in16, io_in32
global io_eflags_read, io_eflags_write
global io_idtr_read, io_idtr_write
global io_int_user
global io_gdtr_read, io_gdtr_write
global io_load_tss, io_load_user_tss
global io_current_esp
global far_jump, far_call, test_jump
global exp_ud, int_dbg

[section .text]
io_in8:
; uint8_t
; uint16_t port
	mov dx, [esp + 4]
	xor eax, eax
	in al, dx
	ret

io_in16:
; uint16_t
; uint16_t port
	mov dx, [esp + 4]
	xor eax, eax
	in ax, dx
	ret

io_in32:
; uint32_t
; uint16_t port
	mov dx, [esp + 4]
	xor eax, eax
	in eax, dx
	ret

io_out8:
; void
; int al
; int port
	xor eax, eax
	mov al, [esp + 4]
	mov dx, [esp + 8]
	out dx, al
	ret

io_out16:
; void
; int ax
; int port
	xor eax, eax
	mov ax, [esp + 4]
	mov dx, [esp + 8]
	out dx, ax
	ret

io_out32:
; void
; int eax
; int port
	mov eax, [esp + 4]
	mov dx, [esp + 8]
	out dx, eax
	ret

io_eflags_read:
; uint32_t
	pushfd
	pop eax
	ret

io_eflags_write:
; void
; uint32_t efl
	mov eax, [esp + 4]
	push eax
	popfd
	xor eax, eax
	ret

io_hlt:
; void
	hlt
	ret

io_cli:
; void
	cli
	ret

io_sti:
	sti
	ret

io_stihlt:
	sti
	hlt
	ret

io_idtr_read:
; idtr*
	mov eax, IDTR0
	ret

io_idtr_write:
; void
; idtr*
	mov eax, [esp + 4]
	lidt [eax]
	ret

io_int_user:
; void
	int 0x80
	ret

io_gdtr_read:
; gdtr*
	mov eax, GDTR0
	ret

io_gdtr_write:
; void
; gdtr*
	mov eax, [esp + 4]
	lgdt [eax]
	ret

io_load_tss:
; void
; int seg
	ltr [esp + 4]
	ret

io_current_esp:
; uint32_t esp
	mov eax, esp
	ret

test_jump:
	jmp 0x30:0
	ret

far_jump:
; int eip
; int cs
	jmp FAR [esp + 4]
	ret

far_call:
; int eip
; int cs
	call FAR [esp + 4]
	retf

io_has_cpuid:
; int (as bool)
	pushfd
	pushfd
	xor DWORD [esp], 0x00200000
	popfd
	pushfd
	pop eax
	xor eax, [esp]
	popfd
	and eax, 0x00200000
	ret

exp_ud:
; void, exception
	ud2
	ret

int_dbg:
; void, interrupt
	xor eax, eax
	int 0x80
	ret
