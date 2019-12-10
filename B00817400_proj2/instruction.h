#include "instruction_commons.h"

#ifndef _APEX_INST_H_
#define _APEX_INST_H_
/**
 *  instruction.h
 *  Contains various Instruction-specific stage processing logic
 *
 *  Author :
 *  Harshit Vadodaria (havdoda1@binghamton.edu)
 *  State University of New York, Binghamton
 */




/* Evaluates OpCode string and returns corresponding
OpCode Enum accordingly */
opcode evalOpCode(const char* opCode);

char* opcodeToStr(opcode opcode);

void parse_operands(APEX_Instruction* ins, char *tokens);

#endif
