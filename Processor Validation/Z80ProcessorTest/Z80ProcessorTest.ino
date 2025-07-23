#include <digitalWriteFast.h>
#include <Arduino.h>
#include <stdint.h>

//
//Define Pins
//

//Data Bus
const int D_0 = 28;
const int D_1 = 30;
const int D_2 = 24;
const int D_3 = 5;
const int D_4 = 6;
const int D_5 = 4;
const int D_6 = 3;
const int D_7 = 26;
const int DataBus[] = { D_0, D_1, D_2, D_3, D_4, D_5, D_6, D_7 };

//Address Bus
const int A_0 = 43;
const int A_1 = 41;
const int A_2 = 39;
const int A_3 = 37;
const int A_4 = 35;
const int A_5 = 33;
const int A_6 = 31;
const int A_7 = 29;
const int A_8 = 27;
const int A_9 = 25;
const int A_10 = 23;
const int A_11 = 12;
const int A_12 = 11;
const int A_13 = 10;
const int A_14 = 9;
const int A_15 = 8;
const int AddressBus[] = { A_0, A_1, A_2, A_3, A_4, A_5, A_6, A_7, A_8, A_9, A_10, A_11, A_12, A_13, A_14, A_15 };

//System Control
const int M1 = 49;
const int MREQ = 38;
const int IORQ = 40;
const int RD = 42;
const int WR = 44;
const int RFSH = 47;

//CPU Control
const int HALT = 36;
const int WAIT = 48;
const int INT = 32;
const int NMI = 34;
const int RESET = 51;
const int CLK = 7;

//CPU Bus Control
const int BUSRQ = 50;
const int BUSAK = 46;

//
//Define bitmasks
//

//Data Bus
const int D_0_Mask = 1;
const int D_1_Mask = 2;
const int D_2_Mask = 4;
const int D_3_Mask = 8;
const int D_4_Mask = 16;
const int D_5_Mask = 32;
const int D_6_Mask = 64;
const int D_7_Mask = 128;
const int DataBusMask[] = { D_0_Mask, D_1_Mask, D_2_Mask, D_3_Mask, D_4_Mask, D_5_Mask, D_6_Mask, D_7_Mask };

//Address Bus
const int A_0_Mask = 1;
const int A_1_Mask = 2;
const int A_2_Mask = 4;
const int A_3_Mask = 8;
const int A_4_Mask = 16;
const int A_5_Mask = 32;
const int A_6_Mask = 64;
const int A_7_Mask = 128;
const int A_8_Mask = 256;
const int A_9_Mask = 512;
const int A_10_Mask = 1024;
const int A_11_Mask = 2048;
const int A_12_Mask = 4096;
const int A_13_Mask = 8192;
const int A_14_Mask = 16384;
const int A_15_Mask = 32768;
const int AddressBusMask[] = { A_0_Mask, A_1_Mask, A_2_Mask, A_3_Mask, A_4_Mask, A_5_Mask, A_6_Mask, A_7_Mask, A_8_Mask, A_9_Mask, A_10_Mask, A_11_Mask, A_12_Mask, A_13_Mask, A_14_Mask, A_15_Mask };

//
//Define instructions

enum InstructionGroup {
  GROUP_16BIT_ARITHMETIC,
  GROUP_16BIT_LOAD,
  GROUP_8BIT_ARITHMETIC_LOGIC,
  GROUP_8BIT_LOAD,
  GROUP_BIT_MANIPULATION,
  GROUP_BLOCK_SEARCH,
  GROUP_BLOCK_TRANSFER,
  GROUP_EXCHANGES,
  GROUP_GENERAL_PURPOSE_AF_OPERATIONS,
  GROUP_INPUT,
  GROUP_JUMP_CALL_RETURN,
  GROUP_MISCELLANEOUS_CPU_CONTROL,
  GROUP_OUTPUT,
  GROUP_RESTART,
  GROUP_ROTATES_SHIFTS,
  GROUP_COUNT  // always last, not a real group
};

// Store group names in PROGMEM
const char group_16bit_arithmetic[] PROGMEM = "16-Bit Arithmetic";
const char group_16bit_load[] PROGMEM = "16-Bit Load";
const char group_8bit_arithmetic_logic[] PROGMEM = "8-Bit Arithmetic & Logic";
const char group_8bit_load[] PROGMEM = "8-Bit Load";
const char group_bit_manipulation[] PROGMEM = "Bit Manipulation";
const char group_block_search[] PROGMEM = "Block Search";
const char group_block_transfer[] PROGMEM = "Block Transfer";
const char group_exchanges[] PROGMEM = "Exchanges";
const char group_general_purpose_af_operations[] PROGMEM = "General Purpose AF Operations";
const char group_input[] PROGMEM = "Input";
const char group_jump_call_return[] PROGMEM = "Jump, Call, & Return";
const char group_miscellaneous_cpu_control[] PROGMEM = "Miscellaneous CPU Control";
const char group_output[] PROGMEM = "Output";
const char group_restart[] PROGMEM = "Restart";
const char group_rotates_shifts[] PROGMEM = "Rotates & Shifts";

const char* const InstructionGroupNames[] PROGMEM = {
  group_16bit_arithmetic,
  group_16bit_load,
  group_8bit_arithmetic_logic,
  group_8bit_load,
  group_bit_manipulation,
  group_block_search,
  group_block_transfer,
  group_exchanges,
  group_general_purpose_af_operations,
  group_input,
  group_jump_call_return,
  group_miscellaneous_cpu_control,
  group_output,
  group_restart,
  group_rotates_shifts
};

typedef bool (*InstructionTestFunction)();

struct instructionDefinitionType {
  uint8_t group;  
  const char* mnemonic;
  uint16_t opCode;
  const char* firstOperand;
  const char* secondOperand;
  uint16_t extendedOpCode;
  InstructionTestFunction testFunction;
};

