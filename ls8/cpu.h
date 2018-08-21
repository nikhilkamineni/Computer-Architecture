#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // PC
  unsigned char PC;
  // registers (array)
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[256];
};

enum alu_op {
  ALU_MUL
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define NOP 0b00000000
#define LDI 0b10000010
#define HLT 0b00000001
#define PRN 0b10001111
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
