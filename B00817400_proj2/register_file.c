#include "register_file.h"

int rename(int a_reg) {
    int i = 0;
    while (i<P_REG_COUNT) {
        if (!allocated[i]) break;
        i++;
    }
    rename_table[i] = a_reg;
    status[i] = 0;
}

int wait(int p_reg, int iq_idx) {
    waiting[p_reg][iq_idx] = 1;
}

int is_valid(int p_reg) {
    return status[p_reg];
}


// Change flag implementation
int p_reg_write(int p_reg, int value, int flag) {
    physical_registers[p_reg].data = value;
    if (flag != -1) physical_registers[p_reg].flags[flag] = 1;
    status[p_reg] = value;
}