//Load all mnemonics into PROGMEM
const char mnemonic_nop[] PROGMEM = "NOP";
const char mnemonic_ld_a_n[] PROGMEM = "LD A,n";
const char mnemonic_ld_b_n[] PROGMEM = "LD B,n";
const char mnemonic_ld_c_n[] PROGMEM = "LD C,n";
const char mnemonic_ld_d_n[] PROGMEM = "LD D,n";
const char mnemonic_ld_e_n[] PROGMEM = "LD E,n";
const char mnemonic_ld_h_n[] PROGMEM = "LD H,n";
const char mnemonic_ld_l_n[] PROGMEM = "LD L,n";
const char mnemonic_ld_ptr_hl_n[] PROGMEM = "LD (HL),n";
const char mnemonic_ld_a_a[] PROGMEM = "LD A,A";
const char mnemonic_ld_b_a[] PROGMEM = "LD B,A";
const char mnemonic_ld_c_a[] PROGMEM = "LD C,A";
const char mnemonic_ld_d_a[] PROGMEM = "LD D,A";
const char mnemonic_ld_e_a[] PROGMEM = "LD E,A";
const char mnemonic_ld_h_a[] PROGMEM = "LD H,A";
const char mnemonic_ld_l_a[] PROGMEM = "LD L,A";
const char mnemonic_ld_a_b[] PROGMEM = "LD A,B";
const char mnemonic_ld_b_b[] PROGMEM = "LD B,B";
const char mnemonic_ld_c_b[] PROGMEM = "LD C,B";
const char mnemonic_ld_d_b[] PROGMEM = "LD D,B";
const char mnemonic_ld_e_b[] PROGMEM = "LD E,B";
const char mnemonic_ld_h_b[] PROGMEM = "LD H,B";
const char mnemonic_ld_l_b[] PROGMEM = "LD L,B";
const char mnemonic_ld_a_c[] PROGMEM = "LD A,C";
const char mnemonic_ld_b_c[] PROGMEM = "LD B,C";
const char mnemonic_ld_c_c[] PROGMEM = "LD C,C";
const char mnemonic_ld_d_c[] PROGMEM = "LD D,C";
const char mnemonic_ld_e_c[] PROGMEM = "LD E,C";
const char mnemonic_ld_h_c[] PROGMEM = "LD H,C";
const char mnemonic_ld_l_c[] PROGMEM = "LD L,C";
const char mnemonic_ld_a_d[] PROGMEM = "LD A,D";
const char mnemonic_ld_b_d[] PROGMEM = "LD B,D";
const char mnemonic_ld_c_d[] PROGMEM = "LD C,D";
const char mnemonic_ld_d_d[] PROGMEM = "LD D,D";
const char mnemonic_ld_e_d[] PROGMEM = "LD E,D";
const char mnemonic_ld_h_d[] PROGMEM = "LD H,D";
const char mnemonic_ld_l_d[] PROGMEM = "LD L,D";
const char mnemonic_ld_a_e[] PROGMEM = "LD A,E";
const char mnemonic_ld_b_e[] PROGMEM = "LD B,E";
const char mnemonic_ld_c_e[] PROGMEM = "LD C,E";
const char mnemonic_ld_d_e[] PROGMEM = "LD D,E";
const char mnemonic_ld_e_e[] PROGMEM = "LD E,E";
const char mnemonic_ld_h_e[] PROGMEM = "LD H,E";
const char mnemonic_ld_l_e[] PROGMEM = "LD L,E";
const char mnemonic_ld_a_h[] PROGMEM = "LD A,H";
const char mnemonic_ld_b_h[] PROGMEM = "LD B,H";
const char mnemonic_ld_c_h[] PROGMEM = "LD C,H";
const char mnemonic_ld_d_h[] PROGMEM = "LD D,H";
const char mnemonic_ld_e_h[] PROGMEM = "LD E,H";
const char mnemonic_ld_h_h[] PROGMEM = "LD H,H";
const char mnemonic_ld_l_h[] PROGMEM = "LD L,H";
const char mnemonic_ld_a_l[] PROGMEM = "LD A,L";
const char mnemonic_ld_b_l[] PROGMEM = "LD B,L";
const char mnemonic_ld_c_l[] PROGMEM = "LD C,L";
const char mnemonic_ld_d_l[] PROGMEM = "LD D,L";
const char mnemonic_ld_e_l[] PROGMEM = "LD E,L";
const char mnemonic_ld_h_l[] PROGMEM = "LD H,L";
const char mnemonic_ld_l_l[] PROGMEM = "LD L,L";
const char mnemonic_ld_a_ptr_hl[] PROGMEM = "LD A,(HL)";
const char mnemonic_ld_b_ptr_hl[] PROGMEM = "LD B,(HL)";
const char mnemonic_ld_c_ptr_hl[] PROGMEM = "LD C,(HL)";
const char mnemonic_ld_d_ptr_hl[] PROGMEM = "LD D,(HL)";
const char mnemonic_ld_e_ptr_hl[] PROGMEM = "LD E,(HL)";
const char mnemonic_ld_h_ptr_hl[] PROGMEM = "LD H,(HL)";
const char mnemonic_ld_l_ptr_hl[] PROGMEM = "LD L,(HL)";
const char mnemonic_ld_ptr_hl_a[] PROGMEM = "LD (HL),A";
const char mnemonic_ld_ptr_hl_b[] PROGMEM = "LD (HL),B";
const char mnemonic_ld_ptr_hl_c[] PROGMEM = "LD (HL),C";
const char mnemonic_ld_ptr_hl_d[] PROGMEM = "LD (HL),D";
const char mnemonic_ld_ptr_hl_e[] PROGMEM = "LD (HL),E";
const char mnemonic_ld_ptr_hl_h[] PROGMEM = "LD (HL),H";
const char mnemonic_ld_ptr_hl_l[] PROGMEM = "LD (HL),L";
const char mnemonic_ld_a_ptr_bc[] PROGMEM = "LD A,(BC)";
const char mnemonic_ld_a_ptr_de[] PROGMEM = "LD A,(DE)";
const char mnemonic_ld_a_ptr_nn[] PROGMEM = "LD A,(nn)";
const char mnemonic_ld_ptr_nn_a[] PROGMEM = "LD (nn),A";
const char mnemonic_ld_ptr_bc_a[] PROGMEM = "LD (BC),A";
const char mnemonic_ld_ptr_de_a[] PROGMEM = "LD (DE),A";
const char mnemonic_ld_a_i[] PROGMEM = "LD A,I";
const char mnemonic_ld_a_r[] PROGMEM = "LD A,R";
const char mnemonic_ld_i_a[] PROGMEM = "LD I,A";
const char mnemonic_ld_r_a[] PROGMEM = "LD R,A";
const char mnemonic_ld_ptr_ix_plus_d_n[] PROGMEM = "LD (IX+d),n";
const char mnemonic_ld_ptr_iy_plus_d_n[] PROGMEM = "LD (IY+d),n";
const char mnemonic_ld_a_ptr_ix_plus_d[] PROGMEM = "LD A,(IX+d)";
const char mnemonic_ld_b_ptr_ix_plus_d[] PROGMEM = "LD B,(IX+d)";
const char mnemonic_ld_c_ptr_ix_plus_d[] PROGMEM = "LD C,(IX+d)";
const char mnemonic_ld_d_ptr_ix_plus_d[] PROGMEM = "LD D,(IX+d)";
const char mnemonic_ld_e_ptr_ix_plus_d[] PROGMEM = "LD E,(IX+d)";
const char mnemonic_ld_h_ptr_ix_plus_d[] PROGMEM = "LD H,(IX+d)";
const char mnemonic_ld_l_ptr_ix_plus_d[] PROGMEM = "LD L,(IX+d)";
const char mnemonic_ld_a_ptr_iy_plus_d[] PROGMEM = "LD A,(IY+d)";
const char mnemonic_ld_b_ptr_iy_plus_d[] PROGMEM = "LD B,(IY+d)";
const char mnemonic_ld_c_ptr_iy_plus_d[] PROGMEM = "LD C,(IY+d)";
const char mnemonic_ld_d_ptr_iy_plus_d[] PROGMEM = "LD D,(IY+d)";
const char mnemonic_ld_e_ptr_iy_plus_d[] PROGMEM = "LD E,(IY+d)";
const char mnemonic_ld_h_ptr_iy_plus_d[] PROGMEM = "LD H,(IY+d)";
const char mnemonic_ld_l_ptr_iy_plus_d[] PROGMEM = "LD L,(IY+d)";
const char mnemonic_ld_ptr_ix_plus_d_a[] PROGMEM = "LD (IX+d),A";
const char mnemonic_ld_ptr_ix_plus_d_b[] PROGMEM = "LD (IX+d),B";
const char mnemonic_ld_ptr_ix_plus_d_c[] PROGMEM = "LD (IX+d),C";
const char mnemonic_ld_ptr_ix_plus_d_d[] PROGMEM = "LD (IX+d),D";
const char mnemonic_ld_ptr_ix_plus_d_e[] PROGMEM = "LD (IX+d),E";
const char mnemonic_ld_ptr_ix_plus_d_h[] PROGMEM = "LD (IX+d),H";
const char mnemonic_ld_ptr_ix_plus_d_l[] PROGMEM = "LD (IX+d),L";
const char mnemonic_ld_ptr_iy_plus_d_a[] PROGMEM = "LD (IY+d),A";
const char mnemonic_ld_ptr_iy_plus_d_b[] PROGMEM = "LD (IY+d),B";
const char mnemonic_ld_ptr_iy_plus_d_c[] PROGMEM = "LD (IY+d),C";
const char mnemonic_ld_ptr_iy_plus_d_d[] PROGMEM = "LD (IY+d),D";
const char mnemonic_ld_ptr_iy_plus_d_e[] PROGMEM = "LD (IY+d),E";
const char mnemonic_ld_ptr_iy_plus_d_h[] PROGMEM = "LD (IY+d),H";
const char mnemonic_ld_ptr_iy_plus_d_l[] PROGMEM = "LD (IY+d),L";
const char mnemonic_ld_bc_nn[] PROGMEM = "LD BC,nn";
const char mnemonic_ld_de_nn[] PROGMEM = "LD DE,nn";
const char mnemonic_ld_hl_nn[] PROGMEM = "LD HL,nn";
const char mnemonic_ld_sp_nn[] PROGMEM = "LD SP,nn";
const char mnemonic_ld_ix_nn[] PROGMEM = "LD IX,nn";
const char mnemonic_ld_iy_nn[] PROGMEM = "LD IY,nn";
const char mnemonic_ld_bc_ptr_nn[] PROGMEM = "LD BC,(nn)";
const char mnemonic_ld_de_ptr_nn[] PROGMEM = "LD DE,(nn)";
const char mnemonic_ld_hl_ptr_nn[] PROGMEM = "LD HL,(nn)";
const char mnemonic_ld_sp_ptr_nn[] PROGMEM = "LD SP,(nn)";
const char mnemonic_ld_ix_ptr_nn[] PROGMEM = "LD IX,(nn)";
const char mnemonic_ld_iy_ptr_nn[] PROGMEM = "LD IY,(nn)";
const char mnemonic_ld_ptr_nn_bc[] PROGMEM = "LD (nn),BC";
const char mnemonic_ld_ptr_nn_de[] PROGMEM = "LD (nn),DE";
const char mnemonic_ld_ptr_nn_hl[] PROGMEM = "LD (nn),HL";
const char mnemonic_ld_ptr_nn_sp[] PROGMEM = "LD (nn),SP";
const char mnemonic_ld_ptr_nn_ix[] PROGMEM = "LD (nn),IX";
const char mnemonic_ld_ptr_nn_iy[] PROGMEM = "LD (nn),IY";
const char mnemonic_ld_sp_hl[] PROGMEM = "LD SP,HL";
const char mnemonic_ld_sp_ix[] PROGMEM = "LD SP,IX";
const char mnemonic_ld_sp_iy[] PROGMEM = "LD SP,IY";
const char mnemonic_pop_af[] PROGMEM = "POP AF";
const char mnemonic_pop_bc[] PROGMEM = "POP BC";
const char mnemonic_pop_de[] PROGMEM = "POP DE";
const char mnemonic_pop_hl[] PROGMEM = "POP HL";
const char mnemonic_pop_ix[] PROGMEM = "POP IX";
const char mnemonic_pop_iy[] PROGMEM = "POP IY";
const char mnemonic_push_af[] PROGMEM = "PUSH AF";
const char mnemonic_push_bc[] PROGMEM = "PUSH BC";
const char mnemonic_push_de[] PROGMEM = "PUSH DE";
const char mnemonic_push_hl[] PROGMEM = "PUSH HL";
const char mnemonic_push_ix[] PROGMEM = "PUSH IX";
const char mnemonic_push_iy[] PROGMEM = "PUSH IY";
const char mnemonic_ex_af_af_prime[] PROGMEM = "EX AF,AF'";
const char mnemonic_exx[] PROGMEM = "EXX";
const char mnemonic_ex_de_hl[] PROGMEM = "EX DE,HL";
const char mnemonic_ex_ptr_sp_hl[] PROGMEM = "EX (SP),HL";
const char mnemonic_ex_ptr_sp_ix[] PROGMEM = "EX (SP),IX";
const char mnemonic_ex_ptr_sp_iy[] PROGMEM = "EX (SP),IY";
const char mnemonic_ldi[] PROGMEM = "LDI";
const char mnemonic_ldir[] PROGMEM = "LDIR";
const char mnemonic_ldd[] PROGMEM = "LDD";
const char mnemonic_lddr[] PROGMEM = "LDDR";
const char mnemonic_cpi[] PROGMEM = "CPI";
const char mnemonic_cpir[] PROGMEM = "CPIR";
const char mnemonic_cpd[] PROGMEM = "CPD";
const char mnemonic_cpdr[] PROGMEM = "CPDR";
const char mnemonic_add_a_a[] PROGMEM = "ADD A,A";
const char mnemonic_add_a_b[] PROGMEM = "ADD A,B";
const char mnemonic_add_a_c[] PROGMEM = "ADD A,C";
const char mnemonic_add_a_d[] PROGMEM = "ADD A,D";
const char mnemonic_add_a_e[] PROGMEM = "ADD A,E";
const char mnemonic_add_a_h[] PROGMEM = "ADD A,H";
const char mnemonic_add_a_l[] PROGMEM = "ADD A,L";
const char mnemonic_add_a_ptr_hl[] PROGMEM = "ADD A,(HL)";
const char mnemonic_add_a_ptr_ix_plus_d[] PROGMEM = "ADD A,(IX+d)";
const char mnemonic_add_a_ptr_iy_plus_d[] PROGMEM = "ADD A,(IY+d)";
const char mnemonic_add_a_n[] PROGMEM = "ADD A,n";
const char mnemonic_adc_a_a[] PROGMEM = "ADC A,A";
const char mnemonic_adc_a_b[] PROGMEM = "ADC A,B";
const char mnemonic_adc_a_c[] PROGMEM = "ADC A,C";
const char mnemonic_adc_a_d[] PROGMEM = "ADC A,D";
const char mnemonic_adc_a_e[] PROGMEM = "ADC A,E";
const char mnemonic_adc_a_h[] PROGMEM = "ADC A,H";
const char mnemonic_adc_a_l[] PROGMEM = "ADC A,L";
const char mnemonic_adc_a_ptr_hl[] PROGMEM = "ADC A,(HL)";
const char mnemonic_adc_a_ptr_ix_plus_d[] PROGMEM = "ADC A,(IX+d)";
const char mnemonic_adc_a_ptr_iy_plus_d[] PROGMEM = "ADC A,(IY+d)";
const char mnemonic_adc_a_n[] PROGMEM = "ADC A,n";
const char mnemonic_sub_a_a[] PROGMEM = "SUB A,A";
const char mnemonic_sub_a_b[] PROGMEM = "SUB A,B";
const char mnemonic_sub_a_c[] PROGMEM = "SUB A,C";
const char mnemonic_sub_a_d[] PROGMEM = "SUB A,D";
const char mnemonic_sub_a_e[] PROGMEM = "SUB A,E";
const char mnemonic_sub_a_h[] PROGMEM = "SUB A,H";
const char mnemonic_sub_a_l[] PROGMEM = "SUB A,L";
const char mnemonic_sub_a_ptr_hl[] PROGMEM = "SUB A,(HL)";
const char mnemonic_sub_a_ptr_ix_plus_d[] PROGMEM = "SUB A,(IX+d)";
const char mnemonic_sub_a_ptr_iy_plus_d[] PROGMEM = "SUB A,(IY+d)";
const char mnemonic_sub_a_n[] PROGMEM = "SUB A,n";
const char mnemonic_sbc_a_a[] PROGMEM = "SBC A,A";
const char mnemonic_sbc_a_b[] PROGMEM = "SBC A,B";
const char mnemonic_sbc_a_c[] PROGMEM = "SBC A,C";
const char mnemonic_sbc_a_d[] PROGMEM = "SBC A,D";
const char mnemonic_sbc_a_e[] PROGMEM = "SBC A,E";
const char mnemonic_sbc_a_h[] PROGMEM = "SBC A,H";
const char mnemonic_sbc_a_l[] PROGMEM = "SBC A,L";
const char mnemonic_sbc_a_ptr_hl[] PROGMEM = "SBC A,(HL)";
const char mnemonic_sbc_a_ptr_ix_plus_d[] PROGMEM = "SBC A,(IX+d)";
const char mnemonic_sbc_a_ptr_iy_plus_d[] PROGMEM = "SBC A,(IY+d)";
const char mnemonic_sbc_a_n[] PROGMEM = "SBC A,n";
const char mnemonic_and_a[] PROGMEM = "AND A";
const char mnemonic_and_b[] PROGMEM = "AND B";
const char mnemonic_and_c[] PROGMEM = "AND C";
const char mnemonic_and_d[] PROGMEM = "AND D";
const char mnemonic_and_e[] PROGMEM = "AND E";
const char mnemonic_and_h[] PROGMEM = "AND H";
const char mnemonic_and_l[] PROGMEM = "AND L";
const char mnemonic_and_ptr_hl[] PROGMEM = "AND (HL)";
const char mnemonic_and_ptr_ix_plus_d[] PROGMEM = "AND (IX+d)";
const char mnemonic_and_ptr_iy_plus_d[] PROGMEM = "AND (IY+d)";
const char mnemonic_and_n[] PROGMEM = "AND n";
const char mnemonic_xor_a[] PROGMEM = "XOR A";
const char mnemonic_xor_b[] PROGMEM = "XOR B";
const char mnemonic_xor_c[] PROGMEM = "XOR C";
const char mnemonic_xor_d[] PROGMEM = "XOR D";
const char mnemonic_xor_e[] PROGMEM = "XOR E";
const char mnemonic_xor_h[] PROGMEM = "XOR H";
const char mnemonic_xor_l[] PROGMEM = "XOR L";
const char mnemonic_xor_ptr_hl[] PROGMEM = "XOR (HL)";
const char mnemonic_xor_ptr_ix_plus_d[] PROGMEM = "XOR (IX+d)";
const char mnemonic_xor_ptr_iy_plus_d[] PROGMEM = "XOR (IY+d)";
const char mnemonic_xor_n[] PROGMEM = "XOR n";
const char mnemonic_or_a[] PROGMEM = "OR A";
const char mnemonic_or_b[] PROGMEM = "OR B";
const char mnemonic_or_c[] PROGMEM = "OR C";
const char mnemonic_or_d[] PROGMEM = "OR D";
const char mnemonic_or_e[] PROGMEM = "OR E";
const char mnemonic_or_h[] PROGMEM = "OR H";
const char mnemonic_or_l[] PROGMEM = "OR L";
const char mnemonic_or_ptr_hl[] PROGMEM = "OR (HL)";
const char mnemonic_or_ptr_ix_plus_d[] PROGMEM = "OR (IX+d)";
const char mnemonic_or_ptr_iy_plus_d[] PROGMEM = "OR (IY+d)";
const char mnemonic_or_n[] PROGMEM = "OR n";
const char mnemonic_cp_a[] PROGMEM = "CP A";
const char mnemonic_cp_b[] PROGMEM = "CP B";
const char mnemonic_cp_c[] PROGMEM = "CP C";
const char mnemonic_cp_d[] PROGMEM = "CP D";
const char mnemonic_cp_e[] PROGMEM = "CP E";
const char mnemonic_cp_h[] PROGMEM = "CP H";
const char mnemonic_cp_l[] PROGMEM = "CP L";
const char mnemonic_cp_ptr_hl[] PROGMEM = "CP (HL)";
const char mnemonic_cp_ptr_ix_plus_d[] PROGMEM = "CP (IX+d)";
const char mnemonic_cp_ptr_iy_plus_d[] PROGMEM = "CP (IY+d)";
const char mnemonic_cp_n[] PROGMEM = "CP n";
const char mnemonic_inc_a[] PROGMEM = "INC A";
const char mnemonic_inc_b[] PROGMEM = "INC B";
const char mnemonic_inc_c[] PROGMEM = "INC C";
const char mnemonic_inc_d[] PROGMEM = "INC D";
const char mnemonic_inc_e[] PROGMEM = "INC E";
const char mnemonic_inc_h[] PROGMEM = "INC H";
const char mnemonic_inc_l[] PROGMEM = "INC L";
const char mnemonic_inc_ptr_hl[] PROGMEM = "INC (HL)";
const char mnemonic_inc_ptr_ix_plus_d[] PROGMEM = "INC (IX+d)";
const char mnemonic_inc_ptr_iy_plus_d[] PROGMEM = "INC (IY+d)";
const char mnemonic_dec_a[] PROGMEM = "DEC A";
const char mnemonic_dec_b[] PROGMEM = "DEC B";
const char mnemonic_dec_c[] PROGMEM = "DEC C";
const char mnemonic_dec_d[] PROGMEM = "DEC D";
const char mnemonic_dec_e[] PROGMEM = "DEC E";
const char mnemonic_dec_h[] PROGMEM = "DEC H";
const char mnemonic_dec_l[] PROGMEM = "DEC L";
const char mnemonic_dec_ptr_hl[] PROGMEM = "DEC (HL)";
const char mnemonic_dec_ptr_ix_plus_d[] PROGMEM = "DEC (IX+d)";
const char mnemonic_dec_ptr_iy_plus_d[] PROGMEM = "DEC (IY+d)";
const char mnemonic_daa[] PROGMEM = "DAA";
const char mnemonic_cpl[] PROGMEM = "CPL";
const char mnemonic_neg[] PROGMEM = "NEG";
const char mnemonic_ccf[] PROGMEM = "CCF";
const char mnemonic_scf[] PROGMEM = "SCF";
const char mnemonic_add_hl_bc[] PROGMEM = "ADD HL,BC";
const char mnemonic_add_hl_de[] PROGMEM = "ADD HL,DE";
const char mnemonic_add_hl_hl[] PROGMEM = "ADD HL,HL";
const char mnemonic_add_hl_sp[] PROGMEM = "ADD HL,SP";
const char mnemonic_add_ix_bc[] PROGMEM = "ADD IX,BC";
const char mnemonic_add_ix_de[] PROGMEM = "ADD IX,DE";
const char mnemonic_add_ix_sp[] PROGMEM = "ADD IX,SP";
const char mnemonic_add_ix_ix[] PROGMEM = "ADD IX,IX";
const char mnemonic_add_iy_bc[] PROGMEM = "ADD IY,BC";
const char mnemonic_add_iy_de[] PROGMEM = "ADD IY,DE";
const char mnemonic_add_iy_sp[] PROGMEM = "ADD IY,SP";
const char mnemonic_add_iy_iy[] PROGMEM = "ADD IY,IY";
const char mnemonic_adc_hl_bc[] PROGMEM = "ADC HL,BC";
const char mnemonic_adc_hl_de[] PROGMEM = "ADC HL,DE";
const char mnemonic_adc_hl_hl[] PROGMEM = "ADC HL,HL";
const char mnemonic_adc_hl_sp[] PROGMEM = "ADC HL,SP";
const char mnemonic_sbc_hl_bc[] PROGMEM = "SBC HL,BC";
const char mnemonic_sbc_hl_de[] PROGMEM = "SBC HL,DE";
const char mnemonic_sbc_hl_hl[] PROGMEM = "SBC HL,HL";
const char mnemonic_sbc_hl_sp[] PROGMEM = "SBC HL,SP";
const char mnemonic_inc_bc[] PROGMEM = "INC BC";
const char mnemonic_inc_de[] PROGMEM = "INC DE";
const char mnemonic_inc_hl[] PROGMEM = "INC HL";
const char mnemonic_inc_sp[] PROGMEM = "INC SP";
const char mnemonic_inc_ix[] PROGMEM = "INC IX";
const char mnemonic_inc_iy[] PROGMEM = "INC IY";
const char mnemonic_dec_bc[] PROGMEM = "DEC BC";
const char mnemonic_dec_de[] PROGMEM = "DEC DE";
const char mnemonic_dec_hl[] PROGMEM = "DEC HL";
const char mnemonic_dec_sp[] PROGMEM = "DEC SP";
const char mnemonic_dec_ix[] PROGMEM = "DEC IX";
const char mnemonic_dec_iy[] PROGMEM = "DEC IY";
const char mnemonic_rlc_a[] PROGMEM = "RLC A";
const char mnemonic_rlc_b[] PROGMEM = "RLC B";
const char mnemonic_rlc_c[] PROGMEM = "RLC C";
const char mnemonic_rlc_d[] PROGMEM = "RLC D";
const char mnemonic_rlc_e[] PROGMEM = "RLC E";
const char mnemonic_rlc_h[] PROGMEM = "RLC H";
const char mnemonic_rlc_l[] PROGMEM = "RLC L";
const char mnemonic_rlc_ptr_hl[] PROGMEM = "RLC (HL)";
const char mnemonic_rlc_ptr_ix_plus_d[] PROGMEM = "RLC (IX+d)";
const char mnemonic_rlc_ptr_iy_plus_d[] PROGMEM = "RLC (IY+d)";
const char mnemonic_rrc_a[] PROGMEM = "RRC A";
const char mnemonic_rrc_b[] PROGMEM = "RRC B";
const char mnemonic_rrc_c[] PROGMEM = "RRC C";
const char mnemonic_rrc_d[] PROGMEM = "RRC D";
const char mnemonic_rrc_e[] PROGMEM = "RRC E";
const char mnemonic_rrc_h[] PROGMEM = "RRC H";
const char mnemonic_rrc_l[] PROGMEM = "RRC L";
const char mnemonic_rrc_ptr_hl[] PROGMEM = "RRC (HL)";
const char mnemonic_rrc_ptr_ix_plus_d[] PROGMEM = "RRC (IX+d)";
const char mnemonic_rrc_ptr_iy_plus_d[] PROGMEM = "RRC (IY+d)";
const char mnemonic_rl_a[] PROGMEM = "RL A";
const char mnemonic_rl_b[] PROGMEM = "RL B";
const char mnemonic_rl_c[] PROGMEM = "RL C";
const char mnemonic_rl_d[] PROGMEM = "RL D";
const char mnemonic_rl_e[] PROGMEM = "RL E";
const char mnemonic_rl_h[] PROGMEM = "RL H";
const char mnemonic_rl_l[] PROGMEM = "RL L";
const char mnemonic_rl_ptr_hl[] PROGMEM = "RL (HL)";
const char mnemonic_rl_ptr_ix_plus_d[] PROGMEM = "RL (IX+d)";
const char mnemonic_rl_ptr_iy_plus_d[] PROGMEM = "RL (IY+d)";
const char mnemonic_rr_a[] PROGMEM = "RR A";
const char mnemonic_rr_b[] PROGMEM = "RR B";
const char mnemonic_rr_c[] PROGMEM = "RR C";
const char mnemonic_rr_d[] PROGMEM = "RR D";
const char mnemonic_rr_e[] PROGMEM = "RR E";
const char mnemonic_rr_h[] PROGMEM = "RR H";
const char mnemonic_rr_l[] PROGMEM = "RR L";
const char mnemonic_rr_ptr_hl[] PROGMEM = "RR (HL)";
const char mnemonic_rr_ptr_ix_plus_d[] PROGMEM = "RR (IX+d)";
const char mnemonic_rr_ptr_iy_plus_d[] PROGMEM = "RR (IY+d)";
const char mnemonic_sla_a[] PROGMEM = "SLA A";
const char mnemonic_sla_b[] PROGMEM = "SLA B";
const char mnemonic_sla_c[] PROGMEM = "SLA C";
const char mnemonic_sla_d[] PROGMEM = "SLA D";
const char mnemonic_sla_e[] PROGMEM = "SLA E";
const char mnemonic_sla_h[] PROGMEM = "SLA H";
const char mnemonic_sla_l[] PROGMEM = "SLA L";
const char mnemonic_sla_ptr_hl[] PROGMEM = "SLA (HL)";
const char mnemonic_sla_ptr_ix_plus_d[] PROGMEM = "SLA (IX+d)";
const char mnemonic_sla_ptr_iy_plus_d[] PROGMEM = "SLA (IY+d)";
const char mnemonic_sra_a[] PROGMEM = "SRA A";
const char mnemonic_sra_b[] PROGMEM = "SRA B";
const char mnemonic_sra_c[] PROGMEM = "SRA C";
const char mnemonic_sra_d[] PROGMEM = "SRA D";
const char mnemonic_sra_e[] PROGMEM = "SRA E";
const char mnemonic_sra_h[] PROGMEM = "SRA H";
const char mnemonic_sra_l[] PROGMEM = "SRA L";
const char mnemonic_sra_ptr_hl[] PROGMEM = "SRA (HL)";
const char mnemonic_sra_ptr_ix_plus_d[] PROGMEM = "SRA (IX+d)";
const char mnemonic_sra_ptr_iy_plus_d[] PROGMEM = "SRA (IY+d)";
const char mnemonic_sll_a[] PROGMEM = "SLL A";
const char mnemonic_sll_b[] PROGMEM = "SLL B";
const char mnemonic_sll_c[] PROGMEM = "SLL C";
const char mnemonic_sll_d[] PROGMEM = "SLL D";
const char mnemonic_sll_e[] PROGMEM = "SLL E";
const char mnemonic_sll_h[] PROGMEM = "SLL H";
const char mnemonic_sll_l[] PROGMEM = "SLL L";
const char mnemonic_sll_ptr_hl[] PROGMEM = "SLL (HL)";
const char mnemonic_sll_ptr_ix_plus_d[] PROGMEM = "SLL (IX+d)";
const char mnemonic_sll_ptr_iy_plus_d[] PROGMEM = "SLL (IY+d)";
const char mnemonic_srl_a[] PROGMEM = "SRL A";
const char mnemonic_srl_b[] PROGMEM = "SRL B";
const char mnemonic_srl_c[] PROGMEM = "SRL C";
const char mnemonic_srl_d[] PROGMEM = "SRL D";
const char mnemonic_srl_e[] PROGMEM = "SRL E";
const char mnemonic_srl_h[] PROGMEM = "SRL H";
const char mnemonic_srl_l[] PROGMEM = "SRL L";
const char mnemonic_srl_ptr_hl[] PROGMEM = "SRL (HL)";
const char mnemonic_srl_ptr_ix_plus_d[] PROGMEM = "SRL (IX+d)";
const char mnemonic_srl_ptr_iy_plus_d[] PROGMEM = "SRL (IY+d)";
const char mnemonic_rld[] PROGMEM = "RLD";
const char mnemonic_rrd[] PROGMEM = "RRD";
const char mnemonic_rlca[] PROGMEM = "RLCA";
const char mnemonic_rrca[] PROGMEM = "RRCA";
const char mnemonic_rla[] PROGMEM = "RLA";
const char mnemonic_rra[] PROGMEM = "RRA";
const char mnemonic_bit_0_a[] PROGMEM = "BIT 0,A";
const char mnemonic_bit_0_b[] PROGMEM = "BIT 0,B";
const char mnemonic_bit_0_c[] PROGMEM = "BIT 0,C";
const char mnemonic_bit_0_d[] PROGMEM = "BIT 0,D";
const char mnemonic_bit_0_e[] PROGMEM = "BIT 0,E";
const char mnemonic_bit_0_h[] PROGMEM = "BIT 0,H";
const char mnemonic_bit_0_l[] PROGMEM = "BIT 0,L";
const char mnemonic_bit_0_ptr_hl[] PROGMEM = "BIT 0,(HL)";
const char mnemonic_bit_0_ptr_ix_plus_d[] PROGMEM = "BIT 0,(IX+d)";
const char mnemonic_bit_0_ptr_iy_plus_d[] PROGMEM = "BIT 0,(IY+d)";
const char mnemonic_bit_1_a[] PROGMEM = "BIT 1,A";
const char mnemonic_bit_1_b[] PROGMEM = "BIT 1,B";
const char mnemonic_bit_1_c[] PROGMEM = "BIT 1,C";
const char mnemonic_bit_1_d[] PROGMEM = "BIT 1,D";
const char mnemonic_bit_1_e[] PROGMEM = "BIT 1,E";
const char mnemonic_bit_1_h[] PROGMEM = "BIT 1,H";
const char mnemonic_bit_1_l[] PROGMEM = "BIT 1,L";
const char mnemonic_bit_1_ptr_hl[] PROGMEM = "BIT 1,(HL)";
const char mnemonic_bit_1_ptr_ix_plus_d[] PROGMEM = "BIT 1,(IX+d)";
const char mnemonic_bit_1_ptr_iy_plus_d[] PROGMEM = "BIT 1,(IY+d)";
const char mnemonic_bit_2_a[] PROGMEM = "BIT 2,A";
const char mnemonic_bit_2_b[] PROGMEM = "BIT 2,B";
const char mnemonic_bit_2_c[] PROGMEM = "BIT 2,C";
const char mnemonic_bit_2_d[] PROGMEM = "BIT 2,D";
const char mnemonic_bit_2_e[] PROGMEM = "BIT 2,E";
const char mnemonic_bit_2_h[] PROGMEM = "BIT 2,H";
const char mnemonic_bit_2_l[] PROGMEM = "BIT 2,L";
const char mnemonic_bit_2_ptr_hl[] PROGMEM = "BIT 2,(HL)";
const char mnemonic_bit_2_ptr_ix_plus_d[] PROGMEM = "BIT 2,(IX+d)";
const char mnemonic_bit_2_ptr_iy_plus_d[] PROGMEM = "BIT 2,(IY+d)";
const char mnemonic_bit_3_a[] PROGMEM = "BIT 3,A";
const char mnemonic_bit_3_b[] PROGMEM = "BIT 3,B";
const char mnemonic_bit_3_c[] PROGMEM = "BIT 3,C";
const char mnemonic_bit_3_d[] PROGMEM = "BIT 3,D";
const char mnemonic_bit_3_e[] PROGMEM = "BIT 3,E";
const char mnemonic_bit_3_h[] PROGMEM = "BIT 3,H";
const char mnemonic_bit_3_l[] PROGMEM = "BIT 3,L";
const char mnemonic_bit_3_ptr_hl[] PROGMEM = "BIT 3,(HL)";
const char mnemonic_bit_3_ptr_ix_plus_d[] PROGMEM = "BIT 3,(IX+d)";
const char mnemonic_bit_3_ptr_iy_plus_d[] PROGMEM = "BIT 3,(IY+d)";
const char mnemonic_bit_4_a[] PROGMEM = "BIT 4,A";
const char mnemonic_bit_4_b[] PROGMEM = "BIT 4,B";
const char mnemonic_bit_4_c[] PROGMEM = "BIT 4,C";
const char mnemonic_bit_4_d[] PROGMEM = "BIT 4,D";
const char mnemonic_bit_4_e[] PROGMEM = "BIT 4,E";
const char mnemonic_bit_4_h[] PROGMEM = "BIT 4,H";
const char mnemonic_bit_4_l[] PROGMEM = "BIT 4,L";
const char mnemonic_bit_4_ptr_hl[] PROGMEM = "BIT 4,(HL)";
const char mnemonic_bit_4_ptr_ix_plus_d[] PROGMEM = "BIT 4,(IX+d)";
const char mnemonic_bit_4_ptr_iy_plus_d[] PROGMEM = "BIT 4,(IY+d)";
const char mnemonic_bit_5_a[] PROGMEM = "BIT 5,A";
const char mnemonic_bit_5_b[] PROGMEM = "BIT 5,B";
const char mnemonic_bit_5_c[] PROGMEM = "BIT 5,C";
const char mnemonic_bit_5_d[] PROGMEM = "BIT 5,D";
const char mnemonic_bit_5_e[] PROGMEM = "BIT 5,E";
const char mnemonic_bit_5_h[] PROGMEM = "BIT 5,H";
const char mnemonic_bit_5_l[] PROGMEM = "BIT 5,L";
const char mnemonic_bit_5_ptr_hl[] PROGMEM = "BIT 5,(HL)";
const char mnemonic_bit_5_ptr_ix_plus_d[] PROGMEM = "BIT 5,(IX+d)";
const char mnemonic_bit_5_ptr_iy_plus_d[] PROGMEM = "BIT 5,(IY+d)";
const char mnemonic_bit_6_a[] PROGMEM = "BIT 6,A";
const char mnemonic_bit_6_b[] PROGMEM = "BIT 6,B";
const char mnemonic_bit_6_c[] PROGMEM = "BIT 6,C";
const char mnemonic_bit_6_d[] PROGMEM = "BIT 6,D";
const char mnemonic_bit_6_e[] PROGMEM = "BIT 6,E";
const char mnemonic_bit_6_h[] PROGMEM = "BIT 6,H";
const char mnemonic_bit_6_l[] PROGMEM = "BIT 6,L";
const char mnemonic_bit_6_ptr_hl[] PROGMEM = "BIT 6,(HL)";
const char mnemonic_bit_6_ptr_ix_plus_d[] PROGMEM = "BIT 6,(IX+d)";
const char mnemonic_bit_6_ptr_iy_plus_d[] PROGMEM = "BIT 6,(IY+d)";
const char mnemonic_bit_7_a[] PROGMEM = "BIT 7,A";
const char mnemonic_bit_7_b[] PROGMEM = "BIT 7,B";
const char mnemonic_bit_7_c[] PROGMEM = "BIT 7,C";
const char mnemonic_bit_7_d[] PROGMEM = "BIT 7,D";
const char mnemonic_bit_7_e[] PROGMEM = "BIT 7,E";
const char mnemonic_bit_7_h[] PROGMEM = "BIT 7,H";
const char mnemonic_bit_7_l[] PROGMEM = "BIT 7,L";
const char mnemonic_bit_7_ptr_hl[] PROGMEM = "BIT 7,(HL)";
const char mnemonic_bit_7_ptr_ix_plus_d[] PROGMEM = "BIT 7,(IX+d)";
const char mnemonic_bit_7_ptr_iy_plus_d[] PROGMEM = "BIT 7,(IY+d)";
const char mnemonic_res_0_a[] PROGMEM = "RES 0,A";
const char mnemonic_res_0_b[] PROGMEM = "RES 0,B";
const char mnemonic_res_0_c[] PROGMEM = "RES 0,C";
const char mnemonic_res_0_d[] PROGMEM = "RES 0,D";
const char mnemonic_res_0_e[] PROGMEM = "RES 0,E";
const char mnemonic_res_0_h[] PROGMEM = "RES 0,H";
const char mnemonic_res_0_l[] PROGMEM = "RES 0,L";
const char mnemonic_res_0_ptr_hl[] PROGMEM = "RES 0,(HL)";
const char mnemonic_res_0_ptr_ix_plus_d[] PROGMEM = "RES 0,(IX+d)";
const char mnemonic_res_0_ptr_iy_plus_d[] PROGMEM = "RES 0,(IY+d)";
const char mnemonic_res_1_a[] PROGMEM = "RES 1,A";
const char mnemonic_res_1_b[] PROGMEM = "RES 1,B";
const char mnemonic_res_1_c[] PROGMEM = "RES 1,C";
const char mnemonic_res_1_d[] PROGMEM = "RES 1,D";
const char mnemonic_res_1_e[] PROGMEM = "RES 1,E";
const char mnemonic_res_1_h[] PROGMEM = "RES 1,H";
const char mnemonic_res_1_l[] PROGMEM = "RES 1,L";
const char mnemonic_res_1_ptr_hl[] PROGMEM = "RES 1,(HL)";
const char mnemonic_res_1_ptr_ix_plus_d[] PROGMEM = "RES 1,(IX+d)";
const char mnemonic_res_1_ptr_iy_plus_d[] PROGMEM = "RES 1,(IY+d)";
const char mnemonic_res_2_a[] PROGMEM = "RES 2,A";
const char mnemonic_res_2_b[] PROGMEM = "RES 2,B";
const char mnemonic_res_2_c[] PROGMEM = "RES 2,C";
const char mnemonic_res_2_d[] PROGMEM = "RES 2,D";
const char mnemonic_res_2_e[] PROGMEM = "RES 2,E";
const char mnemonic_res_2_h[] PROGMEM = "RES 2,H";
const char mnemonic_res_2_l[] PROGMEM = "RES 2,L";
const char mnemonic_res_2_ptr_hl[] PROGMEM = "RES 2,(HL)";
const char mnemonic_res_2_ptr_ix_plus_d[] PROGMEM = "RES 2,(IX+d)";
const char mnemonic_res_2_ptr_iy_plus_d[] PROGMEM = "RES 2,(IY+d)";
const char mnemonic_res_3_a[] PROGMEM = "RES 3,A";
const char mnemonic_res_3_b[] PROGMEM = "RES 3,B";
const char mnemonic_res_3_c[] PROGMEM = "RES 3,C";
const char mnemonic_res_3_d[] PROGMEM = "RES 3,D";
const char mnemonic_res_3_e[] PROGMEM = "RES 3,E";
const char mnemonic_res_3_h[] PROGMEM = "RES 3,H";
const char mnemonic_res_3_l[] PROGMEM = "RES 3,L";
const char mnemonic_res_3_ptr_hl[] PROGMEM = "RES 3,(HL)";
const char mnemonic_res_3_ptr_ix_plus_d[] PROGMEM = "RES 3,(IX+d)";
const char mnemonic_res_3_ptr_iy_plus_d[] PROGMEM = "RES 3,(IY+d)";
const char mnemonic_res_4_a[] PROGMEM = "RES 4,A";
const char mnemonic_res_4_b[] PROGMEM = "RES 4,B";
const char mnemonic_res_4_c[] PROGMEM = "RES 4,C";
const char mnemonic_res_4_d[] PROGMEM = "RES 4,D";
const char mnemonic_res_4_e[] PROGMEM = "RES 4,E";
const char mnemonic_res_4_h[] PROGMEM = "RES 4,H";
const char mnemonic_res_4_l[] PROGMEM = "RES 4,L";
const char mnemonic_res_4_ptr_hl[] PROGMEM = "RES 4,(HL)";
const char mnemonic_res_4_ptr_ix_plus_d[] PROGMEM = "RES 4,(IX+d)";
const char mnemonic_res_4_ptr_iy_plus_d[] PROGMEM = "RES 4,(IY+d)";
const char mnemonic_res_5_a[] PROGMEM = "RES 5,A";
const char mnemonic_res_5_b[] PROGMEM = "RES 5,B";
const char mnemonic_res_5_c[] PROGMEM = "RES 5,C";
const char mnemonic_res_5_d[] PROGMEM = "RES 5,D";
const char mnemonic_res_5_e[] PROGMEM = "RES 5,E";
const char mnemonic_res_5_h[] PROGMEM = "RES 5,H";
const char mnemonic_res_5_l[] PROGMEM = "RES 5,L";
const char mnemonic_res_5_ptr_hl[] PROGMEM = "RES 5,(HL)";
const char mnemonic_res_5_ptr_ix_plus_d[] PROGMEM = "RES 5,(IX+d)";
const char mnemonic_res_5_ptr_iy_plus_d[] PROGMEM = "RES 5,(IY+d)";
const char mnemonic_res_6_a[] PROGMEM = "RES 6,A";
const char mnemonic_res_6_b[] PROGMEM = "RES 6,B";
const char mnemonic_res_6_c[] PROGMEM = "RES 6,C";
const char mnemonic_res_6_d[] PROGMEM = "RES 6,D";
const char mnemonic_res_6_e[] PROGMEM = "RES 6,E";
const char mnemonic_res_6_h[] PROGMEM = "RES 6,H";
const char mnemonic_res_6_l[] PROGMEM = "RES 6,L";
const char mnemonic_res_6_ptr_hl[] PROGMEM = "RES 6,(HL)";
const char mnemonic_res_6_ptr_ix_plus_d[] PROGMEM = "RES 6,(IX+d)";
const char mnemonic_res_6_ptr_iy_plus_d[] PROGMEM = "RES 6,(IY+d)";
const char mnemonic_res_7_a[] PROGMEM = "RES 7,A";
const char mnemonic_res_7_b[] PROGMEM = "RES 7,B";
const char mnemonic_res_7_c[] PROGMEM = "RES 7,C";
const char mnemonic_res_7_d[] PROGMEM = "RES 7,D";
const char mnemonic_res_7_e[] PROGMEM = "RES 7,E";
const char mnemonic_res_7_h[] PROGMEM = "RES 7,H";
const char mnemonic_res_7_l[] PROGMEM = "RES 7,L";
const char mnemonic_res_7_ptr_hl[] PROGMEM = "RES 7,(HL)";
const char mnemonic_res_7_ptr_ix_plus_d[] PROGMEM = "RES 7,(IX+d)";
const char mnemonic_res_7_ptr_iy_plus_d[] PROGMEM = "RES 7,(IY+d)";
const char mnemonic_set_0_a[] PROGMEM = "SET 0,A";
const char mnemonic_set_0_b[] PROGMEM = "SET 0,B";
const char mnemonic_set_0_c[] PROGMEM = "SET 0,C";
const char mnemonic_set_0_d[] PROGMEM = "SET 0,D";
const char mnemonic_set_0_e[] PROGMEM = "SET 0,E";
const char mnemonic_set_0_h[] PROGMEM = "SET 0,H";
const char mnemonic_set_0_l[] PROGMEM = "SET 0,L";
const char mnemonic_set_0_ptr_hl[] PROGMEM = "SET 0,(HL)";
const char mnemonic_set_0_ptr_ix_plus_d[] PROGMEM = "SET 0,(IX+d)";
const char mnemonic_set_0_ptr_iy_plus_d[] PROGMEM = "SET 0,(IY+d)";
const char mnemonic_set_1_a[] PROGMEM = "SET 1,A";
const char mnemonic_set_1_b[] PROGMEM = "SET 1,B";
const char mnemonic_set_1_c[] PROGMEM = "SET 1,C";
const char mnemonic_set_1_d[] PROGMEM = "SET 1,D";
const char mnemonic_set_1_e[] PROGMEM = "SET 1,E";
const char mnemonic_set_1_h[] PROGMEM = "SET 1,H";
const char mnemonic_set_1_l[] PROGMEM = "SET 1,L";
const char mnemonic_set_1_ptr_hl[] PROGMEM = "SET 1,(HL)";
const char mnemonic_set_1_ptr_ix_plus_d[] PROGMEM = "SET 1,(IX+d)";
const char mnemonic_set_1_ptr_iy_plus_d[] PROGMEM = "SET 1,(IY+d)";
const char mnemonic_set_2_a[] PROGMEM = "SET 2,A";
const char mnemonic_set_2_b[] PROGMEM = "SET 2,B";
const char mnemonic_set_2_c[] PROGMEM = "SET 2,C";
const char mnemonic_set_2_d[] PROGMEM = "SET 2,D";
const char mnemonic_set_2_e[] PROGMEM = "SET 2,E";
const char mnemonic_set_2_h[] PROGMEM = "SET 2,H";
const char mnemonic_set_2_l[] PROGMEM = "SET 2,L";
const char mnemonic_set_2_ptr_hl[] PROGMEM = "SET 2,(HL)";
const char mnemonic_set_2_ptr_ix_plus_d[] PROGMEM = "SET 2,(IX+d)";
const char mnemonic_set_2_ptr_iy_plus_d[] PROGMEM = "SET 2,(IY+d)";
const char mnemonic_set_3_a[] PROGMEM = "SET 3,A";
const char mnemonic_set_3_b[] PROGMEM = "SET 3,B";
const char mnemonic_set_3_c[] PROGMEM = "SET 3,C";
const char mnemonic_set_3_d[] PROGMEM = "SET 3,D";
const char mnemonic_set_3_e[] PROGMEM = "SET 3,E";
const char mnemonic_set_3_h[] PROGMEM = "SET 3,H";
const char mnemonic_set_3_l[] PROGMEM = "SET 3,L";
const char mnemonic_set_3_ptr_hl[] PROGMEM = "SET 3,(HL)";
const char mnemonic_set_3_ptr_ix_plus_d[] PROGMEM = "SET 3,(IX+d)";
const char mnemonic_set_3_ptr_iy_plus_d[] PROGMEM = "SET 3,(IY+d)";
const char mnemonic_set_4_a[] PROGMEM = "SET 4,A";
const char mnemonic_set_4_b[] PROGMEM = "SET 4,B";
const char mnemonic_set_4_c[] PROGMEM = "SET 4,C";
const char mnemonic_set_4_d[] PROGMEM = "SET 4,D";
const char mnemonic_set_4_e[] PROGMEM = "SET 4,E";
const char mnemonic_set_4_h[] PROGMEM = "SET 4,H";
const char mnemonic_set_4_l[] PROGMEM = "SET 4,L";
const char mnemonic_set_4_ptr_hl[] PROGMEM = "SET 4,(HL)";
const char mnemonic_set_4_ptr_ix_plus_d[] PROGMEM = "SET 4,(IX+d)";
const char mnemonic_set_4_ptr_iy_plus_d[] PROGMEM = "SET 4,(IY+d)";
const char mnemonic_set_5_a[] PROGMEM = "SET 5,A";
const char mnemonic_set_5_b[] PROGMEM = "SET 5,B";
const char mnemonic_set_5_c[] PROGMEM = "SET 5,C";
const char mnemonic_set_5_d[] PROGMEM = "SET 5,D";
const char mnemonic_set_5_e[] PROGMEM = "SET 5,E";
const char mnemonic_set_5_h[] PROGMEM = "SET 5,H";
const char mnemonic_set_5_l[] PROGMEM = "SET 5,L";
const char mnemonic_set_5_ptr_hl[] PROGMEM = "SET 5,(HL)";
const char mnemonic_set_5_ptr_ix_plus_d[] PROGMEM = "SET 5,(IX+d)";
const char mnemonic_set_5_ptr_iy_plus_d[] PROGMEM = "SET 5,(IY+d)";
const char mnemonic_set_6_a[] PROGMEM = "SET 6,A";
const char mnemonic_set_6_b[] PROGMEM = "SET 6,B";
const char mnemonic_set_6_c[] PROGMEM = "SET 6,C";
const char mnemonic_set_6_d[] PROGMEM = "SET 6,D";
const char mnemonic_set_6_e[] PROGMEM = "SET 6,E";
const char mnemonic_set_6_h[] PROGMEM = "SET 6,H";
const char mnemonic_set_6_l[] PROGMEM = "SET 6,L";
const char mnemonic_set_6_ptr_hl[] PROGMEM = "SET 6,(HL)";
const char mnemonic_set_6_ptr_ix_plus_d[] PROGMEM = "SET 6,(IX+d)";
const char mnemonic_set_6_ptr_iy_plus_d[] PROGMEM = "SET 6,(IY+d)";
const char mnemonic_set_7_a[] PROGMEM = "SET 7,A";
const char mnemonic_set_7_b[] PROGMEM = "SET 7,B";
const char mnemonic_set_7_c[] PROGMEM = "SET 7,C";
const char mnemonic_set_7_d[] PROGMEM = "SET 7,D";
const char mnemonic_set_7_e[] PROGMEM = "SET 7,E";
const char mnemonic_set_7_h[] PROGMEM = "SET 7,H";
const char mnemonic_set_7_l[] PROGMEM = "SET 7,L";
const char mnemonic_set_7_ptr_hl[] PROGMEM = "SET 7,(HL)";
const char mnemonic_set_7_ptr_ix_plus_d[] PROGMEM = "SET 7,(IX+d)";
const char mnemonic_set_7_ptr_iy_plus_d[] PROGMEM = "SET 7,(IY+d)";
const char mnemonic_jp_nn[] PROGMEM = "JP nn";
const char mnemonic_jp_c_nn[] PROGMEM = "JP C,nn";
const char mnemonic_jp_nc_nn[] PROGMEM = "JP NC,nn";
const char mnemonic_jp_z_nn[] PROGMEM = "JP Z,nn";
const char mnemonic_jp_nz_nn[] PROGMEM = "JP NZ,nn";
const char mnemonic_jp_pe_nn[] PROGMEM = "JP PE,nn";
const char mnemonic_jp_po_nn[] PROGMEM = "JP PO,nn";
const char mnemonic_jp_m_nn[] PROGMEM = "JP M,nn";
const char mnemonic_jp_p_nn[] PROGMEM = "JP P,nn";
const char mnemonic_jr_e[] PROGMEM = "JR e";
const char mnemonic_jr_c_e[] PROGMEM = "JR C,e";
const char mnemonic_jr_nc_e[] PROGMEM = "JR NC,e";
const char mnemonic_jr_z_e[] PROGMEM = "JR Z,e";
const char mnemonic_jr_nz_e[] PROGMEM = "JR NZ,e";
const char mnemonic_jp_ptr_hl[] PROGMEM = "JP (HL)";
const char mnemonic_jp_ptr_ix[] PROGMEM = "JP (IX)";
const char mnemonic_jp_ptr_iy[] PROGMEM = "JP (IY)";
const char mnemonic_call_nn[] PROGMEM = "CALL nn";
const char mnemonic_call_c_nn[] PROGMEM = "CALL C nn";
const char mnemonic_call_nc_nn[] PROGMEM = "CALL NC nn";
const char mnemonic_call_z_nn[] PROGMEM = "CALL Z nn";
const char mnemonic_call_nz_nn[] PROGMEM = "CALL NZ nn";
const char mnemonic_call_pe_nn[] PROGMEM = "CALL PE nn";
const char mnemonic_call_po_nn[] PROGMEM = "CALL PO nn";
const char mnemonic_call_m_nn[] PROGMEM = "CALL M nn";
const char mnemonic_call_p_nn[] PROGMEM = "CALL P nn";
const char mnemonic_djnz_e[] PROGMEM = "DJNZ e";
const char mnemonic_ret[] PROGMEM = "RET";
const char mnemonic_ret_c[] PROGMEM = "RET C";
const char mnemonic_ret_nc[] PROGMEM = "RET NC";
const char mnemonic_ret_z[] PROGMEM = "RET Z";
const char mnemonic_ret_nz[] PROGMEM = "RET NZ";
const char mnemonic_ret_pe[] PROGMEM = "RET PE";
const char mnemonic_ret_po[] PROGMEM = "RET PO";
const char mnemonic_ret_m[] PROGMEM = "RET M";
const char mnemonic_ret_p[] PROGMEM = "RET P";
const char mnemonic_ret_i[] PROGMEM = "RET I";
const char mnemonic_ret_n[] PROGMEM = "RET N";
const char mnemonic_rst_0[] PROGMEM = "RST 0";
const char mnemonic_rst_8[] PROGMEM = "RST 8";
const char mnemonic_rst_16[] PROGMEM = "RST 16";
const char mnemonic_rst_24[] PROGMEM = "RST 24";
const char mnemonic_rst_32[] PROGMEM = "RST 32";
const char mnemonic_rst_40[] PROGMEM = "RST 40";
const char mnemonic_rst_48[] PROGMEM = "RST 48";
const char mnemonic_rst_56[] PROGMEM = "RST 56";
const char mnemonic_in_a_ptr_n[] PROGMEM = "IN A,(n)";
const char mnemonic_in_a_ptr_c[] PROGMEM = "IN A,(C)";
const char mnemonic_in_b_ptr_c[] PROGMEM = "IN B,(C)";
const char mnemonic_in_c_ptr_c[] PROGMEM = "IN C,(C)";
const char mnemonic_in_d_ptr_c[] PROGMEM = "IN D,(C)";
const char mnemonic_in_e_ptr_c[] PROGMEM = "IN E,(C)";
const char mnemonic_in_h_ptr_c[] PROGMEM = "IN H,(C)";
const char mnemonic_in_l_ptr_c[] PROGMEM = "IN L,(C)";
const char mnemonic_ini[] PROGMEM = "INI";
const char mnemonic_inir[] PROGMEM = "INIR";
const char mnemonic_ind[] PROGMEM = "IND";
const char mnemonic_indr[] PROGMEM = "INDR";
const char mnemonic_out_ptr_n_a[] PROGMEM = "OUT (n),A";
const char mnemonic_out_ptr_c_a[] PROGMEM = "OUT (C),A";
const char mnemonic_out_ptr_c_b[] PROGMEM = "OUT (C),B";
const char mnemonic_out_ptr_c_c[] PROGMEM = "OUT (C),C";
const char mnemonic_out_ptr_c_d[] PROGMEM = "OUT (C),D";
const char mnemonic_out_ptr_c_e[] PROGMEM = "OUT (C),E";
const char mnemonic_out_ptr_c_h[] PROGMEM = "OUT (C),H";
const char mnemonic_out_ptr_c_l[] PROGMEM = "OUT (C),L";
const char mnemonic_outi[] PROGMEM = "OUTI";
const char mnemonic_outir[] PROGMEM = "OUTIR";
const char mnemonic_outd[] PROGMEM = "OUTD";
const char mnemonic_outdr[] PROGMEM = "OUTDR";
const char mnemonic_halt[] PROGMEM = "HALT";
const char mnemonic_di[] PROGMEM = "DI";
const char mnemonic_ei[] PROGMEM = "EI";
const char mnemonic_im0[] PROGMEM = "IM0";
const char mnemonic_im1[] PROGMEM = "IM1";
const char mnemonic_im2[] PROGMEM = "IM2";

