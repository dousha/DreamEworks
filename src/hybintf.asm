; hybird interface

extern IDTR0

global io_hlt, io_cli, io_sti
global io_out8, io_out16, io_out32
global io_in8, io_in16, io_in32
global io_eflags_read, io_eflags_write
global io_idtr_read, io_idtr_write
global io_int_user

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
