#include <stdint.h>
#include <stddef.h>

#include "idt.h"
#include "interrupts.h"

void idt_maker(idt* i, void* fn, uint16_t selector, uint8_t type){
	uint32_t offset = (uint32_t) fn;
	i->selector = selector;
	i->padding = (uint8_t) 0;
	i->type = type;
	i->offset0015 = (uint16_t) (offset & 0x0000FFFF);
	i->offset1631 = (uint16_t) (offset >> 16);
}

void init_idt(void* idtr_addr){
	// there are 256 entries
	// idtr is passed here
	idtr* i = (idtr*) idtr_addr; // looks awkward
	idt* idts = (idt*) i->offset;
	idt i2;
	idt_maker(&i2, int_dummy, 0x8, IDT_PRESENT | INT_GATE_32);
	uint32_t k = 0;
	for(k = 0; k < 256; k++){
		idts[k] = i2;
	}
	idt_maker(&i2, int_timer, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x20] = i2; // 0x20 now should be clock counter
	idt_maker(&i2, int_kbd, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x21] = i2; // 0x21 now should be keyboard interrupt
	idt_maker(&i2, int_user_0x80, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x80] = i2; // 0x80 now should be user interrupt
}

