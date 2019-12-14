#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "register_file.h"
#include "functional_units.h"

/* Set this flag to 1 to enable debug messages */
int ENABLE_DEBUG_MESSAGES = 1;
// #define ENABLE_DEBUG_MESSAGES 1

CPU_Stage bubble;
// TODO: Add stages
char print_stages[NUM_STAGES][16] = {
  "FETCH_____STAGE\0", "DECODE_RN_STAGE\0", "FU_INT_1\0", "FU_INT_2\0", "FU_MUL_1\0", "FU_MUL_2\0", "FU_MUL_3\0", "FU_BR\0", "FU_MEM\0"};

/*
 * This function creates and initializes APEX cpu.
 *
 * Note : You are free to edit this function according to your
 * 				implementation
 */
APEX_CPU *
APEX_cpu_init(const char *filename, int debug)
{
  ENABLE_DEBUG_MESSAGES = debug;
  init_reg_file();
  init_lsq();
  init_rob();
  init_iq();
  if (!filename)
  {
    return NULL;
  }

  int num_arch_registers = 16;

  APEX_CPU *cpu = malloc(sizeof(*cpu));
  if (!cpu)
  {
    return NULL;
  }

  /* Initialize PC, Registers and all pipeline stages */
  cpu->pc = 4000;
  cpu->reg_count = num_arch_registers;
  // memset(cpu->regs, 0, sizeof(int) * num_arch_registers);
  // memset(cpu->regs_valid, 1, sizeof(int) * num_arch_registers);
  memset(cpu->stage, 0, sizeof(CPU_Stage) * NUM_STAGES);
  memset(cpu->data_memory, 0, sizeof(int) * 4000);
  memset(cpu->data_memory_dirty, 0, sizeof(int) * 4000);
  for (int i=0; i<NUM_STAGES; i++) {
    cpu->stage[i].opcode = _BUBBLE;
  }
  // memset(cpu->flags, 0, sizeof(int) * NUM_FLAGS); // Important to initialize all flags with 0
  // memset(cpu->flags_valid, 1, sizeof(int) * NUM_FLAGS);
  // for (int i = 0; i < NUM_FLAGS; i++)
  // {
  //   cpu->flags_valid[i] = 1;
  // }
  // for (int i = 0; i < num_arch_registers; i++)
  // {
  //   cpu->regs_valid[i] = 1;
  // }

  /* Parse input file and create code memory */
  cpu->code_memory = create_code_memory(filename, &cpu->code_memory_size);
  cpu->num_instructions = cpu->code_memory_size;

  if (!cpu->code_memory)
  {
    free(cpu);
    return NULL;
  }

  init_forward_buses();

  if (ENABLE_DEBUG_MESSAGES)
  {
    fprintf(stderr,
            "APEX_CPU : Initialized APEX CPU, loaded %d instructions\n",
            cpu->code_memory_size);
    fprintf(stderr, "APEX_CPU : Printing Code Memory\n");
    printf("%-9s %-9s %-9s %-9s %-9s %-9s\n", "opcode", "rd", "rs1", "rs2", "rs3", "imm");

    for (int i = 0; i < cpu->code_memory_size; ++i)
    {
      printf("%-9s %-9d %-9d %-9d %-9d %-9d\n",
             opcodeToStr(cpu->code_memory[i].opcode),
             cpu->code_memory[i].rd,
             cpu->code_memory[i].rs1,
             cpu->code_memory[i].rs2,
             cpu->code_memory[i].rs3,
             cpu->code_memory[i].imm);
    }
  }

  /* Make all stages busy except Fetch stage, initally to start the pipeline */
  /* Make instruction in all stages EMPTY (BUBBLE) except Fetch, to start the pipeline */
  for (int i = 1; i < NUM_STAGES; ++i)
  {
    cpu->stage[i].busy = 1;
    cpu->stage[i].opcode = _BUBBLE;
  }

  int code_index = get_code_index(cpu->pc);
  cpu->stage[F].pc = cpu->pc;
  do_fetch(cpu, &cpu->stage[F], code_index);

  bubble.opcode = _BUBBLE;

  return cpu;
}

/*
 * This function de-allocates APEX cpu.
 *
 * Note : You are free to edit this function according to your
 * 				implementation
 */
void APEX_cpu_stop(APEX_CPU *cpu)
{
  free(cpu->code_memory);
  free(cpu);
}

/* Converts the PC(4000 series) into
 * array index for code memory
 *
 * Note : You are not supposed to edit this function
 *
 */
int get_code_index(int pc)
{
  return (pc - 4000) / 4;
}
int get_memory_index(int address)
{
  return address / 4;
}

CPU_Stage create_bubble()
{
  // if (!bubble) {
  //   memset(&bubble, 0, sizeof(CPU_Stage));
  //   bubble->opcode = NOP;
  // }
  return bubble;
}

