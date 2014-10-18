ipl.bin : ipl.asm Makefile
	./z_tools/nask.exe ipl.asm ipl.bin

dmwk.img : ipl.bin Makefile
	./z_tools/edimg.exe imgin:fdimg0at.tek wbinimg src:ipl.bin len:512 from:0 to:0 imgout:dmwk.img
	
all:
	make exu

exu :
	make -r dmwk.img
	make run

run :
	qemu-system-i386 -m 32 -localtime -fda dmwk.img
