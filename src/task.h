#ifndef _TASK_H_
#define _TASK_H_

#include <stdint.h>
#include <stddef.h>

#include "tss.h"

#define TASK_LIVE 0x01
#define TASK_SLEEP 0x02
#define TASK_SUSPEND 0x04
#define TASK_DEAD 0x08

typedef uint32_t pid_t;

typedef
struct task{
	pid_t id;
	uint8_t tick;
	uint8_t status;
	void* code_chunk;
	tss* tss;
} __attribute__((packed))
task;

typedef
struct delay{
	uint32_t delay_mills;
	void (*callback)();
} __attribute__((packed))
delay;

typedef
struct task_manager{
	task* task_list;
	delay* delay_list;
	pid_t current_pid;
	uint32_t task_count;
	uint32_t delay_count;
	uint8_t tick;
	uint32_t uptime;
} __attribute__((packed))
task_manager;

extern task_manager* task_man_get();
extern size_t task_init(size_t);
extern void task_finalize();
extern pid_t task_create(void*);
extern void task_finalize();
extern pid_t delay_create(uint32_t, void *(callback)());
extern void delay_cancel(pid_t);
extern void delay_poll();

extern void dbg_freeze(uint32_t);

#endif

