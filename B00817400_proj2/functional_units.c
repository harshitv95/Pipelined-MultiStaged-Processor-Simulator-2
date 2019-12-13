#include "functional_units.h"
#include "issue_queue.h"
#include "reorder_buffer.h"

int __fu_busy[__NUM_FUs__];

/* Functional Units Operations */
static int adder(CPU_Stage *stage, int n1, int n2)
{
    int res = n1 + n2;
    if (stage)
        stage->buffer = res;
    return res;
}

static int multiplier(CPU_Stage *stage, int n1, int n2)
{
    int res = n1 * n2;
    if (stage)
        stage->buffer = res;
    return res;
}

static int logical_and(CPU_Stage *stage, int n1, int n2)
{
    int res = n1 & n2;
    if (stage)
        stage->buffer = res;

    return res;
}

static int logical_exor(CPU_Stage *stage, int n1, int n2)
{
    int res = n1 ^ n2;
    if (stage)
        stage->buffer = res;

    return res;
}

static int logical_or(CPU_Stage *stage, int n1, int n2)
{
    int res = n1 | n2;
    if (stage)
        stage->buffer = res;
    return res;
}

/****** Int FU Stages ******/
void __fu_int_1(APEX_CPU *cpu)
{
    CPU_Stage *stage = &cpu->stage[FU_INT_1];

    switch (stage->opcode)
    {
    case STORE:
        adder(stage, stage->rs2_value, stage->imm);
        break;
    case STR:
        adder(stage, stage->rs2_value, stage->rs3_value);
        break;
    case ADD:
    case LDR:
        adder(stage, stage->rs1_value, stage->rs2_value);
        break;
    case SUB:
        adder(stage, stage->rs1_value, -1 * stage->rs2_value);
        break;
    case MOVC:
    case ADDL:
    case LOAD:
        adder(stage, stage->rs1_value, stage->imm);
        break;
    case SUBL:
        adder(stage, stage->rs1_value, -1 * (stage->imm));
        break;
    case MUL:
        multiplier(cpu, stage->rs1_value, stage->rs2_value);
        break;
    case AND:
        logical_and(cpu, stage->rs1_value, stage->rs2_value);
        break;
    case OR:
        logical_or(cpu, stage->rs1_value, stage->rs2_value);
        break;
    case EXOR:
        logical_exor(cpu, stage->rs1_value, stage->rs2_value);
        break;

    case JUMP:
        // cpu->jump_address_register = stage->pc + 4;
        // int pc_offset = stage->pc - stage->buffer;
        // cpu->num_instructions += ((pc_offset / 4));
        // set_flag(cpu, JUMP_FLAG, 1);
        // cpu->pc = stage->buffer;
        // flush_instructions(cpu, EX1);
        break;

    case BNZ:
        // if (branch_with_zero(cpu, 0, stage->imm))
        //     flush_instructions(cpu, EX1);
        break;
    case BZ:
        // if (branch_with_zero(cpu, 1, stage->imm))
        //     flush_instructions(cpu, EX1);
        break;
    }

    cpu->stage[FU_INT_2] = cpu->stage[FU_INT_1];
    cpu->stage[FU_INT_1].opcode = _BUBBLE;
}
void __fu_int_2(APEX_CPU *cpu)
{
    CPU_Stage *stage = &cpu->stage[FU_INT_2];
}
/***************************/

/****** Mul FU Stages ******/
void __fu_mul_1(APEX_CPU *cpu)
{
    CPU_Stage *stage = &cpu->stage[FU_MUL_1];
}
void __fu_mul_2(APEX_CPU *cpu)
{
    CPU_Stage *stage = &cpu->stage[FU_MUL_2];
}
void __fu_mul_3(APEX_CPU *cpu)
{
    CPU_Stage *stage = &cpu->stage[FU_MUL_3];
}
/****************************/

/****** Branch FU Stage ******/
void __fu_branch(APEX_CPU *cpu)
{
    CPU_Stage *stage = &cpu->stage[FU_BR];
}
/*****************************/

/****** Mem FU Stage (2 cycles, not pipelined) ******/
void __fu_mem_1(APEX_CPU *cpu)
{
    CPU_Stage *stage = &cpu->stage[FU_MEM];
    __fu_busy[MEM_FU] = 1;
}
void __fu_mem_2(APEX_CPU *cpu)
{
    CPU_Stage *stage = &cpu->stage[FU_MEM];

    __fu_busy[MEM_FU] = 0;
}
/****************************************************/

int execute(APEX_CPU *cpu)
{
}

int is_fu_available(functional_unit_type fu)
{
    return __fu_busy[fu] == 0;
}

void _fu_int(APEX_CPU *cpu)
{
    __fu_int_2(cpu);
    __fu_int_1(cpu);
}

void _fu_mul(APEX_CPU *cpu)
{
    __fu_mul_3(cpu);
    __fu_mul_2(cpu);
    __fu_mul_1(cpu);
}

void _fu_mem(APEX_CPU *cpu)
{
    __fu_mem_2(cpu);
    __fu_mem_1(cpu);
}

void _fu_branch(APEX_CPU *cpu)
{
    __fu_branch(cpu);
}
