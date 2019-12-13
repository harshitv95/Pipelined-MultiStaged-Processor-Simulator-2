#include "flags.h"

int cached_pc = -1;
int cached_idx = -1;

void set_zero_flag(int value, int pc_value)
{
    int i = 0;
    for (; i < zero_flag_idx; i++)
    {
        if (zero_flag[i][0] == pc_value)
            break;
    }
    if (zero_flag_idx > 0 && i < zero_flag_idx) {
        zero_flag[i][1] = pc_value;
        zero_flag[i][2] = 1;
    } else {
        zero_flag[zero_flag_idx][0] = pc_value;
        zero_flag[zero_flag_idx][1] = value;
        zero_flag[zero_flag_idx][2] = 1;
    }
}

void lock_zero_flag(int pc_value)
{
    zero_flag[zero_flag_idx][0] = pc_value;
    zero_flag[zero_flag_idx++][2] = 0;
}

int get_zero_flag(int pc_value)
{
    int i = 0;
    int found = 0;
    for (; i < zero_flag_idx; i++)
    {
        if (zero_flag[i][0] > pc_value)
        {
            i--;
            found = 1;
            break;
        }
    }
    // if (i == -1 || i == zero_flag_idx)
    //     return 0;
    if (!found) return 0;
    zero_flag_idx = 0;
    return zero_flag[i][1];
}

int zero_flag_valid(int branch_pc)
{
    int i = 0;
    int found = 0;
    for (; i < zero_flag_idx; i++)
    {
        if (zero_flag[i][0] > branch_pc)
        {
            i--;
            found = 1;
            break;
        }
    }
    if (found)
        return zero_flag[i][2];
    return 0;
}

void set_flag(flags flag, int value) {
    flag_values[flag] = value;
    flags_valid[flag] = 1;
}