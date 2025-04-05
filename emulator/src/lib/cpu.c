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

// void instruction_random(u8 reg, u8 nn) {
//   registers.R_X[reg] &= (rand() % 0xFF);
// }
//
// void instruction_jump(u16 nnn) { registers.R_PC = nnn; }
// void instruction_jump_offset_v0(u16 nnn) {
//   registers.R_PC = nnn + registers.R_X[0x0];
// }
// void instruction_jump_offset(u8 reg, u16 nnn) {
//   registers.R_PC = nnn + registers.R_X[reg];
// }
// void instruction_return() {
//   u16 *bytes = stack_pop();
//   registers.R_PC = *bytes;
// }
// void instruction_sub_routine(u16 nnn) {
//   stack_push(registers.R_PC);
//   registers.R_PC = nnn;
// }
//
// void instruction_skip_if_reg(u8 reg, u8 nn) {
//   if (registers.R_X[reg] == nn)
//     registers.R_PC += 2;
// }
// void instruction_skip_if_not_reg(u8 reg, u8 nn) {
//   if (registers.R_X[reg] != nn)
//     registers.R_PC += 2;
// }
// void instruction_skip_if_regs(u8 reg1, u8 reg2) {
//   if (registers.R_X[reg1] == registers.R_X[reg2])
//     registers.R_PC += 2;
// }
// void instruction_skip_if_not_regs(u8 reg1, u8 reg2) {
//   if (registers.R_X[reg1] != registers.R_X[reg2])
//     registers.R_PC += 2;
// }
// void instruction_skip_if_key(u8 reg) {
//   if (keypad_key_is_pressed(registers.R_X[reg]))
//     registers.R_PC += 2;
// }
// void instruction_skip_if_not_key(u8 reg) {
//   if (!keypad_key_is_pressed(registers.R_X[reg]))
//     registers.R_PC += 2;
// }
//
// void instruction_set_reg_d_timer(u8 reg) { registers.R_X[reg] =
// d_timer_get(); } void instruction_set_d_timer_reg(u8 reg) {
// d_timer_set(registers.R_X[reg]); } void instruction_set_s_timer_reg(u8 reg) {
// s_timer_set(registers.R_X[reg]); }
//
// void instruction_set_index(u16 nnn) { registers.R_I = nnn; }
// void instruction_set_nn(u8 reg_dest, u8 nn) { registers.R_X[reg_dest] = nn; }
// void instruction_set_regs(u8 reg_dest, u8 reg_source) {
//   registers.R_X[reg_dest] = registers.R_X[reg_source];
// }
//
// void instruction_or_regs(u8 reg_dest, u8 reg_source) {
//   registers.R_X[reg_dest] |= registers.R_X[reg_source];
// }
// void instruction_and_regs(u8 reg_dest, u8 reg_source) {
//   registers.R_X[reg_dest] &= registers.R_X[reg_source];
// }
// void instruction_xor_regs(u8 reg_dest, u8 reg_source) {
//   registers.R_X[reg_dest] ^= registers.R_X[reg_source];
// }
//
// void instruction_add_index(u8 reg_dest) {
//   if (config_get_int_value(cpu_config, "add_index_carry_flag") &&
//       (0x1000 - registers.R_I < registers.R_X[reg_dest]))
//     registers.R_X[0xF] = 1;
//
//   registers.R_I += registers.R_X[reg_dest];
// }
// void instruction_add_nn(u8 reg_dest, u8 nn) { registers.R_X[reg_dest] += nn;
// } void instruction_add_regs(u8 reg_dest, u8 reg_source) {
//   registers.R_X[0xF] = 0;
//   if (0xFF - registers.R_X[reg_dest] < registers.R_X[reg_source])
//     registers.R_X[0xF] = 1;
//   registers.R_X[reg_dest] += registers.R_X[reg_source];
// }
//
// void instruction_sub_regs(u8 reg_dest, u8 reg_source) {
//   registers.R_X[0xF] = 0;
//   if (registers.R_X[reg_dest] > registers.R_X[reg_source])
//     registers.R_X[0xF] = 1;
//   registers.R_X[reg_dest] = registers.R_X[reg_dest] -
//   registers.R_X[reg_source];
// }
// void instruction_sub_regs_i(u8 reg_dest, u8 reg_source) {
//   registers.R_X[0xF] = 0;
//   if (registers.R_X[reg_source] > registers.R_X[reg_dest])
//     registers.R_X[0xF] = 1;
//   registers.R_X[reg_dest] = registers.R_X[reg_source] -
//   registers.R_X[reg_dest];
// }
//
// void instruction_shift_left(u8 reg_dest, u8 reg_source) {
//   if (config_get_int_value(cpu_config, "shift_set"))
//     registers.R_X[reg_dest] = registers.R_X[reg_source];
//   bool bit = registers.R_X[reg_dest] & 0x80;
//   registers.R_X[reg_dest] <<= registers.R_X[reg_dest];
//   registers.R_X[0xF] = bit;
// }
// void instruction_shift_right(u8 reg_dest, u8 reg_source) {
//   if (config_get_int_value(cpu_config, "shift_set"))
//     registers.R_X[reg_dest] = registers.R_X[reg_source];
//   bool bit = registers.R_X[reg_dest] & 0x1;
//   registers.R_X[reg_dest] >>= registers.R_X[reg_dest];
//   registers.R_X[0xF] = bit;
// }
//
// void instruction_get_key(u8 reg) {
//   bool res = 1;
//   u8 key = keypad_get_key(&res);
//   if (res == 0)
//     registers.R_X[reg] = key;
//   else
//     registers.R_PC -= 2;
// }
//
// void instruction_font_character(u8 reg) {
//   u8 character = (0x000F & registers.R_X[reg]) >> 0;
//   registers.R_I = 0x50 + character * 5;
// }
//
// void instruction_binary_decimal_conversion(u8 reg) {
//   u8 byte = registers.R_X[reg];
//
//   u8 digit_1 = (byte / 100) % 10;
//   u8 digit_2 = (byte / 10) % 10;
//   u8 digit_3 = byte % 10;
//
//   ram_write(&digit_1, 1, registers.R_I);
//   ram_write(&digit_2, 1, registers.R_I + 1);
//   ram_write(&digit_3, 1, registers.R_I + 2);
// }
//
// void instruction_memory_write(u8 reg) {
//   for (u8 i = 0; i <= reg; i++) {
//     ram_write(&registers.R_X[reg], 1, registers.R_I + i);
//   }
//
//   if (config_get_int_value(cpu_config, "memory_access_modify_index"))
//     registers.R_I += reg + 1;
// }
// void instruction_memory_read(u8 reg) {
//   for (u8 i = 0; i <= reg; i++) {
//     u8 byte = ram_read(registers.R_I + i);
//     registers.R_X[reg] = byte;
//   }
//   if (config_get_int_value(cpu_config, "memory_access_modify_index"))
//     registers.R_I += reg + 1;
// }

