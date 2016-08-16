#ifndef _IDT_H_
#define _IDT_H_

#define TASK_GATE_32 0x5
#define INT_GATE_16 0x6
#define TRAP_GATE_16 0x7
#define INT_GATE_32 0xE
#define TRAP_GATE_32 0xF

#define IDT_PRESENT 0x80
#define IDT_DPL_KERN 0x00
#define IDT_DPL_USER 0x40
#define IDT_INT_GATE 0x10

typedef
struct idtr{
	uint16_t size;
	uint32_t offset;
} __attribute__((packed))
idtr;

typedef
struct idt{
	uint16_t offset0015;
	uint16_t selector;
	uint8_t padding;
	uint8_t type;
	uint16_t offset1631;
} __attribute__((packed))
idt;

extern void idt_maker(idt*, void*, uint16_t, uint8_t);
extern void init_idt(void*);

#endif

