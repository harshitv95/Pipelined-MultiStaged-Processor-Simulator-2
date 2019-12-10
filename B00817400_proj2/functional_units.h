#include "cpu.h"

#ifndef _APEX_FU_H_
#define _APEX_FU_H_


int is_fu_available(functional_unit_type fu);

void _fu_int(APEX_CPU* cpu);
void _fu_mul(APEX_CPU* cpu);
void _fu_branch(APEX_CPU* cpu);
void _fu_mem(APEX_CPU* cpu);

#endif