#include "config.h"
#include "cpu_commons.h"
#include "instruction_commons.h"

#ifndef _APEX_IQ_H_
#define _APEX_IQ_H_

CPU_Stage *issueQueueList[ISSUE_QUEUE_CAPACITY];

CPU_Stage *validInstList[ISSUE_QUEUE_CAPACITY];

void init_iq();

int is_iq_full();

int insert_to_iq(CPU_Stage* instruction);

 CPU_Stage* get_iq_available_instructions();

CPU_Stage* pop_from_iq(int index);

void update_iq_value(int tag, int value);

void iq_register_reads();

int iq_has_more();

#endif