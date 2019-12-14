#include "cpu_commons.h"
#include "config.h"

#ifndef _APEX_ROB_H_
#define _APEX_ROB_H_

typedef struct ROB_Entry {
    // int rob_id;
    int valid;
    int allocated;
    CPU_Stage* instruction;
    // Add more fields here
} ROB_Entry;

typedef struct REORDER_BUFFER
{
	ROB_Entry* buffer;
	int head, tail;
    int count;
} REORDER_BUFFER;


REORDER_BUFFER *rob;

void init_rob();

int is_rob_full();

int insert_to_rob(CPU_Stage* instruction);


// Pop the head instruction IF AND ONLY IF
// it is valid, i.e. has the output operand available
ROB_Entry* pop_from_rob();

int is_rob_head_valid();

// Write the output operand (value) to an ROB entry,
// identified by rob_id
int update_rob_value(int phy_register_address, int value);

int rob_has_more();

#endif