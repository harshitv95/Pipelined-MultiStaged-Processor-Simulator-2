#include "config.h"
#include "cpu_commons.h"

#ifndef _APEX_LSQ_H_
#define _APEX_LSQ_H_

CPU_Stage* loadStoreQueueList[LOAD_STORE_QUEUE_CAPACITY];

int is_lsq_full();

int insert_to_lsq(CPU_Stage* instruction);

CPU_Stage* get_lsq_available_instructions(int address);

CPU_Stage* pop_from_lsq(int index);

#endif