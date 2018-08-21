#include <stdio.h>
#include <stdlib.h>
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
void cpu_load(struct cpu *cpu, char *filename)
{
  // Read instructions from a file provided by argv
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t linelen;

  fp = fopen(filename, "r");

  if (!fp) {
    printf("Failed to open file\n");
    exit(2);
  }

  int line_num = 0;
  while ((linelen = getline(&line, &len, fp)) != -1) {
    if (*line != '\n' && *line != '#') {
      line[8] = '\0';
      cpu_ram_write(cpu, cpu->PC + line_num, strtoul(line, NULL, 2));
      line_num++;
    }
  }

  fclose(fp);
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

