#include "../include/instructions.h"
#include <memory.h>

#define ZERO_FLAG 0b10000000
#define SUB_FLAG 0b01000000
#define HALF_CARRY_FLAG 0b00100000
#define CARRY_FLAG 0b00010000

u16 stack_pop() {
  u16 sp = get_reg16(R_SP);
  u16 val = ram_read16(sp);
  set_reg16(R_SP, sp + 2);
  return val;
};
void stack_push(u16 val) {
  u16 sp = get_reg16(R_SP);
  ram_write16(sp, val);
  set_reg16(R_SP, sp - 2);
};

void JP(u16 to) { set_reg16(R_PC, to); }
void POP(reg dest) { set_reg16(dest, stack_pop()); }
void PUSH(reg dest) { stack_push(get_reg16(dest)); }
void RET() { POP(R_PC); }
void CALL() {
  u16 dest = get_imm16();
  stack_push(get_reg16(R_PC));
  JP(dest);
}

void E8(reg dest) {
  u8 imm = get_imm8();
  int8_t e8 = (int8_t)imm;
  u16 sp = get_reg16(R_SP);
  u16 res = sp + e8;
  set_reg16(dest, res);
  set_flag(ZERO_FLAG, false);
  set_flag(SUB_FLAG, false);
  set_flag(HALF_CARRY_FLAG, (sp & 0xF) + (imm & 0xF) > 0xF);
  set_flag(CARRY_FLAG, (sp & 0xFF) + (imm & 0xFF) > 0xFF);
}

void XOR(reg dest, u8 val) {
  u8 a = get_reg8(dest);
  int res = a ^ val;
  set_reg8(dest, (u8)res);

  set_flag(ZERO_FLAG, (res & 0xFF) == 0);
  set_flag(SUB_FLAG, false);
  set_flag(HALF_CARRY_FLAG, false);
  set_flag(CARRY_FLAG, false);
}

void OR(reg dest, u8 val) {
  u8 a = get_reg8(dest);
  int res = a | val;
  set_reg8(dest, (u8)res);

  set_flag(ZERO_FLAG, (res & 0xFF) == 0);
  set_flag(SUB_FLAG, false);
  set_flag(HALF_CARRY_FLAG, false);
  set_flag(CARRY_FLAG, false);
}

void AND(reg dest, u8 val) {
  u8 a = get_reg8(dest);
  int res = a & val;
  set_reg8(dest, (u8)res);

  set_flag(ZERO_FLAG, (res & 0xFF) == 0);
  set_flag(SUB_FLAG, false);
  set_flag(HALF_CARRY_FLAG, true);
  set_flag(CARRY_FLAG, false);
}

void CP(reg dest, u8 val) {
  u8 a = get_reg8(dest);
  int res = a - val;

  set_flag(ZERO_FLAG, (res & 0xFF) == 0);
  set_flag(SUB_FLAG, true);
  set_flag(HALF_CARRY_FLAG, (a & 0xF) < (val & 0xF));
  set_flag(CARRY_FLAG, val > a);
}

void SBC(reg dest, u8 val) {
  u8 a = get_reg8(dest);
  int carry = get_flag(CARRY_FLAG) ? 1 : 0;
  int res = a - val - carry;
  set_reg8(dest, (u8)res);

  set_flag(ZERO_FLAG, (res & 0xFF) == 0);
  set_flag(SUB_FLAG, true);
  set_flag(HALF_CARRY_FLAG, ((a & 0xF) - (val & 0xF) - carry) < 0);
  set_flag(CARRY_FLAG, res < 0);
}

void SUB(reg dest, u8 val) {
  u8 a = get_reg8(dest);
  int res = a - val;
  set_reg8(dest, (u8)res);

  set_flag(ZERO_FLAG, (res & 0xFF) == 0);
  set_flag(SUB_FLAG, true);
  set_flag(HALF_CARRY_FLAG, (a & 0xF) < (val & 0xF));
  set_flag(CARRY_FLAG, val > a);
}

