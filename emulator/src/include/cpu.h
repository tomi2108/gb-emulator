#ifndef CPU_H_
#define CPU_H_

#include "common.h"
#include "instructions.h"
#include "keypad.h"
#include "memory.h"
#include "ppu.h"
#include "timer.h"
#include <commons/buffer.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/sys/exit.h>
#include <commons/sys/file.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  u8 A;
  u8 B;
  u8 C;
  u8 D;
  u8 E;
  u8 F;
  u8 H;
  u8 L;
  u16 PC;
  u16 SP;
} regs;

void cpu_init();

void cpu_free();
void cpu_exec();

#endif
