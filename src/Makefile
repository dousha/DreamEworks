AS=nasm
GAS=i686-elf-as
CC=i686-elf-gcc
LD=i686-elf-ld

ASM_FLAG=-f elf
ASM_SRC=stage2.asm hybintf.asm intrupt.asm
ASM_OBJ=$(ASM_SRC:.asm=.o)
ASM_TARGET=bl.bin
C_SRC=screen.c kern.c memutils.c utils.c string.c memory.c \
interrupts.c idt.c keyboard.c task.c gdt.c shell.c
C_DEP=vga.h hybintf.h memutils.h utils.h string.h memory.h \
interrupts.h idt.h keyboard.h task.h gdt.h shell.h
C_OBJ=$(C_SRC:.c=.o)
C_FLAG=-std=gnu99 -ffreestanding -O2 -Os -Wall -Wextra
TOTAL_TARGET=kernel.pkg

LINKER=linker.ld

EX_DEP=hosttools/edhd

TARGET=hosttools/hd.img

default:
	make all

run: $(TARGET)
	cd hosttools; \
	qemu-system-i386 -fda hd.img -monitor stdio;

dbg: $(TARGET)
	bochs -q -f ~/.bochsrc

all: $(TOTAL_TARGET) $(C_DEP) $(EX_DEP)
	cp bl.bin hosttools/mbr.bin
	cp kernel.pkg hosttools/
	cd hosttools; \
	./edhd -u; \
	./edhd -r kern; \
	./edhd -p kernel.pkg kern;

clean:
	rm -rf $(ASM_TARGET) $(ASM_OBJ) $(C_OBJ)

kernel.pkg : $(ASM_OBJ) $(C_OBJ) $(ASM_TARGET)
	$(CC) $(C_FLAG) -nostdlib -lgcc -T $(LINKER) -o $@ $(ASM_OBJ) $(C_OBJ)

%.o : %.asm
	$(AS) -f elf -o $@ $<

%.o : %.c
	$(CC) $(C_FLAG) -c -o $@ $<

bl.bin : bl.asm
	$(AS) -f bin -l bl.lst -o $@ $<

hosttools/edhd: hosttools/edhd.c hosttools/Makefile
	cd hosttools; make;