void ADC(reg dest, u8 val) {
  u8 a = get_reg8(dest);
  int carry = get_flag(CARRY_FLAG) ? 1 : 0;
  int res = a + val + carry;
  set_reg8(dest, (u8)res);

  set_flag(ZERO_FLAG, (res & 0xFF) == 0);
  set_flag(SUB_FLAG, false);
  set_flag(HALF_CARRY_FLAG, (a & 0xF) + (val & 0xF) > 0xF);
  set_flag(CARRY_FLAG, res > 0xFF);
}

void ADD16(reg dest, u16 val) {
  u16 a = get_reg16(dest);
  int res = a + val;
  set_reg16(dest, (u8)res);

  set_flag(SUB_FLAG, false);
  set_flag(HALF_CARRY_FLAG, (a & 0xFFF) + (val & 0xFFF) > 0xFFF);
  set_flag(CARRY_FLAG, res > 0xFFFF);
}

void ADD8(reg dest, u8 val) {
  u8 a = get_reg8(dest);
  int res = a + val;
  set_reg8(dest, (u8)res);

  set_flag(ZERO_FLAG, (res & 0xFF) == 0);
  set_flag(SUB_FLAG, false);
  set_flag(HALF_CARRY_FLAG, (a & 0xF) + (val & 0xF) > 0xF);
  set_flag(CARRY_FLAG, res > 0xFF);
}

void INC16(reg dest) {
  u16 a = get_reg16(dest);
  u16 res = a + 1;
  set_reg16(dest, res);
}

void INC8(reg dest) {
  u8 a = get_reg8(dest);
  u8 res = a + 1;
  set_reg8(dest, res);

  set_flag(ZERO_FLAG, (res & 0xFF) == 0);
  set_flag(SUB_FLAG, false);
  set_flag(HALF_CARRY_FLAG, (a & 0xF) + (1 & 0xF) > 0xF);
}

void DEC16(reg dest) {
  u16 a = get_reg16(dest);
  u16 res = a - 1;
  set_reg16(dest, res);
}

void DEC8(reg dest) {
  u8 a = get_reg8(dest);
  u8 res = a - 1;
  set_reg8(dest, res);

  set_flag(ZERO_FLAG, (res & 0xFF) == 0);
  set_flag(SUB_FLAG, true);
  set_flag(HALF_CARRY_FLAG, (a & 0xF) < (1 & 0xF));
}

#define OP_ENTRY(num) [0x##num] = op_##num

