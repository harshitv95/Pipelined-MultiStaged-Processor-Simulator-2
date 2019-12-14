#include "config.h"
#include "cpu_commons.h"

#ifndef _APEX_LSQ_H_
#define _APEX_LSQ_H_

typedef struct LSQ_Entry {
    int mem_address_valid;
    int value_valid;
    int allocated;
    CPU_Stage* instruction;
} LSQ_Entry;

typedef struct LOAD_STORE_QUEUE {
    LSQ_Entry* queue;
	int head, tail;
    int count;
} LOAD_STORE_QUEUE;

// CPU_Stage* loadStoreQueueList[LOAD_STORE_QUEUE_CAPACITY];

LOAD_STORE_QUEUE *lsq;

void init_lsq();

int is_lsq_full();

int insert_to_lsq(CPU_Stage* instruction);

LSQ_Entry* pop_from_lsq();

int is_lsq_head_valid();

/*Register Forwarding*/
void update_lsq_value(int phy_reg_address, int value);

void update_lsq_mem_address(int lsq_index, int mem_address);

int lsq_has_more();

#endif