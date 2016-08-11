; stage2

global entry
extern c_kern

[section .gdt]
; GDTs are defined here

; note that we have just set a FLAT gdt
; protection is not presented yet
; needs to be fixed in further development

GDT_DESC_START:

GDT0:
dw 0 ; Limit 0~15
dw 0 ; Base 0~15
db 0 ; Base 16~23
db 0 ; Access Byte
db 0 ; Limit 16~19 + Flags
db 0 ; Base 24~31

DESCRIPTOR_CODE:
dw 0xffff ; Limit 0~15
CODE_BASE_AX dw 0 ; Base 0~15
CODE_BASE_EAHL db 0 ; Base 16~23
db 0b10011011 ; 1001 1011 - Vaild, ring 0, executable, strict, readable,
; accessed
db 0b11001111 ; 1100 1111 - paging, 32-bit
CODE_BASE_EAHH db 0 ; Base 24~31

DESCRIPTOR_DATA:
dw 0xffff ; Limit 0~15
dw 0x0000 ; Base 0~15
db 0x00 ; Base 16~23
db 0b10010011 ; 1001 0011 - vaild, ring 0, data, forward, writable, accessed
db 0b11001111 ; 1100 1111 - 4KB, 32-bits
db 0x0 ; Base 24~31

; Note that in further development, VRAM descriptor should be
; created dynamically
; Base = 0x000b8000
DESCRIPTOR_VIDEO:
dw 0xffff
dw 0x8000
db 0x0b
db 0b10010011
db 0b01001111
db 0x00

DESCRIPTOR_STACK:
dw 0x7000
dw 0x7b00
db 0x00
db 0b10010111
db 0b01000000
db 0x00

GDT_DESC_END:

GDTR0:
	dw GDT_DESC_END - GDT_DESC_START - 1
	dd GDT_DESC_START

SELECTOR_CODE equ DESCRIPTOR_CODE - GDT0
SELECTOR_DATA equ DESCRIPTOR_DATA - GDT0
SELECTOR_VIDEO equ DESCRIPTOR_VIDEO - GDT0
SELECTOR_STACK equ DESCRIPTOR_STACK - GDT0

[section .idt]
; idts are defined here
IDT_DESC_START:

IDT_DESC_END:

IDTR0:
	dw IDT_DESC_END - IDT_DESC_START
	dd IDT_DESC_START

; --- code ------------

[section .text]
[bits 16]

entry:

cli
; set graphics mode

; open a20 gate
; the dirty way
in al, 0x92
or al, 0x02
and al, ~1
out 0x92, al
; goto protected mode
; rewrite code descriptor
; load the gdt
lgdt [GDTR0]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp dword SELECTOR_CODE:entry_32 ; -> protected mode!!!

[bits 32]
entry_32:
; set up segments
mov ax, SELECTOR_DATA
mov ds, ax ; ds varys with each task
mov es, ax ; es can be used freely... yet
mov fs, ax ; fs is reserved as flat segment
mov gs, ax ;
mov ax, SELECTOR_STACK
mov ss, ax
mov ax, 0x7b00
mov sp, ax

; move to C kernel

jmp c_kern