#define ROW_LOW(FN, MEM_FN, H, dest)                                           \
  FN(H##0, dest, R_B)                                                          \
  FN(H##1, dest, R_C)                                                          \
  FN(H##2, dest, R_D)                                                          \
  FN(H##3, dest, R_E)                                                          \
  FN(H##4, dest, R_H)                                                          \
  FN(H##5, dest, R_L)                                                          \
  MEM_FN(H##6, dest)                                                           \
  FN(H##7, dest, R_A)

#define ROW_HIGH(FN, MEM_FN, H, dest)                                          \
  FN(H##8, dest, R_B)                                                          \
  FN(H##9, dest, R_C)                                                          \
  FN(H##A, dest, R_D)                                                          \
  FN(H##B, dest, R_E)                                                          \
  FN(H##C, dest, R_H)                                                          \
  FN(H##D, dest, R_L)                                                          \
  MEM_FN(H##E, dest)                                                           \
  FN(H##F, dest, R_A)

#define ADD_HL_R16(num, src)                                                   \
  void op_##num() { ADD16(R_HL, get_reg16(src)); };

#define ADD_R8_R8(num, dest, src)                                              \
  void op_##num() { ADD8(dest, get_reg8(src)); };

#define ADD_R8_HL(num, dest)                                                   \
  void op_##num() { ADD8(dest, ram_read8(get_reg16(R_HL))); };

#define ADC_R8_R8(num, dest, src)                                              \
  void op_##num() { ADC(dest, get_reg8(src)); };

#define ADC_R8_HL(num, dest)                                                   \
  void op_##num() { ADC(dest, ram_read8(get_reg16(R_HL))); };

#define SUB_R8_R8(num, dest, src)                                              \
  void op_##num() { SUB(dest, get_reg8(src)); };

#define SUB_R8_HL(num, dest)                                                   \
  void op_##num() { SUB(dest, ram_read8(get_reg16(R_HL))); };

#define SBC_R8_R8(num, dest, src)                                              \
  void op_##num() { SBC(dest, get_reg8(src)); };

#define SBC_R8_HL(num, dest)                                                   \
  void op_##num() { SBC(dest, ram_read8(get_reg16(R_HL))); };

#define AND_R8_R8(num, dest, src)                                              \
  void op_##num() { AND(dest, get_reg8(src)); };

#define AND_R8_HL(num, dest)                                                   \
  void op_##num() { AND(dest, ram_read8(get_reg16(R_HL))); };

#define OR_R8_R8(num, dest, src)                                               \
  void op_##num() { OR(dest, get_reg8(src)); };

#define OR_R8_HL(num, dest)                                                    \
  void op_##num() { OR(dest, ram_read8(get_reg16(R_HL))); };

#define XOR_R8_R8(num, dest, src)                                              \
  void op_##num() { XOR(dest, get_reg8(src)); };

#define XOR_R8_HL(num, dest)                                                   \
  void op_##num() { XOR(dest, ram_read8(get_reg16(R_HL))); };

#define CP_R8_R8(num, dest, src)                                               \
  void op_##num() { CP(dest, get_reg8(src)); };

#define CP_R8_HL(num, dest)                                                    \
  void op_##num() { CP(dest, ram_read8(get_reg16(R_HL))); };

#define LD_R8_R8(num, dest, src)                                               \
  void op_##num() { set_reg8(dest, get_reg8(src)); };

#define LD_R8_HL(num, dest)                                                    \
  void op_##num() { set_reg8(dest, ram_read8(get_reg16(R_HL))); };

#define LD_HL_R8(num, src)                                                     \
  void op_##num() { ram_write8(get_reg16(R_HL), get_reg8(src)); };

#define LD_R16_N16(num, dest)                                                  \
  void op_##num() { set_reg16(dest, get_imm16()); };

#define LD_R16_A(num, src)                                                     \
  void op_##num() { ram_write8(get_reg16(src), get_reg8(R_A)); };

#define LD_A_R16(num, src)                                                     \
  void op_##num() { set_reg8(R_A, ram_read8(get_reg16(src))); }

#define LD_N16_A(num)                                                          \
  void op_##num() { ram_write8(get_imm16(), get_reg8(R_A)); };

#define LD_R8_N8(num, dest)                                                    \
  void op_##num() { set_reg8(dest, get_imm8()); };

#define LD_HL_N8(num)                                                          \
  void op_##num() { ram_write8(get_reg16(R_HL), get_imm8()); };

#define INC_R8(num, dest)                                                      \
  void op_##num() { INC8(dest); };

#define INC_R16(num, dest)                                                     \
  void op_##num() { INC16(dest); };

#define DEC_R8(num, dest)                                                      \
  void op_##num() { DEC8(dest); };

#define DEC_R16(num, dest)                                                     \
  void op_##num() { DEC16(dest); };

#define POP_R16(num, dest)                                                     \
  void op_##num() { POP(dest); };

#define PUSH_R16(num, dest)                                                    \
  void op_##num() { PUSH(dest); };

#define JP_CC(num, condition)                                                  \
  void op_##num() {                                                            \
    if (condition)                                                             \
      JP(get_imm16());                                                         \
  }

#define CALL_CC(num, condition)                                                \
  void op_##num() {                                                            \
    if (condition)                                                             \
      CALL();                                                                  \
  }

#define RET_CC(num, condition)                                                 \
  void op_##num() {                                                            \
    if (condition)                                                             \
      RET();                                                                   \
  };

void op_00() {}
LD_R16_N16(01, R_BC);
LD_R16_A(02, R_BC);
INC_R16(03, R_BC);
INC_R8(04, R_B);
DEC_R8(05, R_B);
LD_R8_N8(06, R_B);
void op_08() {
  u16 addr = get_imm16();
  u16 sp = get_reg16(R_SP);
  ram_write8(addr, sp & 0xFF);
  ram_write8(addr + 1, (sp >> 8));
}
ADD_HL_R16(09, R_BC);
LD_A_R16(0A, R_BC);
DEC_R16(0B, R_BC);
INC_R8(0C, R_C);
DEC_R8(0D, R_C);
LD_R8_N8(0E, R_C);

LD_R16_N16(11, R_DE);
LD_R16_A(12, R_DE);
INC_R16(13, R_DE);
INC_R8(14, R_D);
DEC_R8(15, R_D);
LD_R8_N8(16, R_D);
ADD_HL_R16(19, R_DE);
LD_A_R16(1A, R_DE);
DEC_R16(1B, R_DE);
INC_R8(1C, R_E);
DEC_R8(1D, R_E);
LD_R8_N8(1E, R_E);

LD_R16_N16(21, R_HL);
void op_22() {
  u16 hl = get_reg16(R_HL);
  ram_write8(hl, get_reg8(R_A));
  set_reg16(R_HL, hl + 1);
}
INC_R16(23, R_HL);
INC_R8(24, R_H);
DEC_R8(25, R_H);
LD_R8_N8(26, R_H);
ADD_HL_R16(29, R_HL);
void op_2A() {
  u16 hl = get_reg16(R_HL);
  u8 val = ram_read8(hl);
  set_reg16(R_HL, hl + 1);
  set_reg8(R_A, val);
}
DEC_R16(2B, R_HL);
INC_R8(2C, R_L);
DEC_R8(2D, R_L);
LD_R8_N8(2E, R_L);

LD_R16_N16(31, R_SP);
void op_32() {
  u16 hl = get_reg16(R_HL);
  ram_write8(hl, get_reg8(R_A));
  set_reg16(R_HL, hl - 1);
}
INC_R16(33, R_SP);
void op_34() {
  u16 hl = get_reg16(R_HL);
  u8 val = ram_read8(hl);
  u8 res = val + 1;
  ram_write8(hl, res);

  set_flag(ZERO_FLAG, (res & 0xFF) == 0);
  set_flag(SUB_FLAG, false);
  set_flag(HALF_CARRY_FLAG, (val & 0xF) + (1 & 0xF) > 0xF);
}
void op_35() {
  u16 hl = get_reg16(R_HL);
  u8 val = ram_read8(hl);
  u8 res = val - 1;
  ram_write8(hl, res);

  set_flag(ZERO_FLAG, (res & 0xFF) == 0);
  set_flag(SUB_FLAG, true);
  set_flag(HALF_CARRY_FLAG, (val & 0xF) < (1 & 0xF));
}
void op_36() { ram_write8(get_reg16(R_HL), get_imm8()); }
ADD_HL_R16(39, R_SP);
void op_3A() {
  u16 hl = get_reg16(R_HL);
  u8 val = ram_read8(hl);
  set_reg16(R_HL, hl - 1);
  set_reg8(R_A, val);
}
DEC_R16(3B, R_SP);
INC_R8(3C, R_A);
DEC_R8(3D, R_A);
LD_R8_N8(3E, R_A);

ROW_LOW(LD_R8_R8, LD_R8_HL, 4, R_B);
ROW_HIGH(LD_R8_R8, LD_R8_HL, 4, R_C);

ROW_LOW(LD_R8_R8, LD_R8_HL, 5, R_D);
ROW_HIGH(LD_R8_R8, LD_R8_HL, 5, R_E);

ROW_LOW(LD_R8_R8, LD_R8_HL, 6, R_H);
ROW_HIGH(LD_R8_R8, LD_R8_HL, 6, R_L);

LD_HL_R8(70, R_B);
LD_HL_R8(71, R_C)
LD_HL_R8(72, R_D);
LD_HL_R8(73, R_E)
LD_HL_R8(74, R_H);
LD_HL_R8(75, R_L)
void op_76() {
  // TODO: HALT
}
LD_HL_R8(77, R_A)
ROW_HIGH(LD_R8_R8, LD_R8_HL, 7, R_A);

ROW_LOW(ADD_R8_R8, ADD_R8_HL, 8, R_A);
ROW_HIGH(ADC_R8_R8, ADC_R8_HL, 8, R_A);

ROW_LOW(SUB_R8_R8, SUB_R8_HL, 9, R_A);
ROW_HIGH(SBC_R8_R8, SBC_R8_HL, 9, R_A);

ROW_LOW(AND_R8_R8, AND_R8_HL, A, R_A);
ROW_HIGH(XOR_R8_R8, XOR_R8_HL, A, R_A);

ROW_LOW(OR_R8_R8, OR_R8_HL, B, R_A);
ROW_HIGH(CP_R8_R8, CP_R8_HL, B, R_A);

RET_CC(C0, !get_flag(ZERO_FLAG));
POP_R16(C1, R_BC);
JP_CC(C2, !get_flag(ZERO_FLAG));
JP_CC(C3, true);
CALL_CC(C4, !get_flag(ZERO_FLAG));
PUSH_R16(C5, R_BC);
void op_C6() { ADD8(R_A, get_imm8()); };
RET_CC(C8, get_flag(ZERO_FLAG));
RET_CC(C9, true);
JP_CC(CA, get_flag(ZERO_FLAG));
CALL_CC(CC, get_flag(ZERO_FLAG));
CALL_CC(CD, true);
void op_CE() { ADC(R_A, get_imm8()); };

RET_CC(D0, !get_flag(CARRY_FLAG));
POP_R16(D1, R_DE);
JP_CC(D2, !get_flag(CARRY_FLAG));
CALL_CC(D4, !get_flag(CARRY_FLAG));
PUSH_R16(D5, R_DE);
void op_D6() { SUB(R_A, get_imm8()); };
RET_CC(D8, get_flag(CARRY_FLAG));
JP_CC(DA, get_flag(CARRY_FLAG));
CALL_CC(DC, get_flag(CARRY_FLAG));
void op_DE() { SBC(R_A, get_imm8()); };

void op_E0() { ram_write8(0xFF00 | get_imm8(), get_reg8(R_A)); }
POP_R16(E1, R_HL);
void op_E2() { ram_write8(0xFF00 | get_reg8(R_C), get_reg8(R_A)); }
PUSH_R16(E5, R_HL);
void op_E6() { AND(R_A, get_imm8()); };
void op_E8() { E8(R_SP); }
void op_E9() { JP(get_reg16(R_HL)); }
void op_EA() { ram_write8(get_imm16(), get_reg8(R_A)); }
void op_EE() { XOR(R_A, get_imm8()); };

void op_F0() { set_reg8(R_A, ram_read8(0xFF00 | get_imm8())); }
POP_R16(F1, R_AF);
void op_F2() { set_reg8(R_A, ram_read8(0xFF00 | get_reg8(R_C))); }
PUSH_R16(F5, R_AF);
void op_F6() { OR(R_A, get_imm8()); };
void op_F8() { E8(R_HL); }
void op_F9() { set_reg16(R_SP, get_reg16(R_HL)); }
void op_FA() { set_reg8(R_A, ram_read8(get_imm16())); }
void op_FE() { CP(R_A, get_imm8()); };

typedef void (*op_func)(void);

op_func instruction_table[] = {
    OP_ENTRY(00), OP_ENTRY(01), OP_ENTRY(02), OP_ENTRY(03),
    OP_ENTRY(04), OP_ENTRY(05), OP_ENTRY(06), OP_ENTRY(07),
    OP_ENTRY(08), OP_ENTRY(09), OP_ENTRY(0A), OP_ENTRY(0B),
    OP_ENTRY(0C), OP_ENTRY(0D), OP_ENTRY(0E), OP_ENTRY(0F),

    OP_ENTRY(10), OP_ENTRY(11), OP_ENTRY(12), OP_ENTRY(13),
    OP_ENTRY(14), OP_ENTRY(15), OP_ENTRY(16), OP_ENTRY(17),
    OP_ENTRY(18), OP_ENTRY(19), OP_ENTRY(1A), OP_ENTRY(1B),
    OP_ENTRY(1C), OP_ENTRY(1D), OP_ENTRY(1E), OP_ENTRY(1F),

    OP_ENTRY(20), OP_ENTRY(21), OP_ENTRY(22), OP_ENTRY(23),
    OP_ENTRY(24), OP_ENTRY(25), OP_ENTRY(26), OP_ENTRY(27),
    OP_ENTRY(28), OP_ENTRY(29), OP_ENTRY(2A), OP_ENTRY(2B),
    OP_ENTRY(2C), OP_ENTRY(2D), OP_ENTRY(2E), OP_ENTRY(2F),

    OP_ENTRY(30), OP_ENTRY(31), OP_ENTRY(32), OP_ENTRY(33),
    OP_ENTRY(34), OP_ENTRY(35), OP_ENTRY(36), OP_ENTRY(37),
    OP_ENTRY(38), OP_ENTRY(39), OP_ENTRY(3A), OP_ENTRY(3B),
    OP_ENTRY(3C), OP_ENTRY(3D), OP_ENTRY(3E), OP_ENTRY(3F),

    OP_ENTRY(40), OP_ENTRY(41), OP_ENTRY(42), OP_ENTRY(43),
    OP_ENTRY(44), OP_ENTRY(45), OP_ENTRY(46), OP_ENTRY(47),
    OP_ENTRY(48), OP_ENTRY(49), OP_ENTRY(4A), OP_ENTRY(4B),
    OP_ENTRY(4C), OP_ENTRY(4D), OP_ENTRY(4E), OP_ENTRY(4F),

    OP_ENTRY(50), OP_ENTRY(51), OP_ENTRY(52), OP_ENTRY(53),
    OP_ENTRY(54), OP_ENTRY(55), OP_ENTRY(56), OP_ENTRY(57),
    OP_ENTRY(58), OP_ENTRY(59), OP_ENTRY(5A), OP_ENTRY(5B),
    OP_ENTRY(5C), OP_ENTRY(5D), OP_ENTRY(5E), OP_ENTRY(5F),

    OP_ENTRY(60), OP_ENTRY(61), OP_ENTRY(62), OP_ENTRY(63),
    OP_ENTRY(64), OP_ENTRY(65), OP_ENTRY(66), OP_ENTRY(67),
    OP_ENTRY(68), OP_ENTRY(69), OP_ENTRY(6A), OP_ENTRY(6B),
    OP_ENTRY(6C), OP_ENTRY(6D), OP_ENTRY(6E), OP_ENTRY(6F),

    OP_ENTRY(70), OP_ENTRY(71), OP_ENTRY(72), OP_ENTRY(73),
    OP_ENTRY(74), OP_ENTRY(75), OP_ENTRY(76), OP_ENTRY(77),
    OP_ENTRY(78), OP_ENTRY(79), OP_ENTRY(7A), OP_ENTRY(7B),
    OP_ENTRY(7C), OP_ENTRY(7D), OP_ENTRY(7E), OP_ENTRY(7F),

    OP_ENTRY(80), OP_ENTRY(81), OP_ENTRY(82), OP_ENTRY(83),
    OP_ENTRY(84), OP_ENTRY(85), OP_ENTRY(86), OP_ENTRY(87),
    OP_ENTRY(88), OP_ENTRY(89), OP_ENTRY(8A), OP_ENTRY(8B),
    OP_ENTRY(8C), OP_ENTRY(8D), OP_ENTRY(8E), OP_ENTRY(8F),

    OP_ENTRY(90), OP_ENTRY(91), OP_ENTRY(92), OP_ENTRY(93),
    OP_ENTRY(94), OP_ENTRY(95), OP_ENTRY(96), OP_ENTRY(97),
    OP_ENTRY(98), OP_ENTRY(99), OP_ENTRY(9A), OP_ENTRY(9B),
    OP_ENTRY(9C), OP_ENTRY(9D), OP_ENTRY(9E), OP_ENTRY(9F),

    OP_ENTRY(A0), OP_ENTRY(A1), OP_ENTRY(A2), OP_ENTRY(A3),
    OP_ENTRY(A4), OP_ENTRY(A5), OP_ENTRY(A6), OP_ENTRY(A7),
    OP_ENTRY(A8), OP_ENTRY(A9), OP_ENTRY(AA), OP_ENTRY(AB),
    OP_ENTRY(AC), OP_ENTRY(AD), OP_ENTRY(AE), OP_ENTRY(AF),

    OP_ENTRY(B0), OP_ENTRY(B1), OP_ENTRY(B2), OP_ENTRY(B3),
    OP_ENTRY(B4), OP_ENTRY(B5), OP_ENTRY(B6), OP_ENTRY(B7),
    OP_ENTRY(B8), OP_ENTRY(B9), OP_ENTRY(BA), OP_ENTRY(BB),
    OP_ENTRY(BC), OP_ENTRY(BD), OP_ENTRY(BE), OP_ENTRY(BF),

    OP_ENTRY(C0), OP_ENTRY(C1), OP_ENTRY(C2), OP_ENTRY(C3),
    OP_ENTRY(C4), OP_ENTRY(C5), OP_ENTRY(C6), OP_ENTRY(C7),
    OP_ENTRY(C8), OP_ENTRY(C9), OP_ENTRY(CA), OP_ENTRY(CB),
    OP_ENTRY(CC), OP_ENTRY(CD), OP_ENTRY(CE), OP_ENTRY(CF),

    OP_ENTRY(D0), OP_ENTRY(D1), OP_ENTRY(D2), OP_ENTRY(D3),
    OP_ENTRY(D4), OP_ENTRY(D5), OP_ENTRY(D6), OP_ENTRY(D7),
    OP_ENTRY(D8), OP_ENTRY(D9), OP_ENTRY(DA), OP_ENTRY(DB),
    OP_ENTRY(DC), OP_ENTRY(DD), OP_ENTRY(DE), OP_ENTRY(DF),

    OP_ENTRY(E0), OP_ENTRY(E1), OP_ENTRY(E2), OP_ENTRY(E3),
    OP_ENTRY(E4), OP_ENTRY(E5), OP_ENTRY(E6), OP_ENTRY(E7),
    OP_ENTRY(E8), OP_ENTRY(E9), OP_ENTRY(EA), OP_ENTRY(EB),
    OP_ENTRY(EC), OP_ENTRY(ED), OP_ENTRY(EE), OP_ENTRY(EF),

    OP_ENTRY(F0), OP_ENTRY(F1), OP_ENTRY(F2), OP_ENTRY(F3),
    OP_ENTRY(F4), OP_ENTRY(F5), OP_ENTRY(F6), OP_ENTRY(F7),
    OP_ENTRY(F8), OP_ENTRY(F9), OP_ENTRY(FA), OP_ENTRY(FB),
    OP_ENTRY(FC), OP_ENTRY(FD), OP_ENTRY(FE), OP_ENTRY(FF),
};

void instruction_exec(u16 pc) {
  u8 bytes = ram_read8(pc);
  op_func instruction = instruction_table[bytes];
  cpu_advance();
  instruction();
}
