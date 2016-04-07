; nasm -f bin
[code 16]
[org 0x7c00]
LOWMEM equ 0x7e00
SEC_PER_TRACK equ 18

_stage1:
mov sp, 0x7c00 ; stack rasies upwards, 0x500 ~ 0x7bff are free
xor ax, ax
mov si, loading_msg
call printstr

; load stage1.5 (real mode stuff) to 0x7e00


jmp 0x7e00 ; then jump to that address

; --------- sub functions

printstr: ; si -> str_start
	xor ax, ax
	mov ah, 0x0e
	mov al, [si]
	cmp al, 0
	je printstr_break
	int 0x10
	add si
	jmp printstr
printstr_break:
	ret

cmpstrn: ; si -> str1_start, di -> str2_start, cx -> str_len,
		 ; al = 0x1 if not equal
	xor ax, ax
cmpstrn_continue:
	cmp [si], [di]
	jne not_equal
	add si, 1
	add di, 1
	dec cx
	cmp cx, 0
	je equal
	jmp cmpstrn_continue

	not_equal:
		mov al, 0x01
		ret
	equal:
		ret

lba2chs: ; stack are used here
		 ; si -> lba addr
		 ; () <- [ cyl, head, sector ]
	


fread: ; si -> filename_start, di -> mem_start
	; convert offset to CHS address
	push si
	mov si, 512
	call lba2chs
	

loading_msg: 
	db "Loading"
	db 0x0a, 0

filename: ; 16
	db "STAGE15.BIN     "

_stage1_end:
times 510-($-$$) db 0
db 0x55, 0xaa

