global init_8259a

[section .text]
init_8259a:
	push ax
	xor ax, ax
	mov al, 0x11
	out 0x20, al

	pop ax
	ret