const char operand_n[] PROGMEM = "n";

bool NOP();
bool LD_A_N();
bool LD_B_N();
bool LD_C_N();
bool LD_D_N();
bool LD_E_N();
bool LD_H_N();
bool LD_L_N();
bool LD_PTR_HL_N();
bool LD_A_A();
bool LD_B_A();
bool LD_C_A();
bool LD_D_A();
bool LD_E_A();
bool LD_H_A();
bool LD_L_A();
bool LD_A_B();
bool LD_B_B();
bool LD_C_B();
bool LD_D_B();
bool LD_E_B();
bool LD_H_B();
bool LD_L_B();
bool LD_A_C();
bool LD_B_C();
bool LD_C_C();
bool LD_D_C();
bool LD_E_C();
bool LD_H_C();
bool LD_L_C();
bool LD_A_D();
bool LD_B_D();
bool LD_C_D();
bool LD_D_D();
bool LD_E_D();
bool LD_H_D();
bool LD_L_D();
bool LD_A_E();
bool LD_B_E();
bool LD_C_E();
bool LD_D_E();
bool LD_E_E();
bool LD_H_E();
bool LD_L_E();
bool LD_A_H();
bool LD_B_H();
bool LD_C_H();
bool LD_D_H();
bool LD_E_H();
bool LD_H_H();
bool LD_L_H();
bool LD_A_L();
bool LD_B_L();
bool LD_C_L();
bool LD_D_L();
bool LD_E_L();
bool LD_H_L();
bool LD_L_L();
bool LD_A_PTR_HL();
bool LD_B_PTR_HL();
bool LD_C_PTR_HL();
bool LD_D_PTR_HL();
bool LD_E_PTR_HL();
bool LD_H_PTR_HL();
bool LD_L_PTR_HL();
bool LD_PTR_HL_A();
bool LD_PTR_HL_B();
bool LD_PTR_HL_C();
bool LD_PTR_HL_D();
bool LD_PTR_HL_E();
bool LD_PTR_HL_H();
bool LD_PTR_HL_L();
bool LD_A_PTR_BC();
bool LD_A_PTR_DE();
bool LD_A_PTR_NN();
bool LD_PTR_NN_A();
bool LD_PTR_BC_A();
bool LD_PTR_DE_A();
bool LD_A_I();
bool LD_A_R();
bool LD_I_A();
bool LD_R_A();
bool LD_PTR_IX_PLUS_D_N();
bool LD_PTR_IY_PLUS_D_N();
bool LD_A_PTR_IX_PLUS_D();
bool LD_B_PTR_IX_PLUS_D();
bool LD_C_PTR_IX_PLUS_D();
bool LD_D_PTR_IX_PLUS_D();
bool LD_E_PTR_IX_PLUS_D();
bool LD_H_PTR_IX_PLUS_D();
bool LD_L_PTR_IX_PLUS_D();
bool LD_A_PTR_IY_PLUS_D();
bool LD_B_PTR_IY_PLUS_D();
bool LD_C_PTR_IY_PLUS_D();
bool LD_D_PTR_IY_PLUS_D();
bool LD_E_PTR_IY_PLUS_D();
bool LD_H_PTR_IY_PLUS_D();
bool LD_L_PTR_IY_PLUS_D();
bool LD_PTR_IX_PLUS_D_A();
bool LD_PTR_IX_PLUS_D_B();
bool LD_PTR_IX_PLUS_D_C();
bool LD_PTR_IX_PLUS_D_D();
bool LD_PTR_IX_PLUS_D_E();
bool LD_PTR_IX_PLUS_D_H();
bool LD_PTR_IX_PLUS_D_L();
bool LD_PTR_IY_PLUS_D_A();
bool LD_PTR_IY_PLUS_D_B();
bool LD_PTR_IY_PLUS_D_C();
bool LD_PTR_IY_PLUS_D_D();
bool LD_PTR_IY_PLUS_D_E();
bool LD_PTR_IY_PLUS_D_H();
bool LD_PTR_IY_PLUS_D_L();
bool LD_BC_NN();
bool LD_DE_NN();
bool LD_HL_NN();
bool LD_SP_NN();
bool LD_IX_NN();
bool LD_IY_NN();
bool LD_BC_PTR_NN();
bool LD_DE_PTR_NN();
bool LD_HL_PTR_NN();
bool LD_SP_PTR_NN();
bool LD_IX_PTR_NN();
bool LD_IY_PTR_NN();
bool LD_PTR_NN_BC();
bool LD_PTR_NN_DE();
bool LD_PTR_NN_HL();
bool LD_PTR_NN_SP();
bool LD_PTR_NN_IX();
bool LD_PTR_NN_IY();
bool LD_SP_HL();
bool LD_SP_IX();
bool LD_SP_IY();
bool POP_AF();
bool POP_BC();
bool POP_DE();
bool POP_HL();
bool POP_IX();
bool POP_IY();
bool PUSH_AF();
bool PUSH_BC();
bool PUSH_DE();
bool PUSH_HL();
bool PUSH_IX();
bool PUSH_IY();
bool EX_AF_AF_PRIME();
bool EXX();
bool EX_DE_HL();
bool EX_PTR_SP_HL();
bool EX_PTR_SP_IX();
bool EX_PTR_SP_IY();
bool LDI();
bool LDIR();
bool LDD();
bool LDDR();
bool CPI();
bool CPIR();
bool CPD();
bool CPDR();
bool ADD_A_A();
bool ADD_A_B();
bool ADD_A_C();
bool ADD_A_D();
bool ADD_A_E();
bool ADD_A_H();
bool ADD_A_L();
bool ADD_A_PTR_HL();
bool ADD_A_PTR_IX_PLUS_D();
bool ADD_A_PTR_IY_PLUS_D();
bool ADD_A_N();
bool ADC_A_A();
bool ADC_A_B();
bool ADC_A_C();
bool ADC_A_D();
bool ADC_A_E();
bool ADC_A_H();
bool ADC_A_L();
bool ADC_A_PTR_HL();
bool ADC_A_PTR_IX_PLUS_D();
bool ADC_A_PTR_IY_PLUS_D();
bool ADC_A_N();
bool SUB_A_A();
bool SUB_A_B();
bool SUB_A_C();
bool SUB_A_D();
bool SUB_A_E();
bool SUB_A_H();
bool SUB_A_L();
bool SUB_A_PTR_HL();
bool SUB_A_PTR_IX_PLUS_D();
bool SUB_A_PTR_IY_PLUS_D();
bool SUB_A_N();
bool SBC_A_A();
bool SBC_A_B();
bool SBC_A_C();
bool SBC_A_D();
bool SBC_A_E();
bool SBC_A_H();
bool SBC_A_L();
bool SBC_A_PTR_HL();
bool SBC_A_PTR_IX_PLUS_D();
bool SBC_A_PTR_IY_PLUS_D();
bool SBC_A_N();
bool AND_A();
bool AND_B();
bool AND_C();
bool AND_D();
bool AND_E();
bool AND_H();
bool AND_L();
bool AND_PTR_HL();
bool AND_PTR_IX_PLUS_D();
bool AND_PTR_IY_PLUS_D();
bool AND_N();
bool XOR_A();
bool XOR_B();
bool XOR_C();
bool XOR_D();
bool XOR_E();
bool XOR_H();
bool XOR_L();
bool XOR_PTR_HL();
bool XOR_PTR_IX_PLUS_D();
bool XOR_PTR_IY_PLUS_D();
bool XOR_N();
bool OR_A();
bool OR_B();
bool OR_C();
bool OR_D();
bool OR_E();
bool OR_H();
bool OR_L();
bool OR_PTR_HL();
bool OR_PTR_IX_PLUS_D();
bool OR_PTR_IY_PLUS_D();
bool OR_N();
bool CP_A();
bool CP_B();
bool CP_C();
bool CP_D();
bool CP_E();
bool CP_H();
bool CP_L();
bool CP_PTR_HL();
bool CP_PTR_IX_PLUS_D();
bool CP_PTR_IY_PLUS_D();
bool CP_N();
bool INC_A();
bool INC_B();
bool INC_C();
bool INC_D();
bool INC_E();
bool INC_H();
bool INC_L();
bool INC_PTR_HL();
bool INC_PTR_IX_PLUS_D();
bool INC_PTR_IY_PLUS_D();
bool DEC_A();
bool DEC_B();
bool DEC_C();
bool DEC_D();
bool DEC_E();
bool DEC_H();
bool DEC_L();
bool DEC_PTR_HL();
bool DEC_PTR_IX_PLUS_D();
bool DEC_PTR_IY_PLUS_D();
bool DAA();
bool CPL();
bool NEG();
bool CCF();
bool SCF();
bool ADD_HL_BC();
bool ADD_HL_DE();
bool ADD_HL_HL();
bool ADD_HL_SP();
bool ADD_IX_BC();
bool ADD_IX_DE();
bool ADD_IX_SP();
bool ADD_IX_IX();
bool ADD_IY_BC();
bool ADD_IY_DE();
bool ADD_IY_SP();
bool ADD_IY_IY();
bool ADC_HL_BC();
bool ADC_HL_DE();
bool ADC_HL_HL();
bool ADC_HL_SP();
bool SBC_HL_BC();
bool SBC_HL_DE();
bool SBC_HL_HL();
bool SBC_HL_SP();
bool INC_BC();
bool INC_DE();
bool INC_HL();
bool INC_SP();
bool INC_IX();
bool INC_IY();
bool DEC_BC();
bool DEC_DE();
bool DEC_HL();
bool DEC_SP();
bool DEC_IX();
bool DEC_IY();
bool RLC_A();
bool RLC_B();
bool RLC_C();
bool RLC_D();
bool RLC_E();
bool RLC_H();
bool RLC_L();
bool RLC_PTR_HL();
bool RLC_PTR_IX_PLUS_D();
bool RLC_PTR_IY_PLUS_D();
bool RRC_A();
bool RRC_B();
bool RRC_C();
bool RRC_D();
bool RRC_E();
bool RRC_H();
bool RRC_L();
bool RRC_PTR_HL();
bool RRC_PTR_IX_PLUS_D();
bool RRC_PTR_IY_PLUS_D();
bool RL_A();
bool RL_B();
bool RL_C();
bool RL_D();
bool RL_E();
bool RL_H();
bool RL_L();
bool RL_PTR_HL();
bool RL_PTR_IX_PLUS_D();
bool RL_PTR_IY_PLUS_D();
bool RR_A();
bool RR_B();
bool RR_C();
bool RR_D();
bool RR_E();
bool RR_H();
bool RR_L();
bool RR_PTR_HL();
bool RR_PTR_IX_PLUS_D();
bool RR_PTR_IY_PLUS_D();
bool SLA_A();
bool SLA_B();
bool SLA_C();
bool SLA_D();
bool SLA_E();
bool SLA_H();
bool SLA_L();
bool SLA_PTR_HL();
bool SLA_PTR_IX_PLUS_D();
bool SLA_PTR_IY_PLUS_D();
bool SRA_A();
bool SRA_B();
bool SRA_C();
bool SRA_D();
bool SRA_E();
bool SRA_H();
bool SRA_L();
bool SRA_PTR_HL();
bool SRA_PTR_IX_PLUS_D();
bool SRA_PTR_IY_PLUS_D();
bool SLL_A();
bool SLL_B();
bool SLL_C();
bool SLL_D();
bool SLL_E();
bool SLL_H();
bool SLL_L();
bool SLL_PTR_HL();
bool SLL_PTR_IX_PLUS_D();
bool SLL_PTR_IY_PLUS_D();
bool SRL_A();
bool SRL_B();
bool SRL_C();
bool SRL_D();
bool SRL_E();
bool SRL_H();
bool SRL_L();
bool SRL_PTR_HL();
bool SRL_PTR_IX_PLUS_D();
bool SRL_PTR_IY_PLUS_D();
bool RLD();
bool RRD();
bool RLCA();
bool RRCA();
bool RLA();
bool RRA();
bool BIT_0_A();
bool BIT_0_B();
bool BIT_0_C();
bool BIT_0_D();
bool BIT_0_E();
bool BIT_0_H();
bool BIT_0_L();
bool BIT_0_PTR_HL();
bool BIT_0_PTR_IX_PLUS_D();
bool BIT_0_PTR_IY_PLUS_D();
bool BIT_1_A();
bool BIT_1_B();
bool BIT_1_C();
bool BIT_1_D();
bool BIT_1_E();
bool BIT_1_H();
bool BIT_1_L();
bool BIT_1_PTR_HL();
bool BIT_1_PTR_IX_PLUS_D();
bool BIT_1_PTR_IY_PLUS_D();
bool BIT_2_A();
bool BIT_2_B();
bool BIT_2_C();
bool BIT_2_D();
bool BIT_2_E();
bool BIT_2_H();
bool BIT_2_L();
bool BIT_2_PTR_HL();
bool BIT_2_PTR_IX_PLUS_D();
bool BIT_2_PTR_IY_PLUS_D();
bool BIT_3_A();
bool BIT_3_B();
bool BIT_3_C();
bool BIT_3_D();
bool BIT_3_E();
bool BIT_3_H();
bool BIT_3_L();
bool BIT_3_PTR_HL();
bool BIT_3_PTR_IX_PLUS_D();
bool BIT_3_PTR_IY_PLUS_D();
bool BIT_4_A();
bool BIT_4_B();
bool BIT_4_C();
bool BIT_4_D();
bool BIT_4_E();
bool BIT_4_H();
bool BIT_4_L();
bool BIT_4_PTR_HL();
bool BIT_4_PTR_IX_PLUS_D();
bool BIT_4_PTR_IY_PLUS_D();
bool BIT_5_A();
bool BIT_5_B();
bool BIT_5_C();
bool BIT_5_D();
bool BIT_5_E();
bool BIT_5_H();
bool BIT_5_L();
bool BIT_5_PTR_HL();
bool BIT_5_PTR_IX_PLUS_D();
bool BIT_5_PTR_IY_PLUS_D();
bool BIT_6_A();
bool BIT_6_B();
bool BIT_6_C();
bool BIT_6_D();
bool BIT_6_E();
bool BIT_6_H();
bool BIT_6_L();
bool BIT_6_PTR_HL();
bool BIT_6_PTR_IX_PLUS_D();
bool BIT_6_PTR_IY_PLUS_D();
bool BIT_7_A();
bool BIT_7_B();
bool BIT_7_C();
bool BIT_7_D();
bool BIT_7_E();
bool BIT_7_H();
bool BIT_7_L();
bool BIT_7_PTR_HL();
bool BIT_7_PTR_IX_PLUS_D();
bool BIT_7_PTR_IY_PLUS_D();
bool RES_0_A();
bool RES_0_B();
bool RES_0_C();
bool RES_0_D();
bool RES_0_E();
bool RES_0_H();
bool RES_0_L();
bool RES_0_PTR_HL();
bool RES_0_PTR_IX_PLUS_D();
bool RES_0_PTR_IY_PLUS_D();
bool RES_1_A();
bool RES_1_B();
bool RES_1_C();
bool RES_1_D();
bool RES_1_E();
bool RES_1_H();
bool RES_1_L();
bool RES_1_PTR_HL();
bool RES_1_PTR_IX_PLUS_D();
bool RES_1_PTR_IY_PLUS_D();
bool RES_2_A();
bool RES_2_B();
bool RES_2_C();
bool RES_2_D();
bool RES_2_E();
bool RES_2_H();
bool RES_2_L();
bool RES_2_PTR_HL();
bool RES_2_PTR_IX_PLUS_D();
bool RES_2_PTR_IY_PLUS_D();
bool RES_3_A();
bool RES_3_B();
bool RES_3_C();
bool RES_3_D();
bool RES_3_E();
bool RES_3_H();
bool RES_3_L();
bool RES_3_PTR_HL();
bool RES_3_PTR_IX_PLUS_D();
bool RES_3_PTR_IY_PLUS_D();
bool RES_4_A();
bool RES_4_B();
bool RES_4_C();
bool RES_4_D();
bool RES_4_E();
bool RES_4_H();
bool RES_4_L();
bool RES_4_PTR_HL();
bool RES_4_PTR_IX_PLUS_D();
bool RES_4_PTR_IY_PLUS_D();
bool RES_5_A();
bool RES_5_B();
bool RES_5_C();
bool RES_5_D();
bool RES_5_E();
bool RES_5_H();
bool RES_5_L();
bool RES_5_PTR_HL();
bool RES_5_PTR_IX_PLUS_D();
bool RES_5_PTR_IY_PLUS_D();
bool RES_6_A();
bool RES_6_B();
bool RES_6_C();
bool RES_6_D();
bool RES_6_E();
bool RES_6_H();
bool RES_6_L();
bool RES_6_PTR_HL();
bool RES_6_PTR_IX_PLUS_D();
bool RES_6_PTR_IY_PLUS_D();
bool RES_7_A();
bool RES_7_B();
bool RES_7_C();
bool RES_7_D();
bool RES_7_E();
bool RES_7_H();
bool RES_7_L();
bool RES_7_PTR_HL();
bool RES_7_PTR_IX_PLUS_D();
bool RES_7_PTR_IY_PLUS_D();
bool SET_0_A();
bool SET_0_B();
bool SET_0_C();
bool SET_0_D();
bool SET_0_E();
bool SET_0_H();
bool SET_0_L();
bool SET_0_PTR_HL();
bool SET_0_PTR_IX_PLUS_D();
bool SET_0_PTR_IY_PLUS_D();
bool SET_1_A();
bool SET_1_B();
bool SET_1_C();
bool SET_1_D();
bool SET_1_E();
bool SET_1_H();
bool SET_1_L();
bool SET_1_PTR_HL();
bool SET_1_PTR_IX_PLUS_D();
bool SET_1_PTR_IY_PLUS_D();
bool SET_2_A();
bool SET_2_B();
bool SET_2_C();
bool SET_2_D();
bool SET_2_E();
bool SET_2_H();
bool SET_2_L();
bool SET_2_PTR_HL();
bool SET_2_PTR_IX_PLUS_D();
bool SET_2_PTR_IY_PLUS_D();
bool SET_3_A();
bool SET_3_B();
bool SET_3_C();
bool SET_3_D();
bool SET_3_E();
bool SET_3_H();
bool SET_3_L();
bool SET_3_PTR_HL();
bool SET_3_PTR_IX_PLUS_D();
bool SET_3_PTR_IY_PLUS_D();
bool SET_4_A();
bool SET_4_B();
bool SET_4_C();
bool SET_4_D();
bool SET_4_E();
bool SET_4_H();
bool SET_4_L();
bool SET_4_PTR_HL();
bool SET_4_PTR_IX_PLUS_D();
bool SET_4_PTR_IY_PLUS_D();
bool SET_5_A();
bool SET_5_B();
bool SET_5_C();
bool SET_5_D();
bool SET_5_E();
bool SET_5_H();
bool SET_5_L();
bool SET_5_PTR_HL();
bool SET_5_PTR_IX_PLUS_D();
bool SET_5_PTR_IY_PLUS_D();
bool SET_6_A();
bool SET_6_B();
bool SET_6_C();
bool SET_6_D();
bool SET_6_E();
bool SET_6_H();
bool SET_6_L();
bool SET_6_PTR_HL();
bool SET_6_PTR_IX_PLUS_D();
bool SET_6_PTR_IY_PLUS_D();
bool SET_7_A();
bool SET_7_B();
bool SET_7_C();
bool SET_7_D();
bool SET_7_E();
bool SET_7_H();
bool SET_7_L();
bool SET_7_PTR_HL();
bool SET_7_PTR_IX_PLUS_D();
bool SET_7_PTR_IY_PLUS_D();
bool JP_NN();
bool JP_C_NN();
bool JP_NC_NN();
bool JP_Z_NN();
bool JP_NZ_NN();
bool JP_PE_NN();
bool JP_PO_NN();
bool JP_M_NN();
bool JP_P_NN();
bool JR_E();
bool JR_C_E();
bool JR_NC_E();
bool JR_Z_E();
bool JR_NZ_E();
bool JP_PTR_HL();
bool JP_PTR_IX();
bool JP_PTR_IY();
bool CALL_NN();
bool CALL_C_NN();
bool CALL_NC_NN();
bool CALL_Z_NN();
bool CALL_NZ_NN();
bool CALL_PE_NN();
bool CALL_PO_NN();
bool CALL_M_NN();
bool CALL_P_NN();
bool DJNZ_E();
bool RET();
bool RET_C();
bool RET_NC();
bool RET_Z();
bool RET_NZ();
bool RET_PE();
bool RET_PO();
bool RET_M();
bool RET_P();
bool RET_I();
bool RET_N();
bool RST_0();
bool RST_8();
bool RST_16();
bool RST_24();
bool RST_32();
bool RST_40();
bool RST_48();
bool RST_56();
bool IN_A_PTR_N();
bool IN_A_PTR_C();
bool IN_B_PTR_C();
bool IN_C_PTR_C();
bool IN_D_PTR_C();
bool IN_E_PTR_C();
bool IN_H_PTR_C();
bool IN_L_PTR_C();
bool INI();
bool INIR();
bool IND();
bool INDR();
bool OUT_PTR_N_A();
bool OUT_PTR_C_A();
bool OUT_PTR_C_B();
bool OUT_PTR_C_C();
bool OUT_PTR_C_D();
bool OUT_PTR_C_E();
bool OUT_PTR_C_H();
bool OUT_PTR_C_L();
bool OUTI();
bool OUTIR();
bool OUTD();
bool OUTDR();
bool HALT_();
bool DI();
bool EI();
bool IM0();
bool IM1();
bool IM2();

