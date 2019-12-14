
#ifndef _APEX_FLAGS_H
#define _APEX_FLAGS_H



/* Flags */
typedef enum {
  ZERO_FLAG,
  JUMP_FLAG,
  HALT_FLAG,
  CARRY_FLAG,

  NUM_FLAGS
} flags;


/* Zero Flag */
int flag_values[NUM_FLAGS];
int flags_valid[NUM_FLAGS];


int zero_flag[1000][3];


void set_zero_flag(int value, int pc_value);

void lock_zero_flag(int pc_value);

int get_zero_flag(int pc_value);

int zero_flag_valid(int branch_pc);

void set_flag(flags flag, int value);

int get_flag(flags flag);

int is_flag_valid(flags flag);

#endif