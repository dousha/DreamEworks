#include "hybintf.h"
#include "tss.h"
#include "gdt.h"
#include "task.h"
#include "utils.h"
#include "memutils.h"
#include "memory.h"
#include "screen.h"

static task_manager* tm = 0;
static tss* t0 = NULL;
static tss* t1 = NULL;
static uint32_t freezer = 0;
static uint16_t curtid = 5;

void task_blank();

task_manager* task_man_get(){
	return tm; /// this variable should be
	/// in the shared memory
}

size_t task_init(size_t size){
	// task manager would take 2% of highmem
	t0 = (tss*) malloc(sizeof(tss));
	t1 = (tss*) malloc(sizeof(tss));
	memfill_b((uint8_t*) t0, sizeof(tss), 0);
	memfill_b((uint8_t*) t1, sizeof(tss), 0);
	t0->esp0 = 0;
	t0->ss0 = 0x20;
	t0->iopb = 104;
	t0->ldtr = 0;
	t1->esp0 = 0;
	t1->ss0 = 0x20;
	t1->iopb = 104;
	t1->eip = (uint32_t) &task_blank;
	t1->eflags = 0x00001202;
	t1->ss = 0x10;
	t1->esp = (uint32_t) malloc(1024) + 1024;
	t1->cs = 0x08;
	t1->ds = 0x10;
	t1->es = 0x10;
	t1->fs = 0x10;
	t1->gs = 0x18;
	gdtr* gr0 = (gdtr*) io_gdtr_read();
	gdt* g0 = (gdt*) gr0->offset;
	encode((g0 + 5), (uint32_t) t0, sizeof(tss), 0x89, 0x40);
	encode((g0 + 6), (uint32_t) t1, sizeof(tss), 0x89, 0x40);
	io_load_tss(5 * 8);

	size_t tableLen = (size - sizeof(task_manager) - sizeof(tss)) / 100;
	tm = (task_manager*) malloc(sizeof(task_manager));
	tm->task_count = 0;
	tm->task_list = (task*) malloc(tableLen / 2);
	tm->task_len = tableLen / 2 / sizeof(task);
	tm->delay_list = (delay*) malloc(tableLen / 2);
	tm->delay_count = 0;
	tm->delay_len = tableLen / 2 / sizeof(delay);
	tm->current_pid = 0;
	tm->tick = 100;
	tm->uptime = 0;

	return tableLen;
}

void task_switch(pid_t id){
	// on task switching
	// disable system calls
	// in case of stack ruining

}

pid_t task_create(void* code, void* data, void* stack){
	// create a task where it's text starts at prgm
	// the program and it's layout shall be loaded into memory
	// by elf_loader

}

void task_tick(){
	/*
	if(--tm->tick == 0){
		if(++tm->current_pid == tm->task_count){
			tm->current_pid = 0;
		}
		tm->tick = ((tm->task_list)[tm->current_pid]).tick;
		task_switch(tm->current_pid);
	}
	*/
	if(curtid == 5){
		curtid = 6;
		far_jump(0, 6*8);
	}else{
		curtid = 5;
		far_jump(0, 5*8);
	}
}

void task_finalize(){
	free(tm->delay_list);
	free(tm->task_list);
	free(tm);
	free(t0);
}

void task_blank(){
	for(;;);
}

pid_t delay_create(uint32_t d, void (*callback)()){
	tm->delay_list[tm->delay_count].delay_mills = tm->uptime + d;
	tm->delay_list[tm->delay_count].callback = callback;
	++tm->delay_count;
	return tm->delay_count - 1;
}

void delay_poll(){
	if(freezer > 0) --freezer;
	size_t i = 0;
	for(i = 0; i < tm->delay_count; i++){
		if(tm->delay_list[i].delay_mills < tm->uptime){
			if(tm->delay_list[i].callback != NULL){
				tm->delay_list[i].callback();
				tm->delay_list[i].callback = NULL; // deactivate
			}
		}
	}
}

void delay_cancel(pid_t pid){
	// XXX: just not really good to do this
	// check the ptr before you do
	tm->delay_list[pid].callback = NULL;
}

void dbg_freeze(uint32_t counts){
	// freeze the system
	freezer = counts;
	while(freezer > 0)
		io_stihlt();
}