// void
// print_instruction(CPU_Stage* stage)
// {
//   opcode op = stage->opcode;
//   switch(op) {
//     case STORE:
//     // case ADDL:
//     // case SUBL:
//     printf(
//       "%s,R%d,R%d,#%d ", opcodeToStr(stage->opcode), stage->rs1, stage->rs2, stage->imm);
//       break;
//     case MOVC:
//       printf("%s,R%d,#%d ", opcodeToStr(stage->opcode), stage->rd, stage->imm);
//       break;
//     default:
//     printf(
//       "%s,R%d,R%d,#%d ", opcodeToStr(stage->opcode), stage->rd, stage->rs1, stage->rs2);
//       break;
//   }
// }

static void
print_instruction(APEX_CPU *cpu, int stageId)
{
  // for (int i=0;i<NUM_STAGES;i++) {
  printf("Instruction at %s --->\t\t", print_stages[stageId]);
  CPU_Stage *stage = &cpu->stage[stageId];
  if (!stage || stage->opcode == _BUBBLE || stage->pc == 0)
    printf("EMPTY");
  else
  {
    APEX_Instruction *ins =
        &cpu->code_memory[get_code_index(stage->pc)];
    printf("(I%d : %d) %s",
           ins->instNum,
           stage->pc,
           ins->instText);
    // }
  }
  printf("\n");
}

/* Debug function which dumps the cpu stage
 * content
 *
 * Note : You are not supposed to edit this function
 *
 */
static void
print_stage_content(char *name, APEX_CPU *cpu, CPU_Stage *stage)
{
  printf("%-15s: pc(%d) ", name, stage->pc);
  // print_instruction(stage);
  printf("\n");
}

static void
display_instructions(APEX_CPU* cpu) {
  printf("========= Stages =============\n");
  for (int i=F; i<NUM_STAGES; i++) {
    printf("%s : (I%d) %s [%s]\n",
      print_stages[i],
      cpu->stage[i].opcode == _BUBBLE ? "EMPTY" : cpu->stage[i].inst_text,
      cpu->stage[i].opcode == _BUBBLE ? "" : cpu->stage[i].renamed_inst_text
    );
  }
}

static void
display_register_contents(APEX_CPU *cpu)
{

  printf(
      "\n=============== LSQ ==========\n");
      for (int i=0; i<LOAD_STORE_QUEUE_CAPACITY; i++) {
        if (lsq->queue[i].allocated) {
          CPU_Stage* stage = lsq->queue[i].instruction;
            printf("%s : (I%d) %s [%s]\n",
            print_stages[i],
            stage[i].opcode == _BUBBLE ? "EMPTY" : stage[i].inst_text,
            stage[i].opcode == _BUBBLE ? "" : stage[i].renamed_inst_text
          );
        }
      }

  printf(
    "\n=============== ROB ==========\n");
      for (int i=0; i<REORDER_BUFFER_CAPACITY; i++) {
        if (rob->buffer[i].allocated) {
          CPU_Stage* stage = rob->buffer[i].instruction;
            printf("%s : (I%d) %s [%s]\n",
            print_stages[i],
            stage[i].opcode == _BUBBLE ? "EMPTY" : stage[i].inst_text,
            stage[i].opcode == _BUBBLE ? "" : stage[i].renamed_inst_text
          );
        }
      }

      printf(
  "\n=============== IQ ==========\n");
      for (int i=0; i<REORDER_BUFFER_CAPACITY; i++) {
        if (issueQueueList[i] != NULL) {
          CPU_Stage* stage = issueQueueList[i];
            printf("%s : (I%d) %s [%s]\n",
            print_stages[i],
            stage[i].opcode == _BUBBLE ? "EMPTY" : stage[i].inst_text,
            stage[i].opcode == _BUBBLE ? "" : stage[i].renamed_inst_text
          );
        }
      }



  printf(
      "\n=============== STATE OF RENAME TABLE ==========\n");
    for (int i=0; i<A_REG_COUNT; i++) {
      if (rename_table[i] != -1)
      printf("R[%d] --> P[%d]\n", i, rename_table[i]);
    }

  printf(
      "\n=============== STATE OF ARCHITECTURAL REGISTER FILE ==========\n");
  for (int i = 0; i < A_REG_COUNT; i++)
  {
    if (_architectural_register_dirty[i])
      printf("|\tREG[%02d]\t|\tValue = %-6d\t|\n",
             i,
             architectural_registers[i]);
  }
}

