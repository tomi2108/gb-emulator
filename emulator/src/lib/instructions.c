#include "../include/instructions.h"

#define ZERO_FLAG 0b10000000
#define SUB_FLAG 0b01000000
#define HALF_CARRY_FLAG 0b00100000
#define CARRY_FLAG 0b00010000

void sub(reg dest, u8 val) {
  u8 a = get_reg8(dest);
  int res = a - val;
  set_flag(ZERO_FLAG, (res & 0xFF) == 0);
  set_flag(SUB_FLAG, true);
  set_flag(HALF_CARRY_FLAG, (a & 0xF) < (val & 0xF));
  set_flag(CARRY_FLAG, val > a);
  set_reg8(dest, (u8)res);
}

void add(reg dest, u8 val) {
  u8 a = get_reg8(dest);
  int res = a + val;
  set_flag(ZERO_FLAG, (res & 0xFF) == 0);
  set_flag(SUB_FLAG, false);
  set_flag(HALF_CARRY_FLAG, (a & 0xF) + (val & 0xF) > 0xF);
  set_flag(CARRY_FLAG, res > 0xFF);
  set_reg8(dest, (u8)res);
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

#define ADD_R8_R8(num, dest, src)                                              \
  void op_##num() { add(dest, get_reg8(src)); };

#define ADD_R8_HL(num, dest)                                                   \
  void op_##num() { add(dest, ram_read8(get_reg16(R_HL))); };

#define SUB_R8_R8(num, dest, src)                                              \
  void op_##num() { sub(dest, get_reg8(src)); };

#define SUB_R8_HL(num, dest)                                                   \
  void op_##num() { sub(dest, ram_read8(get_reg16(R_HL))); };

#define LD_R8_R8(num, dest, src)                                               \
  void op_##num() { set_reg8(dest, get_reg8(src)); };

#define LD_R8_HL(num, dest)                                                    \
  void op_##num() { set_reg8(dest, ram_read8(get_reg16(R_HL))); };

#define LD_HL_R8(num, src)                                                     \
  void op_##num() { ram_write8(get_reg16(R_HL), get_reg8(src)); };

void op_00() {}

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
ROW_HIGH(ADD_R8_R8, ADD_R8_HL, 8, R_A);

ROW_LOW(SUB_R8_R8, SUB_R8_HL, 9, R_A);

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