// void instruction_draw(u8 reg_x, u8 reg_y, u8 n) {
//   registers.R_X[0xF] = 0;
//   u8 y = registers.R_X[reg_y] & (SCREEN_HEIGHT - 1);
//
//   for (u8 i = 0; i < n; i++) {
//     u8 x = registers.R_X[reg_x] & (SCREEN_WIDTH - 1);
//     u8 sprite_byte = ram_read(registers.R_I + i);
//
//     for (int j = 0; j < 8; j++) {
//
//       bool bit = (sprite_byte >> (7 - j)) & 0x1;
//       bool pixel = screen_get(x, y);
//
//       if (bit && pixel) {
//         screen_set(x, y, 0);
//         registers.R_X[0xF] = 1;
//       }
//       if (bit && !pixel)
//         screen_set(x, y, 1);
//
//       if (x > SCREEN_WIDTH)
//         break;
//       x++;
//     }
//     y++;
//     if (y > SCREEN_HEIGHT)
//       break;
//   }
// }

void set_register(reg r, u8 *to) {
  switch (r) {
  case R_A:
    registers.A = *to;
    break;
  case R_B:
    registers.B = *to;
    break;
  case R_C:
    registers.C = *to;
    break;
  case R_D:
    registers.D = *to;
    break;
  case R_E:
    registers.E = *to;
    break;
  case R_F:
    registers.F = *to;
    break;
  case R_H:
    registers.H = *to;
    break;
  case R_L:
    registers.L = *to;
    break;
  case R_SP:
    registers.SP = *to;
    break;
  case R_AF:
    registers.A = to[0];
    registers.F = to[1];
    break;
  case R_BC:
    registers.B = to[0];
    registers.C = to[1];
    break;
  case R_DE:
    registers.D = to[0];
    registers.E = to[1];
    break;
  case R_HL:
    registers.H = to[0];
    registers.L = to[1];
    break;
  }
}

u16 join_registers(u8 upper, u8 lower) { return (upper << 7) & 0xF0 | lower; };

u16 get_register(reg r) {
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
  case R_F:
    return registers.F;
  case R_H:
    return registers.H;
  case R_L:
    return registers.L;
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
  }
}

void instruction_load(instruction i) {
  if (i.address_mode == AM_R) {
    u8 to = (u8)get_register(i.reg_source);
    set_register(i.reg_dest, &to);
    registers.PC += 1;
    return;
  }

  u8 *to = ram_read(registers.PC + 1);
  set_register(i.reg_dest, to);
  registers.PC += 1;

  if (i.address_mode == AM_16)
    registers.PC += 1;
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
  u8 *ins = ram_read(registers.PC);
  instruction i = get_instruction(*ins);
  instruction_exec(i);
  registers.PC += 1;
}

void cpu_free() {}