static void
display_memory_contents(APEX_CPU *cpu)
{
  // int MAX_MEM_DISP = 100;
  printf(
      "\n============== STATE OF DATA MEMORY =============\n");
  for (int i = 0; i < 4000; i++)
  {
    if (cpu->data_memory_dirty[i])
    printf("|\tMEM[%02d]\t|\tData Value = %02d\t|\n", i, cpu->data_memory[i]);
  }
}

void print(APEX_CPU *cpu)
{
  // printf("--------------------------------\n");
  // printf("Clock Cycle #: %d\n", cpu->clock);
  // printf("--------------------------------\n");
}

/**************/

// static int
// get_flag(APEX_CPU *cpu, int flag)
// {
//   return cpu->flags[flag];
// }

// static void
// set_flag(APEX_CPU *cpu, int flag, int value)
// {
//   cpu->flags[flag] = value;
//   cpu->flags_valid[flag]++;
// }

void do_fetch(APEX_CPU *cpu, CPU_Stage *stage, int code_index)
{
  APEX_Instruction *current_ins = &cpu->code_memory[code_index];
  stage->opcode = current_ins->opcode;
  stage->a_rd = current_ins->rd;
  stage->a_rs1 = current_ins->rs1;
  stage->a_rs2 = current_ins->rs2;
  stage->a_rs3 = current_ins->rs3;
  stage->imm = current_ins->imm;
  stage->inst_text = current_ins->instText;
  stage->inst_num = current_ins->instNum;
  stage->renamed_inst_text = current_ins->instText;

  /* Update PC for next instruction */
  cpu->pc += 4;
}
/*
 *  Fetch Stage of APEX Pipeline
 *
 *  Note : You are free to edit this function according to your
 * 				 implementation
 */
int fetch(APEX_CPU *cpu)
{
  CPU_Stage *stage = &cpu->stage[F];

  if (get_flag(JUMP_FLAG) == 1)
  {
    set_flag(JUMP_FLAG, 2);
  }
  else if (get_flag(JUMP_FLAG) == 2)
  {
    cpu->pc = cpu->jump_address_register;
    set_flag(JUMP_FLAG, 0);
  }

  int code_index = get_code_index(cpu->pc);

  if (ENABLE_DEBUG_MESSAGES)
  {
    // print_stage_content("Fetch", cpu, stage);
    print_instruction(cpu, F);
  }
  if (stage->flushed)
    cpu->stage[F] = create_bubble();

  int halt = get_flag(HALT_FLAG);
  if (halt)
    cpu->stage[F] = create_bubble();

  if (!stage->busy && !stage->stalled)
  {
    /* Copy data from fetch latch to decode latch*/
    cpu->stage[DRF] = cpu->stage[F];

    /* Store current PC in fetch latch */
    stage->pc = cpu->pc;

    /* Index into code memory using this pc and copy all instruction fields into
     * fetch latch
     */
    if (!halt)
    {
      if (code_index >= cpu->code_memory_size)
        cpu->stage[F] = create_bubble();
      else
        do_fetch(cpu, stage, code_index);
    }
  }
  stage->flushed = 0;
  return 0;
}

static void
forward_bus_read(APEX_CPU *cpu, CPU_Stage *stage)
{
  int n = -1;

  switch (stage->opcode)
  {
  case STR:
    if (!stage->rs3_valid && (n = check_forwarded_bus_data(stage->p_rs3)) > -1)
    {
      stage->rs3_value = forward[n].data;
      stage->rs3_valid = 1;
    }
  case ADD:
  case SUB:
  case MUL:
  case STORE:
  case LDR:
  case EXOR:
  case OR:
  case AND:
    if (!stage->rs2_valid && (n = check_forwarded_bus_data(stage->p_rs2)) > -1)
    {
      stage->rs2_value = forward[n].data;
      stage->rs2_valid = 1;
    }
  case ADDL:
  case SUBL:
  case JUMP:
  case LOAD:
    if (!stage->rs1_valid && (n = check_forwarded_bus_data(stage->p_rs1)) > -1)
    {
      stage->rs1_value = forward[n].data;
      stage->rs1_valid = 1;
    }
    break;
  }
}

/* Checks whether the specified register is valid */
static int register_valid(int regNum, APEX_CPU *cpu)
{
  return
      // (cpu->forward[0].tag = regNum && cpu->forward[0].valid) ||
      // (cpu->forward[1].tag = regNum && cpu->forward[1].valid) ||
      (physical_register_valid(regNum)) ||
      check_forwarded_register(regNum) > -1;
}

// void lock_register(APEX_CPU *cpu, int regNum)
// {
//   cpu->regs_valid[regNum]--;
// }

/* Checks flags valid status */
// int flag_valid(int flag)
// {
//   return flags_valid[flag] > 0;
// }

static void lock_flag(CPU_Stage *stage)
{
  // flags_valid[flag]--;
  lock_zero_flag(stage->pc);
}

