#ifndef _PIPER_H_
#define _PIPER_H_

#include <stdint.h>
#include <stddef.h>

typedef
struct listenser{
	uint32_t signal;
	uint8_t (*callback)(uint8_t*, size_t);
}
listener;

typedef
struct piper{
	uint8_t* piper;
	size_t size;
	size_t rp;
	size_t wp;
}
piper;

piper piper_init();
void piper_send(uint8_t);
listener* piper_register();
listener* piper_assign(uint32_t);
void piper_finalize();

#endif

