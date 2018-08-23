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
  printf("%s\n", filename);
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
      /* cpu_ram_write(cpu, line_num, strtoul(line, NULL, 2)); */
      cpu->ram[line_num] = strtoul(line, NULL, 2);
      /* printf("%x\n", cpu->ram[line_num]); */
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
    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;
    case ALU_SUB:
      cpu->reg[regA] -= cpu->reg[regB];
      break;
    case ALU_MUL:
      cpu->reg[regA] *= cpu->reg[regB];
      break;
    case ALU_DIV:
      if (!cpu->reg[regB]) {
        fprintf(stderr, "Value of operandB is 0!");
        exit(1);
      }
      cpu->reg[regA] /= cpu->reg[regB];
      break;
    case ALU_MOD:
      if (!cpu->reg[regB]) {
        fprintf(stderr, "Value of operandB is 0!");
        exit(1);
      }
      cpu->reg[regA] %= cpu->reg[regB];
      break;
    case ALU_INC:
      cpu->reg[regA]++;
    case ALU_DEC:
      cpu->reg[regA]++;
    default:
      printf("There is no ALU instruction with that code");
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
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);

    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    /* printf("TRACE: IR: %x; opB: %x; opB: %x\n", IR, operandA, operandB); */

    // 2. switch() over it to decide on a course of action.
    switch(IR) {
      // 3. Do whatever the instruction should do according to the spec.
      case LDI:
        cpu->reg[operandA] = operandB;
        break;
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      case SUB:
        alu(cpu, ALU_SUB, operandA, operandB);
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case DIV:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case MOD:
        alu(cpu, ALU_MOD, operandA, operandB);
        break;
      case INC:
        alu(cpu, ALU_MOD, operandA, operandB);
        break;
      case DEC:
        alu(cpu, ALU_MOD, operandA, operandB);
        break;
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;
      case PUSH:
        cpu->reg[SP]--;
        cpu_ram_write(cpu, cpu->reg[SP], cpu->reg[operandA]);
        break;
      case POP:
        cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[SP]);
        cpu->reg[SP]++;
        break;
      case CALL:
        cpu->reg[SP]--;
        cpu_ram_write(cpu, cpu->reg[SP], cpu->PC + 2);
        cpu->PC = cpu->reg[operandA];
        break;
      case HLT:
        running = 0;
        break;
      default:
        printf("Did not recognize instruction %x\n", IR);
        break;
    }
    // 4. Move the PC to the next instruction.
    // Check if instruction sets the PC using bit 4 of IR
    int instruction_sets_PC = (IR >> 4) & 0b00000001;

    if (!instruction_sets_PC)
      cpu->PC += (IR >> 6) + 1;

  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // Initialize the PC and other special registers
  cpu->PC = 0;

  // Zero registers and RAM
  for (int i = 0; i < 8; i++) {
    cpu->reg[i] = 0;
  }

  for (int i = 0; i < 256; i++) {
    cpu->ram[i] = 0;
  }

  cpu->reg[SP] = 0xF4; // Initialize SP to point to empty stack
}

