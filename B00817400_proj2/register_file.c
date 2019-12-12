#include "register_file.h"

void init_reg_file() {
//    for (int i=0; i<A_REG_COUNT; i++) {
//        rename(i);
//    }
}

int rename_register(int a_reg) {
    int i = 0;
    while (i<P_REG_COUNT) {
        if (!allocated[i]) break;
        i++;
    }
    if (i == P_REG_COUNT) return -1;
    rename_table[i] = a_reg;
    allocated[i] = 1;
    status[i] = 0;
    return i;
}

int get_physical_reg_address(int a_reg) {
    return rename_table[a_reg];
}

int wait_for_value(int p_reg, int iq_idx) {
    waiting[p_reg][iq_idx] = 1;
}

int* waiting_iq_index(int p_reg) {
    return waiting[p_reg];
}

int physical_register_valid(int p_reg) {
    return status[p_reg];
}

// Change flag implementation
void physical_register_write(int p_reg, int value, int flag) {
    physical_registers[p_reg].data = value;
    if (flag != -1) physical_registers[p_reg].flags[flag] = 1;
    status[p_reg] = value;
}

int physical_register_read(int p_reg) {
    return physical_registers[p_reg].data;
}