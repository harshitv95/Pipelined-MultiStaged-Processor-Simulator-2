#include "cpu.h"

#ifndef _APEX_FU_H_
#define _APEX_FU_H_

typedef enum {
    // 0, Default int value, thus default functional unit of an instruction will be No Type
    __NO_FU__,

    INT_FU,
    MUL_FU,
    BRANCH_FU,
    MEM_FU,

    __NUM_FUs__
} functional_unit_type;

int is_fu_available(functional_unit_type fu);

void _fu_int(APEX_CPU* cpu);
void _fu_mul(APEX_CPU* cpu);
void _fu_branch(APEX_CPU* cpu);
void _fu_mem(APEX_CPU* cpu);

#endif