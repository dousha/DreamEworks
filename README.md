DreamEworks - A Simple OS for Hobbyists
=========================================

# Requirements

You will need these tools to build and run this OS.

* gcc 5.x or higher
* GNU make
* NASM
* QEMU

Optional package for debugging.

* gdb

Really optional packages if you want to run it on a real machine.

* A (vintage) computer comes with 80386 or later
* 64M RAM or higher
* 3.5-inch floppy (1440K)
* 3.5-inch floppy drive (NOT USB floppy drive)
* Determination that you would like to run an experimental stuff that
can blow up your house
* Braveness that dare to push a charging Samsung Note 7 against your eyeballs

# Build

To start building, goto `src/` and type `make` or `make run` if you want
to run it after the build.

If you want to run this OS on a real stuff (highly unrecommended), `dd` the
`hd.img` file into a 1440K floppy disk and boot it. Hopefully everything 
could work.

# Licence

This program is licenced under GNU GPL v3 or later.

This program comes with ABSOLUTELY NO WARRANTY, use it at your own risk.

