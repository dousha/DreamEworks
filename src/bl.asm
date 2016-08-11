; bootloader
; -f bin
; note that after jumping into stage2
; this code will be covered by stack

[bits 16]
[org 0x7c00]

TOTAL_CYLINDERS equ 10 ; how many cylinders do we want to read?
; 1 cylinder = 2 x 18 sectors = 2 x 18 x 512 Bytes

_stage1:
mov sp, 0x7c00 ; stack rasies upwards, 0x500 ~ 0x7bff are free
xor ax, ax
mov si, loading_msg
call printstr ; say hello
call mem_probe ; get memory info
jmp load_stage2 ; load stage 2

; --------- sub functions

printdot:
	push ax
	xor ax, ax
	mov ah, 0x0e
	mov al, '.'
	int 0x10
	pop ax
	ret

printstr: ; si -> str_start
	xor ax, ax
	mov ah, 0x0e
	mov al, [si]
	cmp al, 0
	je printstr_break
	int 0x10
	add si, 1
	jmp printstr
printstr_break:
	ret

load_stage2:
	; the following code shall load
	; the first 10 cylinders of the floppy 
	; in to 0x8000
	mov ax, 0x0820 ; since we skipped the first sector (0x20)
	mov es, ax
	mov ch, 0 ; cylinder 0
	mov dh, 0 ; head 0
	mov cl, 2 ; sector 2
	rl:
		mov si, 0
	retry:
		mov ah, 0x02
		mov al, 1
		xor bx, bx
		mov dl, 0
		int 0x13
		jnc next
		add si, 1
		cmp si, 5
		jae bad_boot
		xor ax, ax
		mov dl, 0
		jmp retry
	next:
		call printdot
		mov ax, es
		add ax, 0x0020
		mov es, ax
		add cl, 1
		cmp cl, 18
		jbe rl
		mov cl, 1
		add dh, 1
		cmp dh, 2
		jb rl
		mov dh, 0
		add ch, 1
		cmp ch, TOTAL_CYLINDERS
		jb rl
	jmp 0xd200 ; then jump to the stage2 code
	; 0xd200 = 0x5200 + 0x8000
	; `edhd` would put file content into 0x5200
	; please refer to the fs.note.md in notes/ for
	; file system details

mem_probe:
	xor ebx, ebx
	mov es, bx
	mov di, 0x7e00 ; right next to the gap between kernel and boot loader
mem_probe_loop:
	mov eax, 0xe820
	mov edx, 0x534D4150 ; magic number
	mov ecx, 20
	int 0x15
	jc mem_probe_error
	cmp eax, 0x534D4150
	jne mem_probe_error
	cmp ebx, 0
	je mem_probe_finish
	add di, cx
	jmp mem_probe_loop
mem_probe_finish:
	mov ax, di
	mov word [0x1000], ax
	ret
mem_probe_error:
	mov si, no_mem_probe_msg
	call printstr
	ret

hang:
	hlt
	jmp hang

bad_boot:
	mov si, die_msg
	call printstr
	jmp hang

loading_msg: 
	db "dsys v0.0.1"
	db 0x0a, 13, 0
die_msg:
	db "FAILED READING DISK"
	db 0x0a, 13, 0
no_mem_probe_msg:
	db "CANNOT PROBE MEMORY"
	db 0x0a, 13, 0

_stage1_end:
times 510-($-$$) db 0
db 0x55, 0xaa
