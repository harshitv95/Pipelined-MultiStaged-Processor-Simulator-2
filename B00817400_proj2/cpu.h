#include "cpu_commons.h"
#include "instruction.h"
#include "issue_queue.h"
#include "load_store_queue.h"
#include "reorder_buffer.h"
#include "flags.h"

#ifndef _APEX_CPU_H_
#define _APEX_CPU_H_




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

// int
// memory1(APEX_CPU* cpu);
// int
// memory2(APEX_CPU* cpu);

// int
// writeback(APEX_CPU* cpu);

void print(APEX_CPU* cpu);

CPU_Stage create_bubble();






// 
// int flag_valid(flags flag);

#endif