const instructionDefinitionType InstructionDefinitions[] PROGMEM = {
  {GROUP_MISCELLANEOUS_CPU_CONTROL,mnemonic_nop,0x00,NULL,NULL,NULL,NOP},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_n,0x3E,operand_n,NULL,NULL,LD_A_N},
  {GROUP_8BIT_LOAD,mnemonic_ld_b_n,0x06,operand_n,NULL,NULL,LD_B_N},
  {GROUP_8BIT_LOAD,mnemonic_ld_c_n,0x0E,operand_n,NULL,NULL,LD_C_N},
  {GROUP_8BIT_LOAD,mnemonic_ld_d_n,0x16,operand_n,NULL,NULL,LD_D_N},
  {GROUP_8BIT_LOAD,mnemonic_ld_e_n,0x1E,operand_n,NULL,NULL,LD_E_N},
  {GROUP_8BIT_LOAD,mnemonic_ld_h_n,0x26,operand_n,NULL,NULL,LD_H_N},
  {GROUP_8BIT_LOAD,mnemonic_ld_l_n,0x2E,operand_n,NULL,NULL,LD_L_N},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_hl_n,0x36,operand_n,NULL,NULL,LD_PTR_HL_N},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_a,0x7F,NULL,NULL,NULL,LD_A_A},
  {GROUP_8BIT_LOAD,mnemonic_ld_b_a,0x47,NULL,NULL,NULL,LD_B_A},
  {GROUP_8BIT_LOAD,mnemonic_ld_c_a,0x4F,NULL,NULL,NULL,LD_C_A},
  {GROUP_8BIT_LOAD,mnemonic_ld_d_a,0x57,NULL,NULL,NULL,LD_D_A},
  {GROUP_8BIT_LOAD,mnemonic_ld_e_a,0x5F,NULL,NULL,NULL,LD_E_A},
  {GROUP_8BIT_LOAD,mnemonic_ld_h_a,0x67,NULL,NULL,NULL,LD_H_A},
  {GROUP_8BIT_LOAD,mnemonic_ld_l_a,0x6F,NULL,NULL,NULL,LD_L_A},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_b,0x78,NULL,NULL,NULL,LD_A_B},
  {GROUP_8BIT_LOAD,mnemonic_ld_b_b,0x40,NULL,NULL,NULL,LD_B_B},
  {GROUP_8BIT_LOAD,mnemonic_ld_c_b,0x48,NULL,NULL,NULL,LD_C_B},
  {GROUP_8BIT_LOAD,mnemonic_ld_d_b,0x50,NULL,NULL,NULL,LD_D_B},
  {GROUP_8BIT_LOAD,mnemonic_ld_e_b,0x58,NULL,NULL,NULL,LD_E_B},
  {GROUP_8BIT_LOAD,mnemonic_ld_h_b,0x60,NULL,NULL,NULL,LD_H_B},
  {GROUP_8BIT_LOAD,mnemonic_ld_l_b,0x68,NULL,NULL,NULL,LD_L_B},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_c,0x79,NULL,NULL,NULL,LD_A_C},
  {GROUP_8BIT_LOAD,mnemonic_ld_b_c,0x41,NULL,NULL,NULL,LD_B_C},
  {GROUP_8BIT_LOAD,mnemonic_ld_c_c,0x49,NULL,NULL,NULL,LD_C_C},
  {GROUP_8BIT_LOAD,mnemonic_ld_d_c,0x51,NULL,NULL,NULL,LD_D_C},
  {GROUP_8BIT_LOAD,mnemonic_ld_e_c,0x59,NULL,NULL,NULL,LD_E_C},
  {GROUP_8BIT_LOAD,mnemonic_ld_h_c,0x61,NULL,NULL,NULL,LD_H_C},
  {GROUP_8BIT_LOAD,mnemonic_ld_l_c,0x69,NULL,NULL,NULL,LD_L_C},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_d,0x7A,NULL,NULL,NULL,LD_A_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_b_d,0x42,NULL,NULL,NULL,LD_B_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_c_d,0x4A,NULL,NULL,NULL,LD_C_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_d_d,0x52,NULL,NULL,NULL,LD_D_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_e_d,0x5A,NULL,NULL,NULL,LD_E_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_h_d,0x62,NULL,NULL,NULL,LD_H_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_l_d,0x6A,NULL,NULL,NULL,LD_L_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_e,0x7B,NULL,NULL,NULL,LD_A_E},
  {GROUP_8BIT_LOAD,mnemonic_ld_b_e,0x43,NULL,NULL,NULL,LD_B_E},
  {GROUP_8BIT_LOAD,mnemonic_ld_c_e,0x4B,NULL,NULL,NULL,LD_C_E},
  {GROUP_8BIT_LOAD,mnemonic_ld_d_e,0x53,NULL,NULL,NULL,LD_D_E},
  {GROUP_8BIT_LOAD,mnemonic_ld_e_e,0x5B,NULL,NULL,NULL,LD_E_E},
  {GROUP_8BIT_LOAD,mnemonic_ld_h_e,0x63,NULL,NULL,NULL,LD_H_E},
  {GROUP_8BIT_LOAD,mnemonic_ld_l_e,0x6B,NULL,NULL,NULL,LD_L_E},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_h,0x7C,NULL,NULL,NULL,LD_A_H},
  {GROUP_8BIT_LOAD,mnemonic_ld_b_h,0x44,NULL,NULL,NULL,LD_B_H},
  {GROUP_8BIT_LOAD,mnemonic_ld_c_h,0x4C,NULL,NULL,NULL,LD_C_H},
  {GROUP_8BIT_LOAD,mnemonic_ld_d_h,0x54,NULL,NULL,NULL,LD_D_H},
  {GROUP_8BIT_LOAD,mnemonic_ld_e_h,0x5C,NULL,NULL,NULL,LD_E_H},
  {GROUP_8BIT_LOAD,mnemonic_ld_h_h,0x64,NULL,NULL,NULL,LD_H_H},
  {GROUP_8BIT_LOAD,mnemonic_ld_l_h,0x6C,NULL,NULL,NULL,LD_L_H},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_l,0x7D,NULL,NULL,NULL,LD_A_L},
  {GROUP_8BIT_LOAD,mnemonic_ld_b_l,0x45,NULL,NULL,NULL,LD_B_L},
  {GROUP_8BIT_LOAD,mnemonic_ld_c_l,0x4D,NULL,NULL,NULL,LD_C_L},
  {GROUP_8BIT_LOAD,mnemonic_ld_d_l,0x55,NULL,NULL,NULL,LD_D_L},
  {GROUP_8BIT_LOAD,mnemonic_ld_e_l,0x5D,NULL,NULL,NULL,LD_E_L},
  {GROUP_8BIT_LOAD,mnemonic_ld_h_l,0x65,NULL,NULL,NULL,LD_H_L},
  {GROUP_8BIT_LOAD,mnemonic_ld_l_l,0x6D,NULL,NULL,NULL,LD_L_L},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_ptr_hl,0x7E,NULL,NULL,NULL,LD_A_PTR_HL},
  {GROUP_8BIT_LOAD,mnemonic_ld_b_ptr_hl,0x46,NULL,NULL,NULL,LD_B_PTR_HL},
  {GROUP_8BIT_LOAD,mnemonic_ld_c_ptr_hl,0x4E,NULL,NULL,NULL,LD_C_PTR_HL},
  {GROUP_8BIT_LOAD,mnemonic_ld_d_ptr_hl,0x56,NULL,NULL,NULL,LD_D_PTR_HL},
  {GROUP_8BIT_LOAD,mnemonic_ld_e_ptr_hl,0x5E,NULL,NULL,NULL,LD_E_PTR_HL},
  {GROUP_8BIT_LOAD,mnemonic_ld_h_ptr_hl,0x66,NULL,NULL,NULL,LD_H_PTR_HL},
  {GROUP_8BIT_LOAD,mnemonic_ld_l_ptr_hl,0x6E,NULL,NULL,NULL,LD_L_PTR_HL},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_hl_a,0x77,NULL,NULL,NULL,LD_PTR_HL_A},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_hl_b,0x70,NULL,NULL,NULL,LD_PTR_HL_B},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_hl_c,0x71,NULL,NULL,NULL,LD_PTR_HL_C},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_hl_d,0x72,NULL,NULL,NULL,LD_PTR_HL_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_hl_e,0x73,NULL,NULL,NULL,LD_PTR_HL_E},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_hl_h,0x74,NULL,NULL,NULL,LD_PTR_HL_H},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_hl_l,0x75,NULL,NULL,NULL,LD_PTR_HL_L},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_ptr_bc,0x0A,NULL,NULL,NULL,LD_A_PTR_BC},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_ptr_de,0x1A,NULL,NULL,NULL,LD_A_PTR_DE},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_ptr_nn,0x3A,operand_n,"n",NULL,LD_A_PTR_NN},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_nn_a,0x32,operand_n,"n",NULL,LD_PTR_NN_A},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_bc_a,0x02,NULL,NULL,NULL,LD_PTR_BC_A},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_de_a,0x12,NULL,NULL,NULL,LD_PTR_DE_A},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_i,0xED57,NULL,NULL,NULL,LD_A_I},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_r,0xED5F,NULL,NULL,NULL,LD_A_R},
  {GROUP_8BIT_LOAD,mnemonic_ld_i_a,0xED47,NULL,NULL,NULL,LD_I_A},
  {GROUP_8BIT_LOAD,mnemonic_ld_r_a,0xED4F,NULL,NULL,NULL,LD_R_A},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_ix_plus_d_n,0xDD36,operand_n,"n",NULL,LD_PTR_IX_PLUS_D_N},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_iy_plus_d_n,0xFD36,operand_n,"n",NULL,LD_PTR_IY_PLUS_D_N},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_ptr_ix_plus_d,0xDD7E,operand_n,NULL,NULL,LD_A_PTR_IX_PLUS_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_b_ptr_ix_plus_d,0xDD46,operand_n,NULL,NULL,LD_B_PTR_IX_PLUS_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_c_ptr_ix_plus_d,0xDD4E,operand_n,NULL,NULL,LD_C_PTR_IX_PLUS_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_d_ptr_ix_plus_d,0xDD56,operand_n,NULL,NULL,LD_D_PTR_IX_PLUS_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_e_ptr_ix_plus_d,0xDD5E,operand_n,NULL,NULL,LD_E_PTR_IX_PLUS_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_h_ptr_ix_plus_d,0xDD66,operand_n,NULL,NULL,LD_H_PTR_IX_PLUS_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_l_ptr_ix_plus_d,0xDD6E,operand_n,NULL,NULL,LD_L_PTR_IX_PLUS_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_a_ptr_iy_plus_d,0xFD7E,operand_n,NULL,NULL,LD_A_PTR_IY_PLUS_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_b_ptr_iy_plus_d,0xFD46,operand_n,NULL,NULL,LD_B_PTR_IY_PLUS_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_c_ptr_iy_plus_d,0xFD4E,operand_n,NULL,NULL,LD_C_PTR_IY_PLUS_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_d_ptr_iy_plus_d,0xFD56,operand_n,NULL,NULL,LD_D_PTR_IY_PLUS_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_e_ptr_iy_plus_d,0xFD5E,operand_n,NULL,NULL,LD_E_PTR_IY_PLUS_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_h_ptr_iy_plus_d,0xFD66,operand_n,NULL,NULL,LD_H_PTR_IY_PLUS_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_l_ptr_iy_plus_d,0xFD6E,operand_n,NULL,NULL,LD_L_PTR_IY_PLUS_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_ix_plus_d_a,0xDD77,operand_n,NULL,NULL,LD_PTR_IX_PLUS_D_A},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_ix_plus_d_b,0xDD70,operand_n,NULL,NULL,LD_PTR_IX_PLUS_D_B},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_ix_plus_d_c,0xDD71,operand_n,NULL,NULL,LD_PTR_IX_PLUS_D_C},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_ix_plus_d_d,0xDD72,operand_n,NULL,NULL,LD_PTR_IX_PLUS_D_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_ix_plus_d_e,0xDD73,operand_n,NULL,NULL,LD_PTR_IX_PLUS_D_E},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_ix_plus_d_h,0xDD74,operand_n,NULL,NULL,LD_PTR_IX_PLUS_D_H},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_ix_plus_d_l,0xDD75,operand_n,NULL,NULL,LD_PTR_IX_PLUS_D_L},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_iy_plus_d_a,0xFD77,operand_n,NULL,NULL,LD_PTR_IY_PLUS_D_A},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_iy_plus_d_b,0xFD70,operand_n,NULL,NULL,LD_PTR_IY_PLUS_D_B},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_iy_plus_d_c,0xFD71,operand_n,NULL,NULL,LD_PTR_IY_PLUS_D_C},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_iy_plus_d_d,0xFD72,operand_n,NULL,NULL,LD_PTR_IY_PLUS_D_D},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_iy_plus_d_e,0xFD73,operand_n,NULL,NULL,LD_PTR_IY_PLUS_D_E},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_iy_plus_d_h,0xFD74,operand_n,NULL,NULL,LD_PTR_IY_PLUS_D_H},
  {GROUP_8BIT_LOAD,mnemonic_ld_ptr_iy_plus_d_l,0xFD75,operand_n,NULL,NULL,LD_PTR_IY_PLUS_D_L},
  {GROUP_16BIT_LOAD,mnemonic_ld_bc_nn,0x01,operand_n,"n",NULL,LD_BC_NN},
  {GROUP_16BIT_LOAD,mnemonic_ld_de_nn,0x11,operand_n,"n",NULL,LD_DE_NN},
  {GROUP_16BIT_LOAD,mnemonic_ld_hl_nn,0x21,operand_n,"n",NULL,LD_HL_NN},
  {GROUP_16BIT_LOAD,mnemonic_ld_sp_nn,0x31,operand_n,"n",NULL,LD_SP_NN},
  {GROUP_16BIT_LOAD,mnemonic_ld_ix_nn,0xDD21,operand_n,"n",NULL,LD_IX_NN},
  {GROUP_16BIT_LOAD,mnemonic_ld_iy_nn,0xFD21,operand_n,"n",NULL,LD_IY_NN},
  {GROUP_16BIT_LOAD,mnemonic_ld_bc_ptr_nn,0xED4B,operand_n,"n",NULL,LD_BC_PTR_NN},
  {GROUP_16BIT_LOAD,mnemonic_ld_de_ptr_nn,0xED5B,operand_n,"n",NULL,LD_DE_PTR_NN},
  {GROUP_16BIT_LOAD,mnemonic_ld_hl_ptr_nn,0x2A,operand_n,"n",NULL,LD_HL_PTR_NN},
  {GROUP_16BIT_LOAD,mnemonic_ld_sp_ptr_nn,0xED7B,operand_n,"n",NULL,LD_SP_PTR_NN},
  {GROUP_16BIT_LOAD,mnemonic_ld_ix_ptr_nn,0xDD2A,operand_n,"n",NULL,LD_IX_PTR_NN},
  {GROUP_16BIT_LOAD,mnemonic_ld_iy_ptr_nn,0xFD2A,operand_n,"n",NULL,LD_IY_PTR_NN},
  {GROUP_16BIT_LOAD,mnemonic_ld_ptr_nn_bc,0xED43,operand_n,"n",NULL,LD_PTR_NN_BC},
  {GROUP_16BIT_LOAD,mnemonic_ld_ptr_nn_de,0xED53,operand_n,"n",NULL,LD_PTR_NN_DE},
  {GROUP_16BIT_LOAD,mnemonic_ld_ptr_nn_hl,0x22,operand_n,"n",NULL,LD_PTR_NN_HL},
  {GROUP_16BIT_LOAD,mnemonic_ld_ptr_nn_sp,0xED73,operand_n,"n",NULL,LD_PTR_NN_SP},
  {GROUP_16BIT_LOAD,mnemonic_ld_ptr_nn_ix,0xDD22,operand_n,"n",NULL,LD_PTR_NN_IX},
  {GROUP_16BIT_LOAD,mnemonic_ld_ptr_nn_iy,0xFD22,operand_n,"n",NULL,LD_PTR_NN_IY},
  {GROUP_16BIT_LOAD,mnemonic_ld_sp_hl,0xF9,NULL,NULL,NULL,LD_SP_HL},
  {GROUP_16BIT_LOAD,mnemonic_ld_sp_ix,0xDDF9,NULL,NULL,NULL,LD_SP_IX},
  {GROUP_16BIT_LOAD,mnemonic_ld_sp_iy,0xFDF9,NULL,NULL,NULL,LD_SP_IY},
  {GROUP_16BIT_LOAD,mnemonic_pop_af,0xF1,NULL,NULL,NULL,POP_AF},
  {GROUP_16BIT_LOAD,mnemonic_pop_bc,0xC1,NULL,NULL,NULL,POP_BC},
  {GROUP_16BIT_LOAD,mnemonic_pop_de,0xD1,NULL,NULL,NULL,POP_DE},
  {GROUP_16BIT_LOAD,mnemonic_pop_hl,0xE1,NULL,NULL,NULL,POP_HL},
  {GROUP_16BIT_LOAD,mnemonic_pop_ix,0xDDE1,NULL,NULL,NULL,POP_IX},
  {GROUP_16BIT_LOAD,mnemonic_pop_iy,0xFDE1,NULL,NULL,NULL,POP_IY},
  {GROUP_16BIT_LOAD,mnemonic_push_af,0xF5,NULL,NULL,NULL,PUSH_AF},
  {GROUP_16BIT_LOAD,mnemonic_push_bc,0xC5,NULL,NULL,NULL,PUSH_BC},
  {GROUP_16BIT_LOAD,mnemonic_push_de,0xD5,NULL,NULL,NULL,PUSH_DE},
  {GROUP_16BIT_LOAD,mnemonic_push_hl,0xE5,NULL,NULL,NULL,PUSH_HL},
  {GROUP_16BIT_LOAD,mnemonic_push_ix,0xDDE5,NULL,NULL,NULL,PUSH_IX},
  {GROUP_16BIT_LOAD,mnemonic_push_iy,0xFDE5,NULL,NULL,NULL,PUSH_IY},
  {GROUP_EXCHANGES,mnemonic_ex_af_af_prime,0x08,NULL,NULL,NULL,EX_AF_AF_PRIME},
  {GROUP_EXCHANGES,mnemonic_exx,0xD9,NULL,NULL,NULL,EXX},
  {GROUP_EXCHANGES,mnemonic_ex_de_hl,0xEB,NULL,NULL,NULL,EX_DE_HL},
  {GROUP_EXCHANGES,mnemonic_ex_ptr_sp_hl,0xE3,NULL,NULL,NULL,EX_PTR_SP_HL},
  {GROUP_EXCHANGES,mnemonic_ex_ptr_sp_ix,0xDDE3,NULL,NULL,NULL,EX_PTR_SP_IX},
  {GROUP_EXCHANGES,mnemonic_ex_ptr_sp_iy,0xFDE3,NULL,NULL,NULL,EX_PTR_SP_IY},
  {GROUP_BLOCK_TRANSFER,mnemonic_ldi,0xEDA0,NULL,NULL,NULL,LDI},
  {GROUP_BLOCK_TRANSFER,mnemonic_ldir,0xEDB0,NULL,NULL,NULL,LDIR},
  {GROUP_BLOCK_TRANSFER,mnemonic_ldd,0xEDA8,NULL,NULL,NULL,LDD},
  {GROUP_BLOCK_TRANSFER,mnemonic_lddr,0xEDB8,NULL,NULL,NULL,LDDR},
  {GROUP_BLOCK_SEARCH,mnemonic_cpi,0xEDA1,NULL,NULL,NULL,CPI},
  {GROUP_BLOCK_SEARCH,mnemonic_cpir,0xEDB1,NULL,NULL,NULL,CPIR},
  {GROUP_BLOCK_SEARCH,mnemonic_cpd,0xEDA9,NULL,NULL,NULL,CPD},
  {GROUP_BLOCK_SEARCH,mnemonic_cpdr,0xEDB9,NULL,NULL,NULL,CPDR},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_add_a_a,0x87,NULL,NULL,NULL,ADD_A_A},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_add_a_b,0x80,NULL,NULL,NULL,ADD_A_B},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_add_a_c,0x81,NULL,NULL,NULL,ADD_A_C},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_add_a_d,0x82,NULL,NULL,NULL,ADD_A_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_add_a_e,0x83,NULL,NULL,NULL,ADD_A_E},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_add_a_h,0x84,NULL,NULL,NULL,ADD_A_H},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_add_a_l,0x85,NULL,NULL,NULL,ADD_A_L},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_add_a_ptr_hl,0x86,NULL,NULL,NULL,ADD_A_PTR_HL},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_add_a_ptr_ix_plus_d,0xDD86,operand_n,NULL,NULL,ADD_A_PTR_IX_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_add_a_ptr_iy_plus_d,0xFD86,operand_n,NULL,NULL,ADD_A_PTR_IY_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_add_a_n,0xC6,operand_n,NULL,NULL,ADD_A_N},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_adc_a_a,0x8F,NULL,NULL,NULL,ADC_A_A},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_adc_a_b,0x88,NULL,NULL,NULL,ADC_A_B},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_adc_a_c,0x89,NULL,NULL,NULL,ADC_A_C},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_adc_a_d,0x8A,NULL,NULL,NULL,ADC_A_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_adc_a_e,0x8B,NULL,NULL,NULL,ADC_A_E},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_adc_a_h,0x8C,NULL,NULL,NULL,ADC_A_H},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_adc_a_l,0x8D,NULL,NULL,NULL,ADC_A_L},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_adc_a_ptr_hl,0x8E,NULL,NULL,NULL,ADC_A_PTR_HL},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_adc_a_ptr_ix_plus_d,0xDD8E,operand_n,NULL,NULL,ADC_A_PTR_IX_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_adc_a_ptr_iy_plus_d,0xFD8E,operand_n,NULL,NULL,ADC_A_PTR_IY_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_adc_a_n,0xCE,operand_n,NULL,NULL,ADC_A_N},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sub_a_a,0x97,NULL,NULL,NULL,SUB_A_A},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sub_a_b,0x90,NULL,NULL,NULL,SUB_A_B},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sub_a_c,0x91,NULL,NULL,NULL,SUB_A_C},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sub_a_d,0x92,NULL,NULL,NULL,SUB_A_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sub_a_e,0x93,NULL,NULL,NULL,SUB_A_E},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sub_a_h,0x94,NULL,NULL,NULL,SUB_A_H},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sub_a_l,0x95,NULL,NULL,NULL,SUB_A_L},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sub_a_ptr_hl,0x96,NULL,NULL,NULL,SUB_A_PTR_HL},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sub_a_ptr_ix_plus_d,0xDD96,operand_n,NULL,NULL,SUB_A_PTR_IX_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sub_a_ptr_iy_plus_d,0xFD96,operand_n,NULL,NULL,SUB_A_PTR_IY_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sub_a_n,0xD6,operand_n,NULL,NULL,SUB_A_N},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sbc_a_a,0x9F,NULL,NULL,NULL,SBC_A_A},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sbc_a_b,0x98,NULL,NULL,NULL,SBC_A_B},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sbc_a_c,0x99,NULL,NULL,NULL,SBC_A_C},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sbc_a_d,0x9A,NULL,NULL,NULL,SBC_A_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sbc_a_e,0x9B,NULL,NULL,NULL,SBC_A_E},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sbc_a_h,0x9C,NULL,NULL,NULL,SBC_A_H},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sbc_a_l,0x9D,NULL,NULL,NULL,SBC_A_L},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sbc_a_ptr_hl,0x9E,NULL,NULL,NULL,SBC_A_PTR_HL},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sbc_a_ptr_ix_plus_d,0xDD9E,operand_n,NULL,NULL,SBC_A_PTR_IX_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sbc_a_ptr_iy_plus_d,0xFD9E,operand_n,NULL,NULL,SBC_A_PTR_IY_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_sbc_a_n,0xDE,operand_n,NULL,NULL,SBC_A_N},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_and_a,0xA7,NULL,NULL,NULL,AND_A},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_and_b,0xA0,NULL,NULL,NULL,AND_B},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_and_c,0xA1,NULL,NULL,NULL,AND_C},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_and_d,0xA2,NULL,NULL,NULL,AND_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_and_e,0xA3,NULL,NULL,NULL,AND_E},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_and_h,0xA4,NULL,NULL,NULL,AND_H},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_and_l,0xA5,NULL,NULL,NULL,AND_L},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_and_ptr_hl,0xA6,NULL,NULL,NULL,AND_PTR_HL},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_and_ptr_ix_plus_d,0xDDA6,operand_n,NULL,NULL,AND_PTR_IX_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_and_ptr_iy_plus_d,0xFDA6,operand_n,NULL,NULL,AND_PTR_IY_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_and_n,0xE6,operand_n,NULL,NULL,AND_N},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_xor_a,0xAF,NULL,NULL,NULL,XOR_A},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_xor_b,0xA8,NULL,NULL,NULL,XOR_B},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_xor_c,0xA9,NULL,NULL,NULL,XOR_C},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_xor_d,0xAA,NULL,NULL,NULL,XOR_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_xor_e,0xAB,NULL,NULL,NULL,XOR_E},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_xor_h,0xAC,NULL,NULL,NULL,XOR_H},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_xor_l,0xAD,NULL,NULL,NULL,XOR_L},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_xor_ptr_hl,0xAE,NULL,NULL,NULL,XOR_PTR_HL},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_xor_ptr_ix_plus_d,0xDDAE,operand_n,NULL,NULL,XOR_PTR_IX_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_xor_ptr_iy_plus_d,0xFDAE,operand_n,NULL,NULL,XOR_PTR_IY_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_xor_n,0xEE,operand_n,NULL,NULL,XOR_N},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_or_a,0xB7,NULL,NULL,NULL,OR_A},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_or_b,0xB0,NULL,NULL,NULL,OR_B},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_or_c,0xB1,NULL,NULL,NULL,OR_C},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_or_d,0xB2,NULL,NULL,NULL,OR_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_or_e,0xB3,NULL,NULL,NULL,OR_E},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_or_h,0xB4,NULL,NULL,NULL,OR_H},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_or_l,0xB5,NULL,NULL,NULL,OR_L},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_or_ptr_hl,0xB6,NULL,NULL,NULL,OR_PTR_HL},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_or_ptr_ix_plus_d,0xDDB6,operand_n,NULL,NULL,OR_PTR_IX_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_or_ptr_iy_plus_d,0xFDB6,operand_n,NULL,NULL,OR_PTR_IY_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_or_n,0xF6,operand_n,NULL,NULL,OR_N},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_cp_a,0xBF,NULL,NULL,NULL,CP_A},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_cp_b,0xB8,NULL,NULL,NULL,CP_B},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_cp_c,0xB9,NULL,NULL,NULL,CP_C},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_cp_d,0xBA,NULL,NULL,NULL,CP_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_cp_e,0xBB,NULL,NULL,NULL,CP_E},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_cp_h,0xBC,NULL,NULL,NULL,CP_H},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_cp_l,0xBD,NULL,NULL,NULL,CP_L},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_cp_ptr_hl,0xBE,NULL,NULL,NULL,CP_PTR_HL},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_cp_ptr_ix_plus_d,0xDDBE,operand_n,NULL,NULL,CP_PTR_IX_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_cp_ptr_iy_plus_d,0xFDBE,operand_n,NULL,NULL,CP_PTR_IY_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_cp_n,0xFE,operand_n,NULL,NULL,CP_N},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_inc_a,0x3C,NULL,NULL,NULL,INC_A},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_inc_b,0x04,NULL,NULL,NULL,INC_B},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_inc_c,0x0C,NULL,NULL,NULL,INC_C},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_inc_d,0x14,NULL,NULL,NULL,INC_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_inc_e,0x1C,NULL,NULL,NULL,INC_E},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_inc_h,0x24,NULL,NULL,NULL,INC_H},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_inc_l,0x2C,NULL,NULL,NULL,INC_L},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_inc_ptr_hl,0x34,NULL,NULL,NULL,INC_PTR_HL},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_inc_ptr_ix_plus_d,0xDD34,operand_n,NULL,NULL,INC_PTR_IX_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_inc_ptr_iy_plus_d,0xFD34,operand_n,NULL,NULL,INC_PTR_IY_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_dec_a,0x3D,NULL,NULL,NULL,DEC_A},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_dec_b,0x05,NULL,NULL,NULL,DEC_B},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_dec_c,0x0D,NULL,NULL,NULL,DEC_C},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_dec_d,0x15,NULL,NULL,NULL,DEC_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_dec_e,0x1D,NULL,NULL,NULL,DEC_E},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_dec_h,0x25,NULL,NULL,NULL,DEC_H},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_dec_l,0x2D,NULL,NULL,NULL,DEC_L},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_dec_ptr_hl,0x35,NULL,NULL,NULL,DEC_PTR_HL},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_dec_ptr_ix_plus_d,0xDD35,operand_n,NULL,NULL,DEC_PTR_IX_PLUS_D},
  {GROUP_8BIT_ARITHMETIC_LOGIC,mnemonic_dec_ptr_iy_plus_d,0xFD35,operand_n,NULL,NULL,DEC_PTR_IY_PLUS_D},
  {GROUP_GENERAL_PURPOSE_AF_OPERATIONS,mnemonic_daa,0x27,NULL,NULL,NULL,DAA},
  {GROUP_GENERAL_PURPOSE_AF_OPERATIONS,mnemonic_cpl,0x2F,NULL,NULL,NULL,CPL},
  {GROUP_GENERAL_PURPOSE_AF_OPERATIONS,mnemonic_neg,0xED44,NULL,NULL,NULL,NEG},
  {GROUP_GENERAL_PURPOSE_AF_OPERATIONS,mnemonic_ccf,0x3F,NULL,NULL,NULL,CCF},
  {GROUP_GENERAL_PURPOSE_AF_OPERATIONS,mnemonic_scf,0x37,NULL,NULL,NULL,SCF},
  {GROUP_16BIT_ARITHMETIC,mnemonic_add_hl_bc,0x09,NULL,NULL,NULL,ADD_HL_BC},
  {GROUP_16BIT_ARITHMETIC,mnemonic_add_hl_de,0x19,NULL,NULL,NULL,ADD_HL_DE},
  {GROUP_16BIT_ARITHMETIC,mnemonic_add_hl_hl,0x29,NULL,NULL,NULL,ADD_HL_HL},
  {GROUP_16BIT_ARITHMETIC,mnemonic_add_hl_sp,0x39,NULL,NULL,NULL,ADD_HL_SP},
  {GROUP_16BIT_ARITHMETIC,mnemonic_add_ix_bc,0xDD09,NULL,NULL,NULL,ADD_IX_BC},
  {GROUP_16BIT_ARITHMETIC,mnemonic_add_ix_de,0xDD19,NULL,NULL,NULL,ADD_IX_DE},
  {GROUP_16BIT_ARITHMETIC,mnemonic_add_ix_sp,0xDD39,NULL,NULL,NULL,ADD_IX_SP},
  {GROUP_16BIT_ARITHMETIC,mnemonic_add_ix_ix,0xDD29,NULL,NULL,NULL,ADD_IX_IX},
  {GROUP_16BIT_ARITHMETIC,mnemonic_add_iy_bc,0xFD09,NULL,NULL,NULL,ADD_IY_BC},
  {GROUP_16BIT_ARITHMETIC,mnemonic_add_iy_de,0xFD19,NULL,NULL,NULL,ADD_IY_DE},
  {GROUP_16BIT_ARITHMETIC,mnemonic_add_iy_sp,0xFD39,NULL,NULL,NULL,ADD_IY_SP},
  {GROUP_16BIT_ARITHMETIC,mnemonic_add_iy_iy,0xFD29,NULL,NULL,NULL,ADD_IY_IY},
  {GROUP_16BIT_ARITHMETIC,mnemonic_adc_hl_bc,0xED4A,NULL,NULL,NULL,ADC_HL_BC},
  {GROUP_16BIT_ARITHMETIC,mnemonic_adc_hl_de,0xED5A,NULL,NULL,NULL,ADC_HL_DE},
  {GROUP_16BIT_ARITHMETIC,mnemonic_adc_hl_hl,0xED6A,NULL,NULL,NULL,ADC_HL_HL},
  {GROUP_16BIT_ARITHMETIC,mnemonic_adc_hl_sp,0xED7A,NULL,NULL,NULL,ADC_HL_SP},
  {GROUP_16BIT_ARITHMETIC,mnemonic_sbc_hl_bc,0xED42,NULL,NULL,NULL,SBC_HL_BC},
  {GROUP_16BIT_ARITHMETIC,mnemonic_sbc_hl_de,0xED52,NULL,NULL,NULL,SBC_HL_DE},
  {GROUP_16BIT_ARITHMETIC,mnemonic_sbc_hl_hl,0xED62,NULL,NULL,NULL,SBC_HL_HL},
  {GROUP_16BIT_ARITHMETIC,mnemonic_sbc_hl_sp,0xED72,NULL,NULL,NULL,SBC_HL_SP},
  {GROUP_16BIT_ARITHMETIC,mnemonic_inc_bc,0x03,NULL,NULL,NULL,INC_BC},
  {GROUP_16BIT_ARITHMETIC,mnemonic_inc_de,0x13,NULL,NULL,NULL,INC_DE},
  {GROUP_16BIT_ARITHMETIC,mnemonic_inc_hl,0x23,NULL,NULL,NULL,INC_HL},
  {GROUP_16BIT_ARITHMETIC,mnemonic_inc_sp,0x33,NULL,NULL,NULL,INC_SP},
  {GROUP_16BIT_ARITHMETIC,mnemonic_inc_ix,0xDD23,NULL,NULL,NULL,INC_IX},
  {GROUP_16BIT_ARITHMETIC,mnemonic_inc_iy,0xFD23,NULL,NULL,NULL,INC_IY},
  {GROUP_16BIT_ARITHMETIC,mnemonic_dec_bc,0x0B,NULL,NULL,NULL,DEC_BC},
  {GROUP_16BIT_ARITHMETIC,mnemonic_dec_de,0x1B,NULL,NULL,NULL,DEC_DE},
  {GROUP_16BIT_ARITHMETIC,mnemonic_dec_hl,0x2B,NULL,NULL,NULL,DEC_HL},
  {GROUP_16BIT_ARITHMETIC,mnemonic_dec_sp,0x3B,NULL,NULL,NULL,DEC_SP},
  {GROUP_16BIT_ARITHMETIC,mnemonic_dec_ix,0xDD2B,NULL,NULL,NULL,DEC_IX},
  {GROUP_16BIT_ARITHMETIC,mnemonic_dec_iy,0xFD2B,NULL,NULL,NULL,DEC_IY},
  {GROUP_ROTATES_SHIFTS,mnemonic_rlc_a,0xCB07,NULL,NULL,NULL,RLC_A},
  {GROUP_ROTATES_SHIFTS,mnemonic_rlc_b,0xCB00,NULL,NULL,NULL,RLC_B},
  {GROUP_ROTATES_SHIFTS,mnemonic_rlc_c,0xCB01,NULL,NULL,NULL,RLC_C},
  {GROUP_ROTATES_SHIFTS,mnemonic_rlc_d,0xCB02,NULL,NULL,NULL,RLC_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_rlc_e,0xCB03,NULL,NULL,NULL,RLC_E},
  {GROUP_ROTATES_SHIFTS,mnemonic_rlc_h,0xCB04,NULL,NULL,NULL,RLC_H},
  {GROUP_ROTATES_SHIFTS,mnemonic_rlc_l,0xCB05,NULL,NULL,NULL,RLC_L},
  {GROUP_ROTATES_SHIFTS,mnemonic_rlc_ptr_hl,0xCB06,NULL,NULL,NULL,RLC_PTR_HL},
  {GROUP_ROTATES_SHIFTS,mnemonic_rlc_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x06,RLC_PTR_IX_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_rlc_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x06,RLC_PTR_IY_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_rrc_a,0xCB0F,NULL,NULL,NULL,RRC_A},
  {GROUP_ROTATES_SHIFTS,mnemonic_rrc_b,0xCB08,NULL,NULL,NULL,RRC_B},
  {GROUP_ROTATES_SHIFTS,mnemonic_rrc_c,0xCB09,NULL,NULL,NULL,RRC_C},
  {GROUP_ROTATES_SHIFTS,mnemonic_rrc_d,0xCB0A,NULL,NULL,NULL,RRC_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_rrc_e,0xCB0B,NULL,NULL,NULL,RRC_E},
  {GROUP_ROTATES_SHIFTS,mnemonic_rrc_h,0xCB0C,NULL,NULL,NULL,RRC_H},
  {GROUP_ROTATES_SHIFTS,mnemonic_rrc_l,0xCB0D,NULL,NULL,NULL,RRC_L},
  {GROUP_ROTATES_SHIFTS,mnemonic_rrc_ptr_hl,0xCB0E,NULL,NULL,NULL,RRC_PTR_HL},
  {GROUP_ROTATES_SHIFTS,mnemonic_rrc_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x0E,RRC_PTR_IX_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_rrc_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x0E,RRC_PTR_IY_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_rl_a,0xCB17,NULL,NULL,NULL,RL_A},
  {GROUP_ROTATES_SHIFTS,mnemonic_rl_b,0xCB10,NULL,NULL,NULL,RL_B},
  {GROUP_ROTATES_SHIFTS,mnemonic_rl_c,0xCB11,NULL,NULL,NULL,RL_C},
  {GROUP_ROTATES_SHIFTS,mnemonic_rl_d,0xCB12,NULL,NULL,NULL,RL_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_rl_e,0xCB13,NULL,NULL,NULL,RL_E},
  {GROUP_ROTATES_SHIFTS,mnemonic_rl_h,0xCB14,NULL,NULL,NULL,RL_H},
  {GROUP_ROTATES_SHIFTS,mnemonic_rl_l,0xCB15,NULL,NULL,NULL,RL_L},
  {GROUP_ROTATES_SHIFTS,mnemonic_rl_ptr_hl,0xCB16,NULL,NULL,NULL,RL_PTR_HL},
  {GROUP_ROTATES_SHIFTS,mnemonic_rl_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x16,RL_PTR_IX_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_rl_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x16,RL_PTR_IY_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_rr_a,0xCB1F,NULL,NULL,NULL,RR_A},
  {GROUP_ROTATES_SHIFTS,mnemonic_rr_b,0xCB18,NULL,NULL,NULL,RR_B},
  {GROUP_ROTATES_SHIFTS,mnemonic_rr_c,0xCB19,NULL,NULL,NULL,RR_C},
  {GROUP_ROTATES_SHIFTS,mnemonic_rr_d,0xCB1A,NULL,NULL,NULL,RR_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_rr_e,0xCB1B,NULL,NULL,NULL,RR_E},
  {GROUP_ROTATES_SHIFTS,mnemonic_rr_h,0xCB1C,NULL,NULL,NULL,RR_H},
  {GROUP_ROTATES_SHIFTS,mnemonic_rr_l,0xCB1D,NULL,NULL,NULL,RR_L},
  {GROUP_ROTATES_SHIFTS,mnemonic_rr_ptr_hl,0xCB1E,NULL,NULL,NULL,RR_PTR_HL},
  {GROUP_ROTATES_SHIFTS,mnemonic_rr_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x1E,RR_PTR_IX_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_rr_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x1E,RR_PTR_IY_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_sla_a,0xCB27,NULL,NULL,NULL,SLA_A},
  {GROUP_ROTATES_SHIFTS,mnemonic_sla_b,0xCB20,NULL,NULL,NULL,SLA_B},
  {GROUP_ROTATES_SHIFTS,mnemonic_sla_c,0xCB21,NULL,NULL,NULL,SLA_C},
  {GROUP_ROTATES_SHIFTS,mnemonic_sla_d,0xCB22,NULL,NULL,NULL,SLA_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_sla_e,0xCB23,NULL,NULL,NULL,SLA_E},
  {GROUP_ROTATES_SHIFTS,mnemonic_sla_h,0xCB24,NULL,NULL,NULL,SLA_H},
  {GROUP_ROTATES_SHIFTS,mnemonic_sla_l,0xCB25,NULL,NULL,NULL,SLA_L},
  {GROUP_ROTATES_SHIFTS,mnemonic_sla_ptr_hl,0xCB26,NULL,NULL,NULL,SLA_PTR_HL},
  {GROUP_ROTATES_SHIFTS,mnemonic_sla_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x26,SLA_PTR_IX_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_sla_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x26,SLA_PTR_IY_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_sra_a,0xCB2F,NULL,NULL,NULL,SRA_A},
  {GROUP_ROTATES_SHIFTS,mnemonic_sra_b,0xCB28,NULL,NULL,NULL,SRA_B},
  {GROUP_ROTATES_SHIFTS,mnemonic_sra_c,0xCB29,NULL,NULL,NULL,SRA_C},
  {GROUP_ROTATES_SHIFTS,mnemonic_sra_d,0xCB2A,NULL,NULL,NULL,SRA_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_sra_e,0xCB2B,NULL,NULL,NULL,SRA_E},
  {GROUP_ROTATES_SHIFTS,mnemonic_sra_h,0xCB2C,NULL,NULL,NULL,SRA_H},
  {GROUP_ROTATES_SHIFTS,mnemonic_sra_l,0xCB2D,NULL,NULL,NULL,SRA_L},
  {GROUP_ROTATES_SHIFTS,mnemonic_sra_ptr_hl,0xCB2E,NULL,NULL,NULL,SRA_PTR_HL},
  {GROUP_ROTATES_SHIFTS,mnemonic_sra_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x2E,SRA_PTR_IX_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_sra_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x2E,SRA_PTR_IY_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_sll_a,0xCB37,NULL,NULL,NULL,SLL_A},
  {GROUP_ROTATES_SHIFTS,mnemonic_sll_b,0xCB30,NULL,NULL,NULL,SLL_B},
  {GROUP_ROTATES_SHIFTS,mnemonic_sll_c,0xCB31,NULL,NULL,NULL,SLL_C},
  {GROUP_ROTATES_SHIFTS,mnemonic_sll_d,0xCB32,NULL,NULL,NULL,SLL_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_sll_e,0xCB33,NULL,NULL,NULL,SLL_E},
  {GROUP_ROTATES_SHIFTS,mnemonic_sll_h,0xCB34,NULL,NULL,NULL,SLL_H},
  {GROUP_ROTATES_SHIFTS,mnemonic_sll_l,0xCB35,NULL,NULL,NULL,SLL_L},
  {GROUP_ROTATES_SHIFTS,mnemonic_sll_ptr_hl,0xCB36,NULL,NULL,NULL,SLL_PTR_HL},
  {GROUP_ROTATES_SHIFTS,mnemonic_sll_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x36,SLL_PTR_IX_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_sll_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x36,SLL_PTR_IY_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_srl_a,0xCB3F,NULL,NULL,NULL,SRL_A},
  {GROUP_ROTATES_SHIFTS,mnemonic_srl_b,0xCB38,NULL,NULL,NULL,SRL_B},
  {GROUP_ROTATES_SHIFTS,mnemonic_srl_c,0xCB39,NULL,NULL,NULL,SRL_C},
  {GROUP_ROTATES_SHIFTS,mnemonic_srl_d,0xCB3A,NULL,NULL,NULL,SRL_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_srl_e,0xCB3B,NULL,NULL,NULL,SRL_E},
  {GROUP_ROTATES_SHIFTS,mnemonic_srl_h,0xCB3C,NULL,NULL,NULL,SRL_H},
  {GROUP_ROTATES_SHIFTS,mnemonic_srl_l,0xCB3D,NULL,NULL,NULL,SRL_L},
  {GROUP_ROTATES_SHIFTS,mnemonic_srl_ptr_hl,0xCB3E,NULL,NULL,NULL,SRL_PTR_HL},
  {GROUP_ROTATES_SHIFTS,mnemonic_srl_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x3E,SRL_PTR_IX_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_srl_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x3E,SRL_PTR_IY_PLUS_D},
  {GROUP_ROTATES_SHIFTS,mnemonic_rld,0xED6F,NULL,NULL,NULL,RLD},
  {GROUP_ROTATES_SHIFTS,mnemonic_rrd,0xED67,NULL,NULL,NULL,RRD},
  {GROUP_ROTATES_SHIFTS,mnemonic_rlca,0x07,NULL,NULL,NULL,RLCA},
  {GROUP_ROTATES_SHIFTS,mnemonic_rrca,0x0F,NULL,NULL,NULL,RRCA},
  {GROUP_ROTATES_SHIFTS,mnemonic_rla,0x17,NULL,NULL,NULL,RLA},
  {GROUP_ROTATES_SHIFTS,mnemonic_rra,0x1F,NULL,NULL,NULL,RRA},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_0_a,0xCB47,NULL,NULL,NULL,BIT_0_A},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_0_b,0xCB40,NULL,NULL,NULL,BIT_0_B},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_0_c,0xCB41,NULL,NULL,NULL,BIT_0_C},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_0_d,0xCB42,NULL,NULL,NULL,BIT_0_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_0_e,0xCB43,NULL,NULL,NULL,BIT_0_E},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_0_h,0xCB44,NULL,NULL,NULL,BIT_0_H},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_0_l,0xCB45,NULL,NULL,NULL,BIT_0_L},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_0_ptr_hl,0xCB46,NULL,NULL,NULL,BIT_0_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_0_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x46,BIT_0_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_0_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x46,BIT_0_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_1_a,0xCB4F,NULL,NULL,NULL,BIT_1_A},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_1_b,0xCB48,NULL,NULL,NULL,BIT_1_B},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_1_c,0xCB49,NULL,NULL,NULL,BIT_1_C},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_1_d,0xCB4A,NULL,NULL,NULL,BIT_1_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_1_e,0xCB4B,NULL,NULL,NULL,BIT_1_E},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_1_h,0xCB4C,NULL,NULL,NULL,BIT_1_H},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_1_l,0xCB4D,NULL,NULL,NULL,BIT_1_L},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_1_ptr_hl,0xCB4E,NULL,NULL,NULL,BIT_1_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_1_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x4E,BIT_1_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_1_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x4E,BIT_1_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_2_a,0xCB57,NULL,NULL,NULL,BIT_2_A},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_2_b,0xCB50,NULL,NULL,NULL,BIT_2_B},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_2_c,0xCB51,NULL,NULL,NULL,BIT_2_C},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_2_d,0xCB52,NULL,NULL,NULL,BIT_2_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_2_e,0xCB53,NULL,NULL,NULL,BIT_2_E},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_2_h,0xCB54,NULL,NULL,NULL,BIT_2_H},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_2_l,0xCB55,NULL,NULL,NULL,BIT_2_L},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_2_ptr_hl,0xCB56,NULL,NULL,NULL,BIT_2_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_2_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x56,BIT_2_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_2_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x56,BIT_2_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_3_a,0xCB5F,NULL,NULL,NULL,BIT_3_A},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_3_b,0xCB58,NULL,NULL,NULL,BIT_3_B},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_3_c,0xCB59,NULL,NULL,NULL,BIT_3_C},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_3_d,0xCB5A,NULL,NULL,NULL,BIT_3_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_3_e,0xCB5B,NULL,NULL,NULL,BIT_3_E},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_3_h,0xCB5C,NULL,NULL,NULL,BIT_3_H},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_3_l,0xCB5D,NULL,NULL,NULL,BIT_3_L},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_3_ptr_hl,0xCB5E,NULL,NULL,NULL,BIT_3_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_3_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x5E,BIT_3_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_3_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x5E,BIT_3_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_4_a,0xCB67,NULL,NULL,NULL,BIT_4_A},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_4_b,0xCB60,NULL,NULL,NULL,BIT_4_B},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_4_c,0xCB61,NULL,NULL,NULL,BIT_4_C},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_4_d,0xCB62,NULL,NULL,NULL,BIT_4_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_4_e,0xCB63,NULL,NULL,NULL,BIT_4_E},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_4_h,0xCB64,NULL,NULL,NULL,BIT_4_H},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_4_l,0xCB65,NULL,NULL,NULL,BIT_4_L},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_4_ptr_hl,0xCB66,NULL,NULL,NULL,BIT_4_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_4_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x66,BIT_4_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_4_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x66,BIT_4_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_5_a,0xCB6F,NULL,NULL,NULL,BIT_5_A},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_5_b,0xCB68,NULL,NULL,NULL,BIT_5_B},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_5_c,0xCB69,NULL,NULL,NULL,BIT_5_C},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_5_d,0xCB6A,NULL,NULL,NULL,BIT_5_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_5_e,0xCB6B,NULL,NULL,NULL,BIT_5_E},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_5_h,0xCB6C,NULL,NULL,NULL,BIT_5_H},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_5_l,0xCB6D,NULL,NULL,NULL,BIT_5_L},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_5_ptr_hl,0xCB6E,NULL,NULL,NULL,BIT_5_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_5_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x6E,BIT_5_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_5_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x6E,BIT_5_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_6_a,0xCB77,NULL,NULL,NULL,BIT_6_A},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_6_b,0xCB70,NULL,NULL,NULL,BIT_6_B},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_6_c,0xCB71,NULL,NULL,NULL,BIT_6_C},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_6_d,0xCB72,NULL,NULL,NULL,BIT_6_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_6_e,0xCB73,NULL,NULL,NULL,BIT_6_E},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_6_h,0xCB74,NULL,NULL,NULL,BIT_6_H},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_6_l,0xCB75,NULL,NULL,NULL,BIT_6_L},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_6_ptr_hl,0xCB76,NULL,NULL,NULL,BIT_6_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_6_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x76,BIT_6_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_6_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x76,BIT_6_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_7_a,0xCB7F,NULL,NULL,NULL,BIT_7_A},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_7_b,0xCB78,NULL,NULL,NULL,BIT_7_B},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_7_c,0xCB79,NULL,NULL,NULL,BIT_7_C},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_7_d,0xCB7A,NULL,NULL,NULL,BIT_7_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_7_e,0xCB7B,NULL,NULL,NULL,BIT_7_E},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_7_h,0xCB7C,NULL,NULL,NULL,BIT_7_H},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_7_l,0xCB7D,NULL,NULL,NULL,BIT_7_L},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_7_ptr_hl,0xCB7E,NULL,NULL,NULL,BIT_7_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_7_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x7E,BIT_7_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_bit_7_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x7E,BIT_7_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_0_a,0xCB87,NULL,NULL,NULL,RES_0_A},
  {GROUP_BIT_MANIPULATION,mnemonic_res_0_b,0xCB80,NULL,NULL,NULL,RES_0_B},
  {GROUP_BIT_MANIPULATION,mnemonic_res_0_c,0xCB81,NULL,NULL,NULL,RES_0_C},
  {GROUP_BIT_MANIPULATION,mnemonic_res_0_d,0xCB82,NULL,NULL,NULL,RES_0_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_0_e,0xCB83,NULL,NULL,NULL,RES_0_E},
  {GROUP_BIT_MANIPULATION,mnemonic_res_0_h,0xCB84,NULL,NULL,NULL,RES_0_H},
  {GROUP_BIT_MANIPULATION,mnemonic_res_0_l,0xCB85,NULL,NULL,NULL,RES_0_L},
  {GROUP_BIT_MANIPULATION,mnemonic_res_0_ptr_hl,0xCB86,NULL,NULL,NULL,RES_0_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_res_0_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x86,RES_0_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_0_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x86,RES_0_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_1_a,0xCB8F,NULL,NULL,NULL,RES_1_A},
  {GROUP_BIT_MANIPULATION,mnemonic_res_1_b,0xCB88,NULL,NULL,NULL,RES_1_B},
  {GROUP_BIT_MANIPULATION,mnemonic_res_1_c,0xCB89,NULL,NULL,NULL,RES_1_C},
  {GROUP_BIT_MANIPULATION,mnemonic_res_1_d,0xCB8A,NULL,NULL,NULL,RES_1_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_1_e,0xCB8B,NULL,NULL,NULL,RES_1_E},
  {GROUP_BIT_MANIPULATION,mnemonic_res_1_h,0xCB8C,NULL,NULL,NULL,RES_1_H},
  {GROUP_BIT_MANIPULATION,mnemonic_res_1_l,0xCB8D,NULL,NULL,NULL,RES_1_L},
  {GROUP_BIT_MANIPULATION,mnemonic_res_1_ptr_hl,0xCB8E,NULL,NULL,NULL,RES_1_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_res_1_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x8E,RES_1_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_1_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x8E,RES_1_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_2_a,0xCB97,NULL,NULL,NULL,RES_2_A},
  {GROUP_BIT_MANIPULATION,mnemonic_res_2_b,0xCB90,NULL,NULL,NULL,RES_2_B},
  {GROUP_BIT_MANIPULATION,mnemonic_res_2_c,0xCB91,NULL,NULL,NULL,RES_2_C},
  {GROUP_BIT_MANIPULATION,mnemonic_res_2_d,0xCB92,NULL,NULL,NULL,RES_2_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_2_e,0xCB93,NULL,NULL,NULL,RES_2_E},
  {GROUP_BIT_MANIPULATION,mnemonic_res_2_h,0xCB94,NULL,NULL,NULL,RES_2_H},
  {GROUP_BIT_MANIPULATION,mnemonic_res_2_l,0xCB95,NULL,NULL,NULL,RES_2_L},
  {GROUP_BIT_MANIPULATION,mnemonic_res_2_ptr_hl,0xCB96,NULL,NULL,NULL,RES_2_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_res_2_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x96,RES_2_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_2_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x96,RES_2_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_3_a,0xCB9F,NULL,NULL,NULL,RES_3_A},
  {GROUP_BIT_MANIPULATION,mnemonic_res_3_b,0xCB98,NULL,NULL,NULL,RES_3_B},
  {GROUP_BIT_MANIPULATION,mnemonic_res_3_c,0xCB99,NULL,NULL,NULL,RES_3_C},
  {GROUP_BIT_MANIPULATION,mnemonic_res_3_d,0xCB9A,NULL,NULL,NULL,RES_3_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_3_e,0xCB9B,NULL,NULL,NULL,RES_3_E},
  {GROUP_BIT_MANIPULATION,mnemonic_res_3_h,0xCB9C,NULL,NULL,NULL,RES_3_H},
  {GROUP_BIT_MANIPULATION,mnemonic_res_3_l,0xCB9D,NULL,NULL,NULL,RES_3_L},
  {GROUP_BIT_MANIPULATION,mnemonic_res_3_ptr_hl,0xCB9E,NULL,NULL,NULL,RES_3_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_res_3_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0x9E,RES_3_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_3_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0x9E,RES_3_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_4_a,0xCBA7,NULL,NULL,NULL,RES_4_A},
  {GROUP_BIT_MANIPULATION,mnemonic_res_4_b,0xCBA0,NULL,NULL,NULL,RES_4_B},
  {GROUP_BIT_MANIPULATION,mnemonic_res_4_c,0xCBA1,NULL,NULL,NULL,RES_4_C},
  {GROUP_BIT_MANIPULATION,mnemonic_res_4_d,0xCBA2,NULL,NULL,NULL,RES_4_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_4_e,0xCBA3,NULL,NULL,NULL,RES_4_E},
  {GROUP_BIT_MANIPULATION,mnemonic_res_4_h,0xCBA4,NULL,NULL,NULL,RES_4_H},
  {GROUP_BIT_MANIPULATION,mnemonic_res_4_l,0xCBA5,NULL,NULL,NULL,RES_4_L},
  {GROUP_BIT_MANIPULATION,mnemonic_res_4_ptr_hl,0xCBA6,NULL,NULL,NULL,RES_4_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_res_4_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0xA6,RES_4_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_4_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0xA6,RES_4_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_5_a,0xCBAF,NULL,NULL,NULL,RES_5_A},
  {GROUP_BIT_MANIPULATION,mnemonic_res_5_b,0xCBA8,NULL,NULL,NULL,RES_5_B},
  {GROUP_BIT_MANIPULATION,mnemonic_res_5_c,0xCBA9,NULL,NULL,NULL,RES_5_C},
  {GROUP_BIT_MANIPULATION,mnemonic_res_5_d,0xCBAA,NULL,NULL,NULL,RES_5_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_5_e,0xCBAB,NULL,NULL,NULL,RES_5_E},
  {GROUP_BIT_MANIPULATION,mnemonic_res_5_h,0xCBAC,NULL,NULL,NULL,RES_5_H},
  {GROUP_BIT_MANIPULATION,mnemonic_res_5_l,0xCBAD,NULL,NULL,NULL,RES_5_L},
  {GROUP_BIT_MANIPULATION,mnemonic_res_5_ptr_hl,0xCBAE,NULL,NULL,NULL,RES_5_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_res_5_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0xAE,RES_5_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_5_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0xAE,RES_5_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_6_a,0xCBB7,NULL,NULL,NULL,RES_6_A},
  {GROUP_BIT_MANIPULATION,mnemonic_res_6_b,0xCBB0,NULL,NULL,NULL,RES_6_B},
  {GROUP_BIT_MANIPULATION,mnemonic_res_6_c,0xCBB1,NULL,NULL,NULL,RES_6_C},
  {GROUP_BIT_MANIPULATION,mnemonic_res_6_d,0xCBB2,NULL,NULL,NULL,RES_6_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_6_e,0xCBB3,NULL,NULL,NULL,RES_6_E},
  {GROUP_BIT_MANIPULATION,mnemonic_res_6_h,0xCBB4,NULL,NULL,NULL,RES_6_H},
  {GROUP_BIT_MANIPULATION,mnemonic_res_6_l,0xCBB5,NULL,NULL,NULL,RES_6_L},
  {GROUP_BIT_MANIPULATION,mnemonic_res_6_ptr_hl,0xCBB6,NULL,NULL,NULL,RES_6_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_res_6_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0xB6,RES_6_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_6_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0xB6,RES_6_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_7_a,0xCBBF,NULL,NULL,NULL,RES_7_A},
  {GROUP_BIT_MANIPULATION,mnemonic_res_7_b,0xCBB8,NULL,NULL,NULL,RES_7_B},
  {GROUP_BIT_MANIPULATION,mnemonic_res_7_c,0xCBB9,NULL,NULL,NULL,RES_7_C},
  {GROUP_BIT_MANIPULATION,mnemonic_res_7_d,0xCBBA,NULL,NULL,NULL,RES_7_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_7_e,0xCBBB,NULL,NULL,NULL,RES_7_E},
  {GROUP_BIT_MANIPULATION,mnemonic_res_7_h,0xCBBC,NULL,NULL,NULL,RES_7_H},
  {GROUP_BIT_MANIPULATION,mnemonic_res_7_l,0xCBBD,NULL,NULL,NULL,RES_7_L},
  {GROUP_BIT_MANIPULATION,mnemonic_res_7_ptr_hl,0xCBBE,NULL,NULL,NULL,RES_7_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_res_7_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0xBE,RES_7_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_res_7_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0xBE,RES_7_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_0_a,0xCBC7,NULL,NULL,NULL,SET_0_A},
  {GROUP_BIT_MANIPULATION,mnemonic_set_0_b,0xCBC0,NULL,NULL,NULL,SET_0_B},
  {GROUP_BIT_MANIPULATION,mnemonic_set_0_c,0xCBC1,NULL,NULL,NULL,SET_0_C},
  {GROUP_BIT_MANIPULATION,mnemonic_set_0_d,0xCBC2,NULL,NULL,NULL,SET_0_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_0_e,0xCBC3,NULL,NULL,NULL,SET_0_E},
  {GROUP_BIT_MANIPULATION,mnemonic_set_0_h,0xCBC4,NULL,NULL,NULL,SET_0_H},
  {GROUP_BIT_MANIPULATION,mnemonic_set_0_l,0xCBC5,NULL,NULL,NULL,SET_0_L},
  {GROUP_BIT_MANIPULATION,mnemonic_set_0_ptr_hl,0xCBC6,NULL,NULL,NULL,SET_0_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_set_0_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0xC6,SET_0_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_0_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0xC6,SET_0_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_1_a,0xCBCF,NULL,NULL,NULL,SET_1_A},
  {GROUP_BIT_MANIPULATION,mnemonic_set_1_b,0xCBC8,NULL,NULL,NULL,SET_1_B},
  {GROUP_BIT_MANIPULATION,mnemonic_set_1_c,0xCBC9,NULL,NULL,NULL,SET_1_C},
  {GROUP_BIT_MANIPULATION,mnemonic_set_1_d,0xCBCA,NULL,NULL,NULL,SET_1_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_1_e,0xCBCB,NULL,NULL,NULL,SET_1_E},
  {GROUP_BIT_MANIPULATION,mnemonic_set_1_h,0xCBCC,NULL,NULL,NULL,SET_1_H},
  {GROUP_BIT_MANIPULATION,mnemonic_set_1_l,0xCBCD,NULL,NULL,NULL,SET_1_L},
  {GROUP_BIT_MANIPULATION,mnemonic_set_1_ptr_hl,0xCBCE,NULL,NULL,NULL,SET_1_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_set_1_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0xDE,SET_1_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_1_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0xDE,SET_1_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_2_a,0xCBD7,NULL,NULL,NULL,SET_2_A},
  {GROUP_BIT_MANIPULATION,mnemonic_set_2_b,0xCBD0,NULL,NULL,NULL,SET_2_B},
  {GROUP_BIT_MANIPULATION,mnemonic_set_2_c,0xCBD1,NULL,NULL,NULL,SET_2_C},
  {GROUP_BIT_MANIPULATION,mnemonic_set_2_d,0xCBD2,NULL,NULL,NULL,SET_2_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_2_e,0xCBD3,NULL,NULL,NULL,SET_2_E},
  {GROUP_BIT_MANIPULATION,mnemonic_set_2_h,0xCBD4,NULL,NULL,NULL,SET_2_H},
  {GROUP_BIT_MANIPULATION,mnemonic_set_2_l,0xCBD5,NULL,NULL,NULL,SET_2_L},
  {GROUP_BIT_MANIPULATION,mnemonic_set_2_ptr_hl,0xCBD6,NULL,NULL,NULL,SET_2_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_set_2_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0xD6,SET_2_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_2_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0xD6,SET_2_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_3_a,0xCBDF,NULL,NULL,NULL,SET_3_A},
  {GROUP_BIT_MANIPULATION,mnemonic_set_3_b,0xCBD8,NULL,NULL,NULL,SET_3_B},
  {GROUP_BIT_MANIPULATION,mnemonic_set_3_c,0xCBD9,NULL,NULL,NULL,SET_3_C},
  {GROUP_BIT_MANIPULATION,mnemonic_set_3_d,0xCBDA,NULL,NULL,NULL,SET_3_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_3_e,0xCBDB,NULL,NULL,NULL,SET_3_E},
  {GROUP_BIT_MANIPULATION,mnemonic_set_3_h,0xCBDC,NULL,NULL,NULL,SET_3_H},
  {GROUP_BIT_MANIPULATION,mnemonic_set_3_l,0xCBDD,NULL,NULL,NULL,SET_3_L},
  {GROUP_BIT_MANIPULATION,mnemonic_set_3_ptr_hl,0xCBDE,NULL,NULL,NULL,SET_3_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_set_3_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0xCE,SET_3_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_3_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0xCE,SET_3_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_4_a,0xCBE7,NULL,NULL,NULL,SET_4_A},
  {GROUP_BIT_MANIPULATION,mnemonic_set_4_b,0xCBE0,NULL,NULL,NULL,SET_4_B},
  {GROUP_BIT_MANIPULATION,mnemonic_set_4_c,0xCBE1,NULL,NULL,NULL,SET_4_C},
  {GROUP_BIT_MANIPULATION,mnemonic_set_4_d,0xCBE2,NULL,NULL,NULL,SET_4_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_4_e,0xCBE3,NULL,NULL,NULL,SET_4_E},
  {GROUP_BIT_MANIPULATION,mnemonic_set_4_h,0xCBE4,NULL,NULL,NULL,SET_4_H},
  {GROUP_BIT_MANIPULATION,mnemonic_set_4_l,0xCBE5,NULL,NULL,NULL,SET_4_L},
  {GROUP_BIT_MANIPULATION,mnemonic_set_4_ptr_hl,0xCBE6,NULL,NULL,NULL,SET_4_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_set_4_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0xE6,SET_4_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_4_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0xE6,SET_4_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_5_a,0xCBEF,NULL,NULL,NULL,SET_5_A},
  {GROUP_BIT_MANIPULATION,mnemonic_set_5_b,0xCBE8,NULL,NULL,NULL,SET_5_B},
  {GROUP_BIT_MANIPULATION,mnemonic_set_5_c,0xCBE9,NULL,NULL,NULL,SET_5_C},
  {GROUP_BIT_MANIPULATION,mnemonic_set_5_d,0xCBEA,NULL,NULL,NULL,SET_5_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_5_e,0xCBEB,NULL,NULL,NULL,SET_5_E},
  {GROUP_BIT_MANIPULATION,mnemonic_set_5_h,0xCBEC,NULL,NULL,NULL,SET_5_H},
  {GROUP_BIT_MANIPULATION,mnemonic_set_5_l,0xCBED,NULL,NULL,NULL,SET_5_L},
  {GROUP_BIT_MANIPULATION,mnemonic_set_5_ptr_hl,0xCBEE,NULL,NULL,NULL,SET_5_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_set_5_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0xEE,SET_5_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_5_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0xEE,SET_5_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_6_a,0xCBF7,NULL,NULL,NULL,SET_6_A},
  {GROUP_BIT_MANIPULATION,mnemonic_set_6_b,0xCBF0,NULL,NULL,NULL,SET_6_B},
  {GROUP_BIT_MANIPULATION,mnemonic_set_6_c,0xCBF1,NULL,NULL,NULL,SET_6_C},
  {GROUP_BIT_MANIPULATION,mnemonic_set_6_d,0xCBF2,NULL,NULL,NULL,SET_6_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_6_e,0xCBF3,NULL,NULL,NULL,SET_6_E},
  {GROUP_BIT_MANIPULATION,mnemonic_set_6_h,0xCBF4,NULL,NULL,NULL,SET_6_H},
  {GROUP_BIT_MANIPULATION,mnemonic_set_6_l,0xCBF5,NULL,NULL,NULL,SET_6_L},
  {GROUP_BIT_MANIPULATION,mnemonic_set_6_ptr_hl,0xCBF6,NULL,NULL,NULL,SET_6_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_set_6_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0xF6,SET_6_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_6_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0xF6,SET_6_PTR_IY_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_7_a,0xCBFF,NULL,NULL,NULL,SET_7_A},
  {GROUP_BIT_MANIPULATION,mnemonic_set_7_b,0xCBF8,NULL,NULL,NULL,SET_7_B},
  {GROUP_BIT_MANIPULATION,mnemonic_set_7_c,0xCBF9,NULL,NULL,NULL,SET_7_C},
  {GROUP_BIT_MANIPULATION,mnemonic_set_7_d,0xCBFA,NULL,NULL,NULL,SET_7_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_7_e,0xCBFB,NULL,NULL,NULL,SET_7_E},
  {GROUP_BIT_MANIPULATION,mnemonic_set_7_h,0xCBFC,NULL,NULL,NULL,SET_7_H},
  {GROUP_BIT_MANIPULATION,mnemonic_set_7_l,0xCBFD,NULL,NULL,NULL,SET_7_L},
  {GROUP_BIT_MANIPULATION,mnemonic_set_7_ptr_hl,0xCBFE,NULL,NULL,NULL,SET_7_PTR_HL},
  {GROUP_BIT_MANIPULATION,mnemonic_set_7_ptr_ix_plus_d,0xDDCB,operand_n,NULL,0xFE,SET_7_PTR_IX_PLUS_D},
  {GROUP_BIT_MANIPULATION,mnemonic_set_7_ptr_iy_plus_d,0xFDCB,operand_n,NULL,0xFE,SET_7_PTR_IY_PLUS_D},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jp_nn,0xC3,operand_n,"n",NULL,JP_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jp_c_nn,0xDA,operand_n,"n",NULL,JP_C_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jp_nc_nn,0xD2,operand_n,"n",NULL,JP_NC_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jp_z_nn,0xCA,operand_n,"n",NULL,JP_Z_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jp_nz_nn,0xC2,operand_n,"n",NULL,JP_NZ_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jp_pe_nn,0xEA,operand_n,"n",NULL,JP_PE_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jp_po_nn,0xE2,operand_n,"n",NULL,JP_PO_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jp_m_nn,0xFA,operand_n,"n",NULL,JP_M_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jp_p_nn,0xF2,operand_n,"n",NULL,JP_P_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jr_e,0x18,operand_n,NULL,NULL,JR_E},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jr_c_e,0x38,operand_n,NULL,NULL,JR_C_E},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jr_nc_e,0x30,operand_n,NULL,NULL,JR_NC_E},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jr_z_e,0x28,operand_n,NULL,NULL,JR_Z_E},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jr_nz_e,0x20,operand_n,NULL,NULL,JR_NZ_E},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jp_ptr_hl,0xE9,NULL,NULL,NULL,JP_PTR_HL},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jp_ptr_ix,0xDDE9,NULL,NULL,NULL,JP_PTR_IX},
  {GROUP_JUMP_CALL_RETURN,mnemonic_jp_ptr_iy,0xFDE9,NULL,NULL,NULL,JP_PTR_IY},
  {GROUP_JUMP_CALL_RETURN,mnemonic_call_nn,0xCD,operand_n,"n",NULL,CALL_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_call_c_nn,0xDC,operand_n,"n",NULL,CALL_C_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_call_nc_nn,0xD4,operand_n,"n",NULL,CALL_NC_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_call_z_nn,0xCC,operand_n,"n",NULL,CALL_Z_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_call_nz_nn,0xC4,operand_n,"n",NULL,CALL_NZ_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_call_pe_nn,0xEC,operand_n,"n",NULL,CALL_PE_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_call_po_nn,0xE4,operand_n,"n",NULL,CALL_PO_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_call_m_nn,0xFC,operand_n,"n",NULL,CALL_M_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_call_p_nn,0xF4,operand_n,"n",NULL,CALL_P_NN},
  {GROUP_JUMP_CALL_RETURN,mnemonic_djnz_e,0x10,operand_n,NULL,NULL,DJNZ_E},
  {GROUP_JUMP_CALL_RETURN,mnemonic_ret,0xC9,NULL,NULL,NULL,RET},
  {GROUP_JUMP_CALL_RETURN,mnemonic_ret_c,0xD8,NULL,NULL,NULL,RET_C},
  {GROUP_JUMP_CALL_RETURN,mnemonic_ret_nc,0xD0,NULL,NULL,NULL,RET_NC},
  {GROUP_JUMP_CALL_RETURN,mnemonic_ret_z,0xC8,NULL,NULL,NULL,RET_Z},
  {GROUP_JUMP_CALL_RETURN,mnemonic_ret_nz,0xC0,NULL,NULL,NULL,RET_NZ},
  {GROUP_JUMP_CALL_RETURN,mnemonic_ret_pe,0xE8,NULL,NULL,NULL,RET_PE},
  {GROUP_JUMP_CALL_RETURN,mnemonic_ret_po,0xE0,NULL,NULL,NULL,RET_PO},
  {GROUP_JUMP_CALL_RETURN,mnemonic_ret_m,0xF8,NULL,NULL,NULL,RET_M},
  {GROUP_JUMP_CALL_RETURN,mnemonic_ret_p,0xF0,NULL,NULL,NULL,RET_P},
  {GROUP_JUMP_CALL_RETURN,mnemonic_ret_i,0xED4D,NULL,NULL,NULL,RET_I},
  {GROUP_JUMP_CALL_RETURN,mnemonic_ret_n,0xED45,NULL,NULL,NULL,RET_N},
  {GROUP_RESTART,mnemonic_rst_0,0xC7,NULL,NULL,NULL,RST_0},
  {GROUP_RESTART,mnemonic_rst_8,0xCF,NULL,NULL,NULL,RST_8},
  {GROUP_RESTART,mnemonic_rst_16,0xD7,NULL,NULL,NULL,RST_16},
  {GROUP_RESTART,mnemonic_rst_24,0xDF,NULL,NULL,NULL,RST_24},
  {GROUP_RESTART,mnemonic_rst_32,0xE7,NULL,NULL,NULL,RST_32},
  {GROUP_RESTART,mnemonic_rst_40,0xEF,NULL,NULL,NULL,RST_40},
  {GROUP_RESTART,mnemonic_rst_48,0xF7,NULL,NULL,NULL,RST_48},
  {GROUP_RESTART,mnemonic_rst_56,0xFF,NULL,NULL,NULL,RST_56},
  {GROUP_INPUT,mnemonic_in_a_ptr_n,0xDB,operand_n,NULL,NULL,IN_A_PTR_N},
  {GROUP_INPUT,mnemonic_in_a_ptr_c,0xED78,NULL,NULL,NULL,IN_A_PTR_C},
  {GROUP_INPUT,mnemonic_in_b_ptr_c,0xED40,NULL,NULL,NULL,IN_B_PTR_C},
  {GROUP_INPUT,mnemonic_in_c_ptr_c,0xED48,NULL,NULL,NULL,IN_C_PTR_C},
  {GROUP_INPUT,mnemonic_in_d_ptr_c,0xED50,NULL,NULL,NULL,IN_D_PTR_C},
  {GROUP_INPUT,mnemonic_in_e_ptr_c,0xED58,NULL,NULL,NULL,IN_E_PTR_C},
  {GROUP_INPUT,mnemonic_in_h_ptr_c,0xED60,NULL,NULL,NULL,IN_H_PTR_C},
  {GROUP_INPUT,mnemonic_in_l_ptr_c,0xED68,NULL,NULL,NULL,IN_L_PTR_C},
  {GROUP_INPUT,mnemonic_ini,0xEDA2,NULL,NULL,NULL,INI},
  {GROUP_INPUT,mnemonic_inir,0xEDB2,NULL,NULL,NULL,INIR},
  {GROUP_INPUT,mnemonic_ind,0xEDAA,NULL,NULL,NULL,IND},
  {GROUP_INPUT,mnemonic_indr,0xEDBA,NULL,NULL,NULL,INDR},
  {GROUP_OUTPUT,mnemonic_out_ptr_n_a,0xD3,operand_n,NULL,NULL,OUT_PTR_N_A},
  {GROUP_OUTPUT,mnemonic_out_ptr_c_a,0xED79,NULL,NULL,NULL,OUT_PTR_C_A},
  {GROUP_OUTPUT,mnemonic_out_ptr_c_b,0xED41,NULL,NULL,NULL,OUT_PTR_C_B},
  {GROUP_OUTPUT,mnemonic_out_ptr_c_c,0xED49,NULL,NULL,NULL,OUT_PTR_C_C},
  {GROUP_OUTPUT,mnemonic_out_ptr_c_d,0xED51,NULL,NULL,NULL,OUT_PTR_C_D},
  {GROUP_OUTPUT,mnemonic_out_ptr_c_e,0xED59,NULL,NULL,NULL,OUT_PTR_C_E},
  {GROUP_OUTPUT,mnemonic_out_ptr_c_h,0xED61,NULL,NULL,NULL,OUT_PTR_C_H},
  {GROUP_OUTPUT,mnemonic_out_ptr_c_l,0xED69,NULL,NULL,NULL,OUT_PTR_C_L},
  {GROUP_OUTPUT,mnemonic_outi,0xEDA3,NULL,NULL,NULL,OUTI},
  {GROUP_OUTPUT,mnemonic_outir,0xEDB3,NULL,NULL,NULL,OUTIR},
  {GROUP_OUTPUT,mnemonic_outd,0xEDAB,NULL,NULL,NULL,OUTD},
  {GROUP_OUTPUT,mnemonic_outdr,0xEDBB,NULL,NULL,NULL,OUTDR},
  {GROUP_MISCELLANEOUS_CPU_CONTROL,mnemonic_halt,0x76,NULL,NULL,NULL,HALT_},
  {GROUP_MISCELLANEOUS_CPU_CONTROL,mnemonic_di,0xF3,NULL,NULL,NULL,DI},
  {GROUP_MISCELLANEOUS_CPU_CONTROL,mnemonic_ei,0xFB,NULL,NULL,NULL,EI},
  {GROUP_MISCELLANEOUS_CPU_CONTROL,mnemonic_im0,0xED46,NULL,NULL,NULL,IM0},
  {GROUP_MISCELLANEOUS_CPU_CONTROL,mnemonic_im1,0xED56,NULL,NULL,NULL,IM1},
  {GROUP_MISCELLANEOUS_CPU_CONTROL,mnemonic_im2,0xED5E,NULL,NULL,NULL,IM2}
};