static void change_stall_status(int stageId, APEX_CPU *cpu, int stallStatus)
{
  for (int i = stageId; i >= 0; i--)
  {
    cpu->stage[i].stalled = stallStatus;
  }
}

// static int has_dependency(APEX_CPU *cpu, int stageId)
// {
//   if (!cpu)
//     return 0;
//   CPU_Stage *stage = &cpu->stage[stageId];
//   if (!stage)
//     return 0;
//   switch (stage->opcode)
//   {
//   case ADD:
//   case LDR:
//   case SUB:
//   case MUL:
//   case AND:
//   case OR:
//   case EXOR:
//   case STORE:
//     return !(
//         (stage->rs1_valid || register_valid(stage->rs1, cpu)) &&
//         (stage->rs2_valid || register_valid(stage->rs2, cpu))
//         // && register_valid(stage->rd, cpu) // Output dependency
//     );
//   case LOAD:
//   case ADDL:
//   case SUBL:
//     return !(
//         (stage->rs1_valid || register_valid(stage->rs1, cpu))
//         // && register_valid(stage->rd, cpu) // Output dependency
//     );
//   // case MOVC:
//   //   return !(
//   //     register_valid(stage->rd, cpu)
//   //   );
//   case STR:
//     return !(
//         (stage->rs1_valid || register_valid(stage->rs1, cpu)) &&
//         (stage->rs2_valid || register_valid(stage->rs2, cpu)) &&
//         (stage->rs3_valid || register_valid(stage->rs3, cpu)));
//   case BZ:
//   case BNZ:
//     return !(
//         cpu->forward_zero->valid ||
//         flag_valid(ZERO_FLAG, cpu));
//   }
//   return 0;
// }

// static void register_wite(CPU_Stage *stage, APEX_CPU *cpu)
// {
//   int regNum = stage->rd;
//   cpu->regs[regNum] = stage->buffer;
//   cpu->regs_valid[regNum]++;
// }

/*
Register Fetch (reading register file with 3 read ports)
*/
// void
// register_read(APEX_CPU *cpu, CPU_Stage *stage, int readS1, int readS2, int readS3)
// {
//   if (readS1 && (physical_register_valid(stage->p_rs1) > 0))
//   {
//     stage->rs1_value = physical_register_read(stage->p_rs1);
//     stage->rs1_valid = 1;
//   }
//   if (readS2 && (physical_register_valid(stage->p_rs2) > 0))
//   {
//     stage->rs2_value = physical_register_read(stage->p_rs2);
//     stage->rs2_valid = 1;
//   }
//   if (readS3 && (physical_register_valid(stage->p_rs3) > 0))
//   {
//     stage->rs3_value = physical_register_read(stage->p_rs3);
//     stage->rs3_valid = 1;
//   }
// }

void rename_registers(CPU_Stage *stage)
{
  switch (stage->opcode)
  {
  case MOVC:
    stage->p_rd = rename_register(stage->a_rd);
    snprintf(
        stage->renamed_inst_text,
        sizeof stage->renamed_inst_text,
        "%s,P%d,#%d",
        opcodeToStr(stage->opcode),
        stage->p_rd,
        stage->imm);
    break;
  case STORE:
    stage->p_rs1 = get_physical_reg_address(stage->a_rs1);
    stage->p_rs2 = get_physical_reg_address(stage->a_rs2);
    snprintf(
        stage->renamed_inst_text,
        sizeof stage->renamed_inst_text,
        "%s,P%d,P%d,#%d",
        opcodeToStr(stage->opcode),
        stage->p_rs1,
        stage->p_rs2,
        stage->imm);
    break;
  case STR:
    stage->p_rs1 = get_physical_reg_address(stage->a_rs1);
    stage->p_rs2 = get_physical_reg_address(stage->a_rs2);
    stage->p_rs3 = get_physical_reg_address(stage->a_rs3);
    snprintf(
        stage->renamed_inst_text,
        sizeof stage->renamed_inst_text,
        "%s,P%d,P%d,P%d",
        opcodeToStr(stage->opcode),
        stage->p_rs1,
        stage->p_rs2,
        stage->p_rs3);
    break;
  case LOAD:
  case ADDL:
  case SUBL:
    stage->p_rs1 = get_physical_reg_address(stage->a_rs1);
    stage->p_rd = rename_register(stage->a_rd);
    snprintf(
        stage->renamed_inst_text,
        sizeof stage->renamed_inst_text,
        "%s,P%d,P%d,#%d",
        opcodeToStr(stage->opcode),
        stage->p_rd,
        stage->p_rs1,
        stage->imm);
    break;
  case ADD:
  case SUB:
  case LDR:
  case MUL:
  case AND:
  case OR:
  case EXOR:
    stage->p_rs1 = get_physical_reg_address(stage->a_rs1);
    stage->p_rs2 = get_physical_reg_address(stage->a_rs2);
    stage->p_rd = rename_register(stage->a_rd);
    snprintf(
        stage->renamed_inst_text,
        sizeof stage->renamed_inst_text,
        "%s,P%d,P%d,P%d",
        opcodeToStr(stage->opcode),
        stage->p_rd,
        stage->p_rs1,
        stage->p_rs2);
    break;
  case BZ:
  case BNZ:
  case HALT:
    break;
  case JUMP:
    stage->p_rs1 = get_physical_reg_address(stage->a_rs1);
    snprintf(
        stage->renamed_inst_text,
        sizeof stage->renamed_inst_text,
        "%s,P%d,#%d",
        opcodeToStr(stage->opcode),
        stage->p_rs1,
        stage->imm);
    break;
  }
}

