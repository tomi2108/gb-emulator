#ifndef MEMORY_H_
#define MEMORY_H_

#include "common.h"
#include <commons/collections/list.h>

void memory_init();
void memory_free();

void ram_write(const u8 *stream, u32 length, u16 offset);
u8* ram_read(u16 offset);

void stack_push(u16 bytes);

u16 *stack_pop();

#endif
