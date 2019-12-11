#include "config.h"
#include "cpu_commons.h"
#include "instruction_commons.h"

#ifndef _APEX_IQ_H_
#define _APEX_IQ_H_

CPU_Stage* issueQueueList[ISSUE_QUEUE_CAPACITY];

int is_iq_full();

int insert_to_iq(CPU_Stage* instruction);

 CPU_Stage* get_available_instructions();

CPU_Stage* pop_from_iq(int index);

#endif