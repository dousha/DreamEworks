#include "hybintf.h"
#include "task.h"
#include "beep.h"

void beep(uint32_t freq){
	uint32_t divr;
	uint8_t tmp;
	divr = 1193180 / freq;
	io_out8(0xb6, 0x43);
	io_out8((uint8_t) divr, 0x42);
	io_out8((uint8_t) (divr >> 8), 0x42);
	tmp = io_in8(0x61);
	if(tmp != (tmp | 3)){
		io_out8(tmp | 3, 0x61);
	}
}

void shutup(){
	uint8_t tmp = io_in8(0x61) & 0xfc;
	io_out8(tmp, 0x61);
}

void hum(uint32_t freq, uint32_t mils){
	beep(freq);
	delay_create(mils, &shutup);
}
