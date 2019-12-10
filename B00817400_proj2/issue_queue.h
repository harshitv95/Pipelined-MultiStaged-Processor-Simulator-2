#include "cpu.h"
#include "instruction.h"
#include "functional_units.h"

#ifndef _APEX_IQ_H_
#define _APEX_IQ_H_

APEX_Instruction issueQueueList[8];

int is_iq_full();

int insert_to_iq(APEX_Instruction* instruction);



APEX_Instruction* pop_from_iq(int index);

#endif