/*
 *  Decode Stage of APEX Pipeline
 *
 *  Note : You are free to edit this function according to your
 * 				 implementation
 */
int decode(APEX_CPU *cpu)
{

  CPU_Stage *stage = &cpu->stage[DRF];
  // forward_bus_read(cpu, stage);
  if (stage->opcode != NOP && stage->opcode != _BUBBLE)
  {
    rename_registers(stage);

    int has_dep = is_rob_full() || is_iq_full() || is_lsq_full();
    change_stall_status(DRF, cpu, has_dep);
  }

  if (!stage->busy && !stage->stalled && !stage->flushed && stage->opcode != _BUBBLE && stage->opcode != NOP)
  {
    register_fetch(stage);

    if (get_flag(HALT_FLAG))
    {
      int pc_offset = cpu->code_memory_size - get_code_index(stage->pc) - 1;
      cpu->num_instructions -= pc_offset;
    }

    /* Copy data from decode latch to execute latch*/
    insert_to_iq(stage);
    insert_to_rob(stage);

    switch (stage->opcode)
    {
    case LOAD:
    case LDR:
    case STORE:
    case STR:
      stage->lsq_index = insert_to_lsq(stage);
    }
  }
  if (ENABLE_DEBUG_MESSAGES)
  {
    // print_stage_content("Decode/RF", cpu, stage);
    print_instruction(cpu, DRF);
  }

  //  if (stage->flushed)
  //  {
  //    stage->opcode = _BUBBLE;
  //    stage->flushed = 0;
  //    cpu->stage[EX1] = cpu->stage[DRF];
  //  }

  return 0;
}

void move_to_fu(APEX_CPU *cpu)
{
  // CPU_Stage* instructions;

  // get_iq_available_instructions();

  if (is_lsq_head_valid() && is_fu_available(MEM_FU))
  {
    LSQ_Entry *entry = pop_from_lsq();
    cpu->stage[FU_MEM] = *(entry->instruction);
  }

  // if (is_rob_head_valid())
  // {
  //   ROB_Entry *entry = pop_from_rob();
  //   architectural_register_write(entry->instruction->p_rd, entry->instruction->a_rd, entry->instruction->buffer);
  // }

  CPU_Stage **instructions = get_iq_available_instructions();
  for (int i = 0; i < ISSUE_QUEUE_CAPACITY; i++)
  {
    if (instructions[i] != NULL)
    {
      switch (instructions[i]->opcode)
      {
      case STORE:
      case STR:
      case LOAD:
      case LDR:
      case ADD:
      case ADDL:
      case SUB:
      case SUBL:
      case MOVC:
      case OR:
        if (is_fu_available(INT_FU))
        {
          cpu->stage[FU_INT_1] = *(pop_from_iq(i));
        }
        break;

      case MUL:
        if (is_fu_available(MUL_FU))
        {
          cpu->stage[FU_MUL_1] = *(pop_from_iq(i));
        }
        break;

      case BZ:
      case BNZ:
      case JUMP:
        if (is_fu_available(BRANCH_FU))
        {
          cpu->stage[FU_BR] = *(pop_from_iq(i));
        }
        break;
      default:
        break;
      }
    }
  }
}

void rob_retire(cpu)
{
  if (is_rob_head_valid())
  {
    ROB_Entry *entry = pop_from_rob();
    opcode op = entry->instruction->opcode;
    if (op != STR && op != STORE && op != HALT && op != JUMP && op != BZ && op != BNZ)
    {
      architectural_register_write(entry->instruction->p_rd, entry->instruction->a_rd, entry->instruction->buffer);
    }
  }
}

/* Functional Units */
// static int adder(CPU_Stage *stage, int n1, int n2)
// {
//   int res = n1 + n2;
//   // if (cpu)  {
//   //   CPU_Stage* stage = &cpu->stage[EX2];
//   if (stage)
//     stage->buffer = res;
//   // }
//   return res;
// }

