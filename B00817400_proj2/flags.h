
#ifndef _APEX_FLAGS_H
#define _APEX_FLAGS_H


/* Zero Flag */
int flags[NUM_FLAGS];
int flags_valid[NUM_FLAGS];


int zero_flag[1000][3];
int zero_flag_idx = 0;


/* Flags */
typedef enum {
  ZERO_FLAG,
  JUMP_FLAG,
  HALT_FLAG,
  CARRY_FLAG,

  NUM_FLAGS
} flags;

void set_zero_flag(int value, int pc_value);

void lock_zero_flag(int pc_value);

int get_zero_flag(int pc_value);

int zero_flag_valid(int branch_pc);

#endif