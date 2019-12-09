#include "cpu.h"
#include "functional_units.h"
#include "reorder_buffer.h"

int __fu_busy[__NUM_FUs__];



/****** Int FU Stages ******/
void __fu_int_1(APEX_CPU* cpu) {
    CPU_Stage* stage = &cpu->stage[FU_INT_1];

}
void __fu_int_2(APEX_CPU* cpu) {
    CPU_Stage* stage = &cpu->stage[FU_INT_2];

    
}
/***************************/

/****** Mul FU Stages ******/
void __fu_mul_1(APEX_CPU* cpu) {
    CPU_Stage* stage = &cpu->stage[FU_MUL_1];
    
}
void __fu_mul_2(APEX_CPU* cpu) {
    CPU_Stage* stage = &cpu->stage[FU_MUL_2];
    
}
void __fu_mul_3(APEX_CPU* cpu) {
    CPU_Stage* stage = &cpu->stage[FU_MUL_3];
    
}
/****************************/

/****** Branch FU Stage ******/
void __fu_branch(APEX_CPU* cpu) {
    CPU_Stage* stage = &cpu->stage[FU_BR];

}
/*****************************/

/****** Mem FU Stage (2 cycles, not pipelined) ******/
void __fu_mem_1(APEX_CPU* cpu) {
    CPU_Stage* stage = &cpu->stage[FU_MEM];
    __fu_busy[MEM_FU] = 1;
}
void __fu_mem_2(APEX_CPU* cpu) {
    CPU_Stage* stage = &cpu->stage[FU_MEM];


    __fu_busy[MEM_FU] = 0;
}
/****************************************************/












int execute(APEX_CPU* cpu) {

}

int is_fu_available(functional_unit_type fu) {
    return __fu_busy[fu] == 0;
}

void _fu_int(APEX_CPU* cpu) {
    __fu_int_2(cpu);
    __fu_int_1(cpu);
}

void _fu_mul(APEX_CPU* cpu) {
    __fu_mul_3(cpu);
    __fu_mul_2(cpu);
    __fu_mul_1(cpu);
}

void _fu_mem(APEX_CPU* cpu) {
    __fu_mem_2(cpu);
    __fu_mem_1(cpu);
}

void _fu_branch(APEX_CPU* cpu) {
    __fu_branch(cpu);
}

