#ifndef CPU_H_
#define CPU_H_

#include "common.h"
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

typedef enum {
  R_NONE,

  // 8-bit
  R_A,
  R_B,
  R_C,
  R_D,
  R_E,
  R_H,
  R_L,

  // 16-bit
  R_AF,
  R_BC,
  R_DE,
  R_HL,
  R_SP,
} reg;

void cpu_init();
void cpu_advance();
void cpu_free();

u8 get_imm8();
u16 get_imm16();

u8 get_reg8(reg r);
void set_reg8(reg r, u8 val);

u16 get_reg16(reg r);
void set_reg16(reg r, u16 val);

void set_flag(u8 flag, bool to);
bool get_flag(u8 flag);

#endif
