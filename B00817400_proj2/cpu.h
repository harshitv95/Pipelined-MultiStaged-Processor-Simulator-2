#include "flags.h"
#include "instruction.h"
#include "reorder_buffer.h"

#ifndef _APEX_CPU_H_
#define _APEX_CPU_H_

#define NUM_FWD_BUSES 2

enum
{
  F,
  DRF,
  IQ,
  
  FU_INT_1,
  FU_INT_2,
  FU_MUL_1,
  FU_MUL_2,
  FU_MUL_3,
  FU_BR,
  FU_MEM,

  // WB,
  NUM_STAGES
};

typedef enum {
    // 0, Default int value, thus default functional unit of an instruction will be No Type
    __NO_FU__,

    INT_FU,
    MUL_FU,
    BRANCH_FU,
    MEM_FU,

    __NUM_FUs__
} functional_unit_type;


/* Model of CPU stage latch */
typedef struct CPU_Stage
{
  int pc;		    // Program Counter
  // char opcode[128];	// Operation Code
  opcode opcode;
  int imm;		    // Literal Value
  int rs1_value;	// Source-1 Register Value
  int rs2_value;	// Source-2 Register Value
  int rs3_value;	// Source-3 Register Value
  int rs1_valid;  // Source-1 valid
  int rs2_valid;
  int rs3_valid;
  int buffer;		    // Latch to hold some value
  int mem_address;	// Computed Memory Address
  int busy;	   	    // Flag to indicate, stage is performing some action
  int empty;
  int stalled;		  // Flag to indicate, stage is stalled
  int flushed;

  int a_rs1;		    // Source-1 Register Address
  int a_rs2;		    // Source-2 Register Address
  int a_rs3;		    // Source-2 Register Address
  int a_rd;
  int imm;		      // Literal Value

  int p_rs1;
  int p_rs2;
  int p_rs3;
  int p_rd;

  functional_unit_type fu;
  
  


} CPU_Stage;

/* Forwarding Bus to forward data and tag */
typedef struct FWD_BUS
{
  int tag;
  int data;
  int valid;
} FWD_BUS;


/* Model of APEX CPU */
typedef struct APEX_CPU
{
  /* Clock cycles elasped */
  int clock;

  /* Current program counter */
  int pc;

  /* Max number of registers supported by the CPU */
  int reg_count;

  /* Integer register file */
  int regs[32];
  int regs_valid[32];

  /* Register to hold the address of the next instruction in sequence,
  while temporarily switching to the JUMPed instruction address */
  int jump_address_register;

  /* Array of %NUM_STAGES% (7) CPU_stage */
  CPU_Stage stage[NUM_STAGES];

  /* Code Memory where instructions are stored */
  APEX_Instruction* code_memory;
  int code_memory_size;
  int num_instructions;

  /* Data Memory */
  int data_memory[4000];

  /* Some stats */
  int ins_completed;

  /* 2 Forwarding Buses, 1 from EX2 and another from MEM2 */
  FWD_BUS forward[NUM_FWD_BUSES];
  FWD_BUS broadcast[NUM_FWD_BUSES*2];
  FWD_BUS *forward_zero;

  /* Flags */
  int flags[NUM_FLAGS];
  int flags_valid[NUM_FLAGS];

  int fetched_before_stall;


} APEX_CPU;


APEX_Instruction*
create_code_memory(const char* filename, int* size);

APEX_CPU*
APEX_cpu_init(const char* filename, int debug);

int
APEX_cpu_run(APEX_CPU* cpu, int numCycles);

void
APEX_cpu_stop(APEX_CPU* cpu);

int
fetch(APEX_CPU* cpu);

int
decode(APEX_CPU* cpu);

int
execute(APEX_CPU* cpu);

int
memory1(APEX_CPU* cpu);
int
memory2(APEX_CPU* cpu);

int
writeback(APEX_CPU* cpu);

void print(APEX_CPU* cpu);

CPU_Stage create_bubble();

#endif
