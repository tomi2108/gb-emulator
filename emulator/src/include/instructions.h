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
  RET,
  JR,
  JP,
  HALT,
  STOP,
  ADC,
  ADD,
  SUB,
  INC,
  DEC,
  AND,
  OR,
  LDH,
  PUSH,
  POP,
  DI,
  RLCA,
} op_code;
typedef enum { AM_16, AM_A16, AM_8, AM_R ,AM_IR} addressing_mode;
typedef enum {
  R_A,
  R_B,
  R_C,
  R_D,
  R_E,
  R_F,
  R_H,
  R_L,
  R_AF,
  R_BC,
  R_DE,
  R_HL,
  R_SP,
} reg;

typedef struct {
  op_code op;
  addressing_mode address_mode;
  reg reg_dest;
  reg reg_source;
} instruction;

instruction get_instruction(u8 byte);

#endif
