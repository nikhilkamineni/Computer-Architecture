#include <stdio.h>
#include "cpu.h"

/**
 * Read a value from ram from the address provided
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

/**
 * Write a given value to the ram address provided
 */
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  const int DATA_LEN = 6;
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // 1. Get the value of the current instruction (in address PC).
    /* unsigned char IR = cpu->ram[cpu->PC]; */
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);

    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);

    // 2. switch() over it to decide on a course of action.
    switch(IR) {
      // 3. Do whatever the instruction should do according to the spec.
      case NOP:
        continue;
      case LDI:
        cpu->reg[operandA] = operandB;
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
      case HLT:
        running = 0;
        break;
      default:
        printf("Did not recognize instruction %10x\n", IR);
    }
    // 4. Move the PC to the next instruction.
    // Check if instruction sets the PC using bit 4 of IR
    int instruction_sets_PC = (IR >> 4) & 0b00000001;

    if (!instruction_sets_PC)
      // Get the number of operands from the two highest bits of IR 
      // and increment PC that number of times plus one
      cpu->PC += ((IR >> 6) & 0b00000011) + 1;

  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;

  // TODO: Zero registers and RAM
}

