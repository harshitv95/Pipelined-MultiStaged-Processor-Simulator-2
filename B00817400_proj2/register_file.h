#include "config.h"
#include "flags.h"


typedef struct P_REG {
    int data;
    int flags[NUM_FLAGS];
} P_REG;

// To store values
P_REG physical_registers[P_REG_COUNT];
int architectural_registers[A_REG_COUNT];

int rename_table[A_REG_COUNT];
int renamed_table[P_REG_COUNT];
int allocated[P_REG_COUNT];
int status[P_REG_COUNT];
int waiting[P_REG_COUNT][ISSUE_QUEUE_CAPACITY];

void init_reg_file();

int rename(int a_reg);

int get_physical_reg_address(int a_reg);

// In IQ, an entry at index iq_idx waits for output of this register
int wait_for_value(int p_reg, int iq_idx);

int* waiting_iq_index(int p_reg);

int physical_register_valid(int p_reg);

// Writes to physical register only
void physical_register_write(int p_reg, int value, int flag);

// Reads from Physical Register only
int physical_register_read(int p_reg);
