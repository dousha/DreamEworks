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
	idt_maker(&i2, int_e_div, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x00] = i2;
	idt_maker(&i2, int_e_dbg, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x01] = i2;
	idt_maker(&i2, int_e_nmi, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x02] = i2;
	idt_maker(&i2, int_e_bp, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x03] = i2;
	idt_maker(&i2, int_e_overflow, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x04] = i2;
	idt_maker(&i2, int_e_bound, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x05] = i2;
	idt_maker(&i2, int_e_undef, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x06] = i2;
	idt_maker(&i2, int_e_no_math, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x07] = i2;
	idt_maker(&i2, int_e_double_fault, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x08] = i2;
	idt_maker(&i2, int_e_aux_bound, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x09] = i2;
	idt_maker(&i2, int_e_invalid_tss, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x0a] = i2;
	idt_maker(&i2, int_e_not_present, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x0b] = i2;
	idt_maker(&i2, int_e_stack_segment, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x0c] = i2;
	idt_maker(&i2, int_e_general_protection, 0x8, IDT_PRESENT |
	INT_GATE_32);
	idts[0x0d] = i2;
	idt_maker(&i2, int_e_page_fault, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x0e] = i2;
	idt_maker(&i2, int_e_reserved, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x0f] = i2;
	idt_maker(&i2, int_e_math_fault, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x10] = i2;
	idt_maker(&i2, int_e_align_check, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x11] = i2;
	idt_maker(&i2, int_e_machine_check, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x12] = i2;
	idt_maker(&i2, int_e_sse_fault, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x13] = i2;

	idt_maker(&i2, int_timer, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x20] = i2; // 0x20 now should be clock counter
	idt_maker(&i2, int_kbd, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x21] = i2; // 0x21 now should be keyboard interrupt
	idt_maker(&i2, int_user_0x80, 0x8, IDT_PRESENT | INT_GATE_32);
	idts[0x80] = i2; // 0x80 now should be user interrupt
}

