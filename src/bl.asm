; bootloader
; -f bin
; note that after jumping into stage2
; this code will be covered by stack

[bits 16]
[org 0x7c00]

TOTAL_SECTORS equ 360 ; how many sectors do we want to read?
; 1 cylinder = 2 x 18 sectors = 2 x 18 x 512 bytes = 18432 bytes

_stage1:
mov sp, 0x7c00 ; stack rasies upwards, 0x500 ~ 0x7bff are free
xor ax, ax
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

fstrcmp: ; si -> src, di -> dest
	push ax
	push si
	push di
	xor ax, ax
.cnt:
	mov al, [si]
	cmp al, 0
	je .done
	cmp al, [di]
	jne .noteq
	add si, 1
	add di, 1
	jmp .cnt
.done:
	stc
	pop di
	pop si
	pop ax
	ret
.noteq:
	clc
	pop di
	pop si
	pop ax
	ret

read_disk:
	; es:bx -> dest
	; ax -> sector (0 - 2880)
	push ax
	push cx
	push dx
	push si
	xor si, si
	xor dx, dx
	mov cl, 18
	div cl
	mov cl, ah
	add cl, 1
	mov dl, al
	mov ax, dx ; this flushes ah
	xor dx, dx
	mov dl, 2
	div dl
	mov ch, al
	xor dx, dx
	mov dh, ah
	mov dl, 0
.retry:
	mov ah, 2
	mov al, 1
	add si, 1
	cmp si, 5
	jge bad_boot
	int 0x13
	jc .retry
	call printdot
	pop si
	pop dx
	pop cx
	pop ax
	ret

load_stage2:
	; the following code shall load kernel into 0x8000
	; load table
	xor ax, ax
	mov es, ax
	mov bx, 0x8000
	mov ax, 1
	call read_disk
	mov si, table_string
	mov di, bx
	call fstrcmp
	jnc bad_boot
	; 16 records per sector
	mov si, kern_string
	xor cx, cx
.cnt:
	add cx, 1
	cmp cx, 16
	jg .next_sector
	add di, 32
	call fstrcmp
	jc .found
	jmp .cnt
.next_sector:
	add ax, 1
	call read_disk ; bx = 0x8000, this would cover them up
	mov di, bx
	cmp BYTE [di], 0
	je bad_boot ; kernel not found
	jmp .cnt
.found:
	add di, 8
	mov DWORD eax, [di]
	add di, 4
	mov DWORD edx, [di]
	xor ecx, ecx
	mov ecx, 512
	div ecx
	cmp edx, 0
	jne hang ; ...FIXME
	mov dx, 0x800
	mov es, dx
	xor bx, bx
	add ax, 0x10; edhd bug??
	mov cx, ax
	add cx, TOTAL_SECTORS
.continue:
	add ax, 1
	cmp ax, cx
	jg .read_done
	call read_disk
	add dx, 0x20
	mov es, dx
	jmp .continue
.read_done:
	jmp 0x8000 ; kernel loaded in this addr

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

; ---------- data

table_string:
	db "#REC_TBL"
	db 0
kern_string:
	db "kern    "
	db 0
die_msg:
	db "FAILED READING DISK"
	db 0x0a, 13, 0
no_mem_probe_msg:
	db "CANNOT PROBE MEMORY"
	db 0x0a, 13, 0
load_done_msg:
	db "done"
	db 0x0a, 13, 0

_stage1_end:
times 510-($-$$) db 0
db 0x55, 0xaa
