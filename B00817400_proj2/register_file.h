#include "flags.h"

#define P_REG_COUNT 24
#define A_REG_COUNT 16

typedef struct P_REG {
    int data;
    int flags[NUM_FLAGS];
} P_REG;

P_REG physical_registers[P_REG_COUNT];
int architectural_registers[A_REG_COUNT];

int rename_table[A_REG_COUNT];
int renamed_table[P_REG_COUNT];
int allocated[P_REG_COUNT];
int status[P_REG_COUNT];
int waiting[P_REG_COUNT][8];

int rename(int a_reg);
int wait(int p_reg, int iq_idx);
int is_valid(int p_reg);
int p_reg_write(int p_reg, int value, int flag);