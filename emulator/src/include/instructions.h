#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include "common.h"
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

typedef enum {
  NOP,
  LD,
  CALL,
  CALL_NZ,
  JP_Z,
  RET_NC,
  CALL_NC,
  RET_C,
  RETI,
  JP_C,
  LDH,
  ADD_SP,
  JP_HL,
  LD_HL_SP,
  LD_SP_HL,
  CALL_C,
  JP_NC,
  CALL_Z,
  RET,
  JR,
  JP,
  HALT,
  STOP,
  ADC,
  ADD,
  SUB,
  SBC,
  INC,
  DEC,
  AND,
  OR,
  XOR,
  CP,
  PUSH,
  POP,
  DI,
  EI,
  RLCA,
  RRCA,
  RLA,
  RRA,
  DAA,
  CPL,
  SCF,
  CCF,
  RST,
  JR_Z,
  JR_NC,
  JR_C,
  RET_Z,
  RET_NZ,
  JP_NZ,
  JR_NZ,
  PREFIX
} op_code;
typedef enum {
  OT_NONE,
  OT_REG,         // A, B, C...
  OT_REG16,       // BC, DE, HL, SP
  OT_IMM8,        // n8
  OT_IMM16,       // n16 / a16
  OT_MEM_REG,     // [BC], [DE], [HL]
  OT_MEM_IMM16,   // [a16]
  OT_MEM_IMM8,    // [a8]
  OT_MEM_C,       // [C] (special case)
  OT_MEM_REG_INC, // [HL+],
  OT_MEM_REG_DEC  // [HL-],
} operand_type;
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

typedef struct {
  operand_type type;
  reg reg; // used if type involves a register
} operand;

typedef struct {
  op_code op;
  operand dst;
  operand src;
} instruction;

instruction get_instruction(u8 byte);

#endif
