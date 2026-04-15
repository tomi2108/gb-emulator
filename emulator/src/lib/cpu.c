#include "../include/cpu.h"
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <stdlib.h>

#define ZERO_FLAG 0b10000000
#define SUB_FLAG 0b01000000
#define HALF_CARRY_FLAG 0b00100000
#define CARRY_FLAG 0b00010000

regs registers = {0};

t_config *cpu_config;
log_t cpu_logger = {.file = "log.log",
                    .process = "CPU",
                    .level = LOG_LEVEL_DEBUG,
                    .is_active_console = 1};

void instruction_noop() {}

void set_reg16(reg r, u16 val) {
  u8 high = (val >> 8) & 0xFF;
  u8 low = val & 0xFF;
  switch (r) {
  case R_BC:
    registers.B = high;
    registers.C = low;
    break;
  case R_DE:
    registers.D = high;
    registers.E = low;
    break;
  case R_HL:
    registers.H = high;
    registers.L = low;
    break;
  case R_AF:
    registers.A = high;
    registers.F = low & 0xF0;
    break;
  case R_SP:
    registers.SP = val;
    break;
  default:
    log_error(cpu_logger.logger, "Invalid set_reg16");
    exit(1);
  }
}

void set_reg8(reg r, u8 val) {
  switch (r) {
  case R_A:
    registers.A = val;
    break;
  case R_B:
    registers.B = val;
    break;
  case R_C:
    registers.C = val;
    break;
  case R_D:
    registers.D = val;
    break;
  case R_E:
    registers.E = val;
    break;
  case R_H:
    registers.H = val;
    break;
  case R_L:
    registers.L = val;
    break;
  default:
    log_error(cpu_logger.logger, "Invalid set_reg8");
    exit(1);
  }
}

u16 join_registers(u8 upper, u8 lower) { return ((u16)upper << 8) | lower; }

u16 get_reg16(reg r) {
  switch (r) {
  case R_SP:
    return registers.SP;
  case R_AF:
    return join_registers(registers.A, registers.F);
  case R_BC:
    return join_registers(registers.B, registers.C);
  case R_DE:
    return join_registers(registers.D, registers.E);
  case R_HL:
    return join_registers(registers.H, registers.L);
  default:
    log_error(cpu_logger.logger, "Invalid get_reg16");
    exit(1);
  }
}

u8 get_reg8(reg r) {
  switch (r) {
  case R_A:
    return registers.A;
  case R_B:
    return registers.B;
  case R_C:
    return registers.C;
  case R_D:
    return registers.D;
  case R_E:
    return registers.E;
  case R_H:
    return registers.H;
  case R_L:
    return registers.L;
  default:
    log_error(cpu_logger.logger, "Invalid get_reg8");
    exit(1);
  }
}

u16 get_imm16() {
  u16 val = ram_read16(registers.PC);
  registers.PC += 2;
  return val;
};

u8 get_imm8() {
  u8 val = ram_read8(registers.PC);
  registers.PC += 1;
  return val;
};

u16 read_operand16(operand op) {
  switch (op.type) {

  case OT_IMM16: {
    return get_imm16();
  }

  case OT_REG16:
    return get_reg16(op.reg);

  case OT_MEM_REG: {
    u16 addr = get_reg16(op.reg);
    return ram_read16(addr);
  }

  case OT_MEM_IMM16: {
    u16 addr = get_imm16();
    return ram_read16(addr);
  }

  default:
    log_error(cpu_logger.logger, "Invalid read_operand16");
    exit(1);
  }
}

void write_operand16(operand op, u16 val) {
  switch (op.type) {

  case OT_REG16:
    set_reg16(op.reg, val);
    break;

  case OT_MEM_REG: {
    u16 addr = get_reg16(op.reg);
    ram_write16(addr, val);
    break;
  }

  case OT_MEM_IMM16: {
    u16 addr = get_imm16();
    ram_write16(addr, val);
    break;
  }

  default:
    log_error(cpu_logger.logger, "Invalid write_operand16");
    exit(1);
  }
}

u8 read_operand8(operand op) {
  switch (op.type) {

  case OT_IMM8: {
    return get_imm8();
  }

  case OT_REG:
    return get_reg8(op.reg);

  case OT_MEM_REG: {
    u16 addr = get_reg16(op.reg);
    return ram_read8(addr);
  }

  case OT_MEM_IMM16: {
    u16 addr = get_imm16();
    return ram_read8(addr);
  }

  default:
    log_error(cpu_logger.logger, "Invalid read_operand8");
    exit(1);
  }
}

void write_operand8(operand op, u8 val) {
  switch (op.type) {

  case OT_REG:
    set_reg8(op.reg, val);
    break;

  case OT_MEM_REG: {
    u16 addr = get_reg16(op.reg);
    ram_write8(addr, val);
    break;
  }

  case OT_MEM_IMM16: {
    u16 addr = get_imm16();
    ram_write8(addr, val);
    break;
  }

  default:
    log_error(cpu_logger.logger, "Invalid write_operand8");
    exit(1);
  }
}

void instruction_load(instruction i) {
  switch (i.dst.type) {
  case OT_REG16: {
    u16 val = read_operand16(i.src);
    write_operand16(i.dst, val);
  } break;

  case OT_REG:
  case OT_MEM_IMM16:
  case OT_MEM_REG: {
    u8 val = read_operand8(i.src);
    write_operand8(i.dst, val);
  } break;

  default:
    log_error(cpu_logger.logger, "Invalid instruction_load");
    exit(1);
  }
}

void instruction_exec(instruction i) {
  switch (i.op) {
  case LD:
    instruction_load(i);
    break;
  default:
    instruction_noop();
  }
}

void cpu_init() {
  srand(time(NULL));
  cpu_logger.logger =
      log_create(cpu_logger.file, cpu_logger.process,
                 cpu_logger.is_active_console, cpu_logger.level);
  cpu_config = config_create("cpu.config");
  if (!cpu_config)
    exit_enoent_error(cpu_logger.logger, "cpu.config");
}

void cpu_exec() {
  instruction i = get_instruction(ram_read8(registers.PC));
  registers.PC++;
  instruction_exec(i);
}

void cpu_free() { log_destroy(cpu_logger.logger); }