// static int multiplier(APEX_CPU *cpu, int n1, int n2)
// {
//   int res = n1 * n2;
//   if (cpu)
//   {
//     CPU_Stage *stage = &cpu->stage[EX2];
//     if (stage)
//       stage->buffer = res;
//   }
//   return res;
// }

// static int logical_and(APEX_CPU *cpu, int n1, int n2)
// {
//   int res = n1 & n2;
//   if (cpu)
//   {
//     CPU_Stage *stage = &cpu->stage[EX2];
//     if (stage)
//       stage->buffer = res;
//   }
//   return res;
// }

// static int logical_exor(APEX_CPU *cpu, int n1, int n2)
// {
//   int res = n1 ^ n2;
//   if (cpu)
//   {
//     CPU_Stage *stage = &cpu->stage[EX2];
//     if (stage)
//       stage->buffer = res;
//   }
//   return res;
// }

// static int logical_or(APEX_CPU *cpu, int n1, int n2)
// {
//   int res = n1 | n2;
//   if (cpu)
//   {
//     CPU_Stage *stage = &cpu->stage[EX2];
//     if (stage)
//       stage->buffer = res;
//   }
//   return res;
// }

union {
  int value;
  char bytes[4];
} number;

static int read_bytes_from_memory(int *data_memory, int address)
{
  for (int i = 0; i < 4; i++)
  {
    number.bytes[i] = data_memory[3 - i + address];
  }
  return number.value;
}

static void write_bytes_to_memory(int *data_memory, int address, int write_number)
{
  number.value = write_number;
  for (int i = 0; i < 4; i++)
  {
    data_memory[3 - i + address] = number.bytes[i];
  }
}

// static void memory_access(APEX_CPU *cpu, int address, int data, CPU_Stage *stage, char mode)
// {
//   switch (mode)
//   {
//   case 'r':
//     stage->buffer = cpu->data_memory[(address)];
//     // cpu->stage[stageId].buffer = read_bytes_from_memory(cpu->data_memory, address);
//     break;
//   case 'w':
//     cpu->data_memory[(address)] = data;
//     // write_bytes_to_memory(cpu->data_memory, address, cpu->stage[stageId].rs1_value);
//     break;
//   }
// }
/********************/

void flush_instructions(APEX_CPU *cpu, int startFromStage)
{
  for (int stageId = startFromStage; stageId >= 0; stageId--)
  {
    if (cpu)
    {
      CPU_Stage *stage = &cpu->stage[stageId];
      if (stage)
        stage->flushed = 1;
    }
  }
}

/*
 *  Execute Stage of APEX Pipeline
 *
 *  Note : You are free to edit this function according to your
 * 				 implementation
 */
// int execute1(APEX_CPU *cpu)
// {
//   CPU_Stage *stage = &cpu->stage[EX1];
//   forward_bus_read(cpu, stage);
//   // forward_bus_read()

//   if (!stage->busy && !stage->stalled && !stage->flushed)
//   {
//     switch (stage->opcode)
//     {
//     case ADD:
//     case SUB:
//     case ADDL:
//     case SUBL:
//     case MUL:
//       // Lock Z Flag
//       lock_flag(ZERO_FLAG, cpu);
//     case LDR:
//     case MOVC:
//     case LOAD:
//     case AND:
//     case OR:
//     case EXOR:
//       lock_register(cpu, stage->rd);
//       break;

//     case BZ:
//     case BNZ:

//       break;

//     case JUMP:
//       adder(stage, stage->rs1_value, stage->imm);
//       break;
//     }

//     // Broadcasting Tag of destination register
//     switch (stage->opcode)
//     {
//     // Register operations that forward data in EX2 stage
//     case ADD:
//     case ADDL:
//     case SUB:
//     case SUBL:
//     case MUL:
//     case MOVC:
//     case EXOR:
//     case OR:
//     case AND:
//       broadcast_tag(cpu, 0, stage->rd);
//       break;
//     default:
//       break;
//     }

//     switch (stage->opcode)
//     {
//     case ADD:
//     case ADDL:
//     case SUB:
//     case SUBL:
//     case MUL:
//       cpu->forward_zero->valid = 0;
//       break;
//     }

//     /* Copy data from Execute latch to Memory latch*/
//     cpu->stage[EX2] = cpu->stage[EX1];
//   }
//   if (ENABLE_DEBUG_MESSAGES)
//   {
//     // print_stage_content("Execute", cpu, stage);
//     print_instruction(cpu, EX1);
//   }

//   if (stage->flushed)
//   {
//     stage->opcode = _BUBBLE;
//     stage->flushed = 0;
//     cpu->stage[EX2] = cpu->stage[EX1];
//   }

