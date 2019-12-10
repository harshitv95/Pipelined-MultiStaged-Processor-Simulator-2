#include "config.h"
#include "instruction_commons.h"

#ifndef _APEX_IQ_H_
#define _APEX_IQ_H_

APEX_Instruction issueQueueList[ISSUE_QUEUE_CAPACITY];

int is_iq_full();

int insert_to_iq(APEX_Instruction* instruction);

APEX_Instruction* pop_from_iq(int index);

#endif