
#ifndef _APEX_ROB_H_
#define _APEX_ROB_H_


typedef struct ROB_Entry {
    int a;
} ROB_Entry;


int is_rob_full();

int insert_to_rob(ROB_Entry* entry);

ROB_Entry* pop_from_rob();

int register_write();

#endif