//   return 0;
// }

int branch_with_zero(APEX_CPU *cpu, CPU_Stage *stage, int zero_set, int pc_offset)
{
  // CPU_Stage *stage = &cpu->stage[EX2];
  int take_branch = (stage->buffer == zero_set);
  if (take_branch)
  {
    cpu->pc = stage->pc + pc_offset;
    cpu->num_instructions += ((-1 * pc_offset / 4) + 1);
  }
  return take_branch;
}

// int execute2(APEX_CPU *cpu)
// {
//   CPU_Stage *stage = &cpu->stage[EX2];
//   if (!stage->busy && !stage->stalled)
//   {

//     switch (stage->opcode)
//     {
//     case STORE:
//       adder(stage, stage->rs2_value, stage->imm);
//       break;
//     case STR:
//       adder(stage, stage->rs2_value, stage->rs3_value);
//       break;
//     case ADD:
//     case LDR:
//       adder(stage, stage->rs1_value, stage->rs2_value);
//       break;
//     case SUB:
//       adder(stage, stage->rs1_value, -1 * stage->rs2_value);
//       break;
//     case MOVC:
//     case ADDL:
//     case LOAD:
//       adder(stage, stage->rs1_value, stage->imm);
//       break;
//     case SUBL:
//       adder(stage, stage->rs1_value, -1 * (stage->imm));
//       break;
//     case MUL:
//       multiplier(cpu, stage->rs1_value, stage->rs2_value);
//       break;
//     case AND:
//       logical_and(cpu, stage->rs1, stage->rs2);
//       break;
//     case OR:
//       logical_or(cpu, stage->rs1, stage->rs2);
//       break;
//     case EXOR:
//       logical_exor(cpu, stage->rs1, stage->rs2);
//       break;

//     case JUMP:
//       cpu->jump_address_register = stage->pc + 4;
//       int pc_offset = stage->pc - stage->buffer;
//       cpu->num_instructions += ((pc_offset / 4));
//       set_flag(cpu, JUMP_FLAG, 1);
//       cpu->pc = stage->buffer;
//       flush_instructions(cpu, EX1);
//       break;

//     case BNZ:
//       if (branch_with_zero(cpu, 0, stage->imm))
//         flush_instructions(cpu, EX1);
//       break;
//     case BZ:
//       if (branch_with_zero(cpu, 1, stage->imm))
//         flush_instructions(cpu, EX1);
//       break;
//     }

//     switch (stage->opcode)
//     {
//     case ADD:
//     case ADDL:
//     case SUB:
//     case SUBL:
//     case MUL:
//       cpu->forward_zero->valid = 1;
//       cpu->forward_zero->data = stage->buffer == 0;
//       break;
//     default:
//       cpu->forward_zero->valid = 0;
//       break;
//     }

//     switch (stage->opcode)
//     {
//     // Register operations that forward data in EX2 stage
//     case ADD:
//     case ADDL:
//     case SUB:
//     case SUBL:
//     case MUL:
//     case MOVC:
//     case EXOR:
//     case OR:
//     case AND:
//       broadcast_tag(cpu, 1, stage->rd);
//       cpu->forward[0].data = stage->buffer;
//       cpu->forward[0].valid = 1;
//       cpu->forward[0].tag = stage->rd;
//       break;
//     default:
//       break;
//     }

//     /* Copy data from Execute latch to Memory latch*/
//     cpu->stage[MEM1] = cpu->stage[EX2];
//   }
//   if (ENABLE_DEBUG_MESSAGES)
//   {
//     // print_stage_content("Execute", cpu, stage);
//     print_instruction(cpu, EX2);
//   }
//   return 0;
// }

/*
 *  Memory Stage of APEX Pipeline
 *
 *  Note : You are free to edit this function according to your
 * 				 implementation
 */
// int memory1(APEX_CPU *cpu)
// {
//   CPU_Stage *stage = &cpu->stage[MEM1];
//   if (!stage->busy && !stage->stalled)
//   {

//     switch (stage->opcode)
//     {
//     case STORE:
//       memory_access(cpu, stage->buffer, stage->rs1_value, stage, 'w');
//       break;
//     case LOAD:
//       memory_access(cpu, stage->buffer, -1, stage, 'r');
//       break;
//     }

//     switch (stage->opcode)
//     {
//     // Register operations forward data
//     case ADD:
//     case ADDL:
//     case SUB:
//     case SUBL:
//     case MUL:
//     case MOVC:
//     case EXOR:
//     case OR:
//     case AND:
//     case LOAD:
//     case LDR:
//       broadcast_tag(cpu, 2, stage->rd);
//       break;
//     default:
//       break;
//     }

