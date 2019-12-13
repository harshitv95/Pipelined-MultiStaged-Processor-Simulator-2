#include "cpu_commons.h"

#ifndef _APEX_ROB_H_
#define _APEX_ROB_H_
#define size 8


struct queue
{
	int array[size];
	int head, tail;
};

typedef struct ROB_Entry {
    int rob_id;
    int valid;
    CPU_Stage instruction;
    // Add more fields here
} ROB_Entry;


int is_rob_full();

int insert_to_rob(CPU_Stage* instruction);


// Pop the head instruction IF AND ONLY IF
// it is valid, i.e. has the output operand available
ROB_Entry* pop_from_rob();

int is_head_valid();

// Write the output operand (value) to an ROB entry,
// identified by rob_id
int update_rob_value(int rob_index, int value);

#endif