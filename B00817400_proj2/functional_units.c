#include "cpu.h"
#include "functional_units.h"
#include "reorder_buffer.h"

void execute(APEX_CPU* cpu) {

}

void _fu_int(APEX_CPU* cpu) {
    
}

void _fu_mul(APEX_CPU* cpu) {
    
}

void _fu_mem(APEX_CPU* cpu) {
    
}

void _fu_branch(APEX_CPU* cpu) {
    
}


/****** Int FU Stages ******/
void __fu_int_1() {

}
void __fu_int_2() {
    
}
/***************************/

/****** Mul FU Stages ******/
void __fu_mul_1(APEX_CPU* cpu) {
    
}
void __fu_mul_2(APEX_CPU* cpu) {
    
}
void __fu_mul_3(APEX_CPU* cpu) {
    
}
/****************************/

/****** Branch FU Stage ******/
void __fu_branch(APEX_CPU* cpu) {

}
/*****************************/

/****** Mem FU Stage (2 cycles, not pipelined) ******/
void __fu_mem_1(APEX_CPU* cpu) {

}
void __fu_mem_2(APEX_CPU* cpu) {
    
}
/****************************************************/