//     /* Copy data from MEM1 latch to MEM2 latch*/
//     cpu->stage[MEM2] = cpu->stage[MEM1];
//   }
//   if (ENABLE_DEBUG_MESSAGES)
//   {
//     // print_stage_content("Memory", cpu, stage);
//     print_instruction(cpu, MEM1);
//   }
//   return 0;
// }
// int memory2(APEX_CPU *cpu)
// {
//   CPU_Stage *stage = &cpu->stage[MEM2];
//   if (!stage->busy && !stage->stalled)
//   {

//     /* Copy data from MEM2 latch to WB latch*/
//     cpu->stage[WB] = cpu->stage[MEM2];
//   }
//   if (ENABLE_DEBUG_MESSAGES)
//   {
//     // print_stage_content("Memory", cpu, stage);
//     print_instruction(cpu, MEM2);
//   }

//   switch (stage->opcode)
//   {
//   case ADD:
//   case ADDL:
//   case SUB:
//   case SUBL:
//   case MUL:
//     cpu->forward_zero->valid = 1;
//     cpu->forward_zero->data = stage->buffer == 0;
//     break;
//   default:
//     cpu->forward_zero->valid = 0;
//     break;
//   }

//   switch (stage->opcode)
//   {
//   // Register operations forward data
//   case ADD:
//   case ADDL:
//   case SUB:
//   case SUBL:
//   case MUL:
//   case MOVC:
//   case EXOR:
//   case OR:
//   case AND:
//   case LOAD:
//   case LDR:
//     broadcast_tag(cpu, 3, stage->rd);
//     cpu->forward[1].data = stage->buffer;
//     cpu->forward[1].valid = 1;
//     cpu->forward[1].tag = stage->rd;

//     // broadcast_tag(cpu, 2, stage->rd);
//     break;
//   default:
//     break;
//   }

//   return 0;
// }

/*
 *  Writeback Stage of APEX Pipeline
 *
 *  Note : You are free to edit this function according to your
 * 				 implementation
 */
// int rob_retire(APEX_CPU *cpu)
// {
//   // CPU_Stage* stage = &cpu->stage[WB];

//   if (is_head_valid())
//     retire(pop_from_rob());

//   if (!stage->busy && !stage->stalled)
//   {

//     switch (stage->opcode)
//     {
//     // Register operations forward data
//     case ADD:
//     case ADDL:
//     case SUB:
//     case SUBL:
//     case MUL:
//     case MOVC:
//     case EXOR:
//     case OR:
//     case AND:
//     case LOAD:
//     case LDR:
//       // cpu->forward[2].data = stage->buffer;
//       // cpu->forward[2].valid = 1;
//       // cpu->forward[2].tag = stage->rd;
//       break;
//     default:
//       break;
//     }

//     if (stage->opcode != _BUBBLE)
//       cpu->ins_completed++;
//   }
//   if (ENABLE_DEBUG_MESSAGES)
//   {
//     // print_stage_content("Writeback", cpu, stage);
//     print_instruction(cpu, WB);
//   }
//   return 0;
// }

int has_instructions(APEX_CPU *cpu)
{
  for (int i = NUM_STAGES - 1; i >= 0; i--)
  {
    if (cpu->stage[i].opcode != _BUBBLE)
      return 1;
  }
  return rob_has_more() || lsq_has_more() || iq_has_more();
  return 0;
}

/*
 *  APEX CPU simulation loop
 *
 *  Note : You are free to edit this function according to your
 * 				 implementation
 */
int APEX_cpu_run(APEX_CPU *cpu, int numCycles)
{
  printf("Executing For %d cycles", numCycles);
  while (numCycles-- > 0 && has_instructions(cpu))
  {

    /* All the instructions committed, so exit */
    // if (cpu->ins_completed == cpu->num_instructions) {

    //   break;
    // }

    // if (ENABLE_DEBUG_MESSAGES) {
    //   printf("--------------------------------\n");
    //   printf("Clock Cycle #: %d\n", cpu->clock);
    //   printf("--------------------------------\n");
    // }
    // print_instructions(cpu);
    cpu->clock++;
    // if (ENABLE_DEBUG_MESSAGES)
      printf("\n----------------------------------- CLOCK CYCLE %d -----------------------------------\n", cpu->clock);

    invalidate_forward_buses();
    rob_retire(cpu);
    // memory2(cpu);

    // memory1(cpu);
    // execute2(cpu);
    execute(cpu);
    move_to_fu(cpu);
    decode(cpu);
    fetch(cpu);

    display_instructions(cpu);

    display_register_contents(cpu);
  }
  display_memory_contents(cpu);
  printf("Simulation Completed after running for %d Cycles", cpu->clock);
  // printf("Simulation Completed after running for %d Cycles (%d - %d)", cpu->clock+1, 0, cpu->clock);

  return 0;
}