#include "../include/cpu.h"
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <stdlib.h>

regs registers = {0};

t_config *cpu_config;
log_t cpu_logger = {.file = "log.log",
                    .process = "CPU",
                    .level = LOG_LEVEL_DEBUG,
                    .is_active_console = 1};

void set_flag(u8 flag, bool to) {
  if (to)
    registers.F |= flag;
  else
    registers.F &= ~flag;
}

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
  cpu_advance();
  cpu_advance();
  return val;
};

u8 get_imm8() {
  u8 val = ram_read8(registers.PC);
  cpu_advance();
  return val;
};

void cpu_init() {
  srand(time(NULL));
  cpu_logger.logger =
      log_create(cpu_logger.file, cpu_logger.process,
                 cpu_logger.is_active_console, cpu_logger.level);
  cpu_config = config_create("cpu.config");
  if (!cpu_config)
    exit_enoent_error(cpu_logger.logger, "cpu.config");
}

void cpu_advance() { registers.PC++; }

void cpu_free() { log_destroy(cpu_logger.logger); }