const size_t numInstructions = sizeof(InstructionDefinitions) / sizeof(InstructionDefinitions[0]);

//
//Prototype functions
//

void printProgmemString(const char* progmemPtr);
void readLine(char* buffer, size_t bufferSize);

//
// General CPU interaction functions
//

//Read the current address on the address bus
uint16_t readAddressBus(int delayTime);

//Write to the data bus
void writeData(byte data);

//Write a single instruction with operands
void writeSingleInstruction(byte data[], int numM1Cycles, int numReadCycles, int numWriteCycles);

//
//Test functions
//


void runAllTests();
void runAllTestsForGroup(InstructionGroup group);
void runTest(const instructionDefinitionType& inst);
int findInstructionIndexByMnemonic(const char* targetMnemonic);

//Test NOP
//bool NOP();


//
//Menu functions
//

void showMainMenu();
void showGroupMenu(void (*callback)(InstructionGroup));
void showInstructionMenu(InstructionGroup group);




void setup() {
  //Start the serial console
  Serial.begin(115200);

  //
  //Set up pins
  //

  //Note that CPU inputs are outputs here and vice versa

  //Data Bus
  //These can be either input or output, initializing as input
  pinMode(D_0, INPUT);
  pinMode(D_1, INPUT);
  pinMode(D_2, INPUT);
  pinMode(D_3, INPUT);
  pinMode(D_4, INPUT);
  pinMode(D_5, INPUT);
  pinMode(D_6, INPUT);
  pinMode(D_7, INPUT);

  //Address Bus
  pinMode(A_0, INPUT);
  pinMode(A_1, INPUT);
  pinMode(A_2, INPUT);
  pinMode(A_3, INPUT);
  pinMode(A_4, INPUT);
  pinMode(A_5, INPUT);
  pinMode(A_6, INPUT);
  pinMode(A_7, INPUT);
  pinMode(A_8, INPUT);
  pinMode(A_9, INPUT);
  pinMode(A_10, INPUT);
  pinMode(A_11, INPUT);
  pinMode(A_12, INPUT);
  pinMode(A_13, INPUT);
  pinMode(A_14, INPUT);
  pinMode(A_15, INPUT);

  //System Control
  pinMode(M1, INPUT);
  pinMode(MREQ, INPUT);
  pinMode(IORQ, INPUT);
  pinMode(RD, INPUT);
  pinMode(WR, INPUT);
  pinMode(RFSH, INPUT);

  //CPU Control
  pinMode(HALT, INPUT);
  pinMode(WAIT, OUTPUT);
  pinMode(INT, OUTPUT);
  pinMode(NMI, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(CLK, OUTPUT);

  //CPU Bus Control
  pinMode(BUSRQ, OUTPUT);
  pinMode(BUSAK, INPUT);

  //
  //Set up the clock
  //

  //First clear the bits the timer uses for generating the frequency
  int eraser = 7;  //0b111
  TCCR4B &= ~eraser;
  //Next set the desired frequency using the prescaler
  // 1 = 31kHz
  // 2 = 4kHz
  // 3 = 490Hz (default value)
  // 4 = 120Hz
  // 5 = 30Hz
  int prescaler = 1;
  TCCR4B |= prescaler;

  //Set up the pin for PWM
  analogWrite(CLK, 128);

  //Set the CPU Control Pins
  digitalWriteFast(INT, HIGH);
  digitalWriteFast(NMI, HIGH);
  digitalWriteFast(BUSRQ, HIGH);

  //
  //Issue a reset to the CPU
  //
  digitalWriteFast(RESET, LOW);
  delay(10);
  digitalWriteFast(RESET, HIGH);

  //Show the menu
  while (!Serial)
    ;
  showMainMenu();
}


void loop() {
}

