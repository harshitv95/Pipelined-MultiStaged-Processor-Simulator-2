#include "register_file.h"

void init_reg_file()
{
  for (int i = 0; i < A_REG_COUNT; i++)
  {
    rename_table[i] = -1;
    _architectural_register_dirty[i] = 0;
  }
}

int rename_register(int a_reg)
{
  int i = 0;
  int old_instance = rename_table[a_reg];
  while (i < P_REG_COUNT)
  {
    if (!allocated[i])
      break;
    i++;
  }
  if (i == P_REG_COUNT)
    return -1;
  rename_table[i] = a_reg;
  allocated[i] = 1;
  status[i] = 0;
  deallocate_register(a_reg, old_instance);
  return i;
}

void deallocate_register(int a_reg, int p_reg)
{
  allocated[p_reg] = 0;
}

int get_physical_reg_address(int a_reg)
{
  return rename_table[a_reg];
}

void wait_for_value(int p_reg, int iq_idx)
{
  waiting[p_reg][iq_idx] = 1;
}

int *waiting_iq_index(int p_reg)
{
  return waiting[p_reg];
}

int physical_register_valid(int p_reg)
{
  return status[p_reg];
}

// Change flag implementation
void physical_register_write(int p_reg, int value, int flag)
{
  physical_registers[p_reg].data = value;
  if (flag != -1)
    physical_registers[p_reg].flags[flag] = 1;
  status[p_reg] = value;
}

int physical_register_read(int p_reg)
{
  return physical_registers[p_reg].data;
}

void all_register_read(CPU_Stage *stage, int readS1, int readS2, int readS3)
{
  if (readS1 && !(stage->rs1_valid))
  {
    int n = -1;
    if ((n = check_forwarded_bus_data(stage->p_rs1)) > -1)
    {
      stage->rs1_value = forward[n].data;
      stage->rs1_valid = 1;
    }
    else if (physical_register_valid(stage->p_rs1) > 0)
    {
      stage->rs1_value = physical_register_read(stage->p_rs1);
      stage->rs1_valid = 1;
    }
  }
  if (readS2 && !(stage->rs2_valid))
  {
    int n = -1;
    if ((n = check_forwarded_bus_data(stage->p_rs2)) > -1)
    {
      stage->rs2_value = forward[n].data;
      stage->rs2_valid = 1;
    }
    else if (physical_register_valid(stage->p_rs2) > 0)
    {
      stage->rs2_value = physical_register_read(stage->p_rs2);
      stage->rs2_valid = 1;
    }
  }
  if (readS3 && !(stage->rs3_valid))
  {
    int n = -1;
    if ((n = check_forwarded_bus_data(stage->p_rs3)) > -1)
    {
      stage->rs3_value = forward[n].data;
      stage->rs3_valid = 1;
    }
    else if (physical_register_valid(stage->p_rs3) > 0)
    {
      stage->rs3_value = physical_register_read(stage->p_rs3);
      stage->rs3_valid = 1;
    }
  }
}

void register_fetch(CPU_Stage *stage)
{
  switch ((opcode)(stage->opcode))
  {
  case LOAD:
  case ADDL:
  case SUBL:
  case JUMP:
    all_register_read(stage, 1, 0, 0);
    break;
  case ADD:
  case SUB:
  case MUL:
  case STORE:
    all_register_read(stage, 1, 1, 0);
    break;
  case STR:
    all_register_read(stage, 1, 1, 1);
    break;
  case HALT:
    set_flag(HALT_FLAG, 1);
    //   int pc_offset = cpu->code_memory_size - get_code_index(stage->pc) - 1;
    //   cpu->num_instructions -= pc_offset;
    break;
  case BZ:
  case BNZ:
    if (zero_flag_valid(stage->pc))
      stage->buffer = get_zero_flag(stage->pc);
    break;
  }
}

void architectural_register_write(int p_reg, int a_reg, int value)
{
  architectural_registers[a_reg] = value;
  _architectural_register_dirty[a_reg] = 1;
}