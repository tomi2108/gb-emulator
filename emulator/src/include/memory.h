#ifndef MEMORY_H_
#define MEMORY_H_

#include "common.h"
#include <commons/collections/list.h>

void memory_init();
void memory_free();

void ram_write(const u8 *stream, u32 length, u16 offset);

u8 ram_read8(u16 addr);
u16 ram_read16(u16 addr);

void ram_write8(u16 addr, u8 val);
void ram_write16(u16 addr, u16 val);

#endif
