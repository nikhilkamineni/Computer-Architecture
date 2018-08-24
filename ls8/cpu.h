#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  unsigned char PC;       // Program Counter
  unsigned char IR;       // Instruction Register 0bAABCDDDD
  unsigned char MAR;      // Memory Address Register
  unsigned char MDR;      // Memory Data Register
  unsigned char FL;       // Flags Register 0b00000LGE
  unsigned char reg[8];   // registers (array)
  unsigned char ram[256]; // ram (array)
};

enum alu_op {
  ALU_ADD,
  ALU_SUB,
  ALU_MUL,
  ALU_DIV,
  ALU_MOD,
  ALU_INC,
  ALU_DEC,
  ALU_CMP,
  ALU_AND,
  ALU_NOT,
  ALU_OR,
  ALU_XOR,
  ALU_SHL,
  ALU_SHR
};

// Registers
#define IM 5
#define IS 6
#define SP 7

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.
#define ADD  0b10100000
#define SUB  0b10100001
#define MUL  0b10100010
#define DIV  0b10100011
#define MOD  0b10100100
#define INC  0b01100101
#define DEC  0b01100110
#define CMP  0b10100111
#define AND  0b10101000
#define NOT  0b01101001
#define OR   0b10101010
#define XOR  0b10101011
#define SHL  0b10101100
#define SHR  0b10101101
#define NOP  0b00000000
#define HLT  0b00000001
#define LDI  0b10000010
#define PRN  0b01000111
#define LD   0b10000011
#define ST   0b10000100
#define PUSH 0b01000101
#define POP  0b01000110
#define PRA  0b01001000
#define CALL 0b01010000
#define RET  0b00010001
#define ST   0b10000100
#define JMP  0b01010100
#define JEQ  0b01010101

// Function declarations
extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
