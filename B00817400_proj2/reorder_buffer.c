#include "reorder_buffer.h"
#include <stdlib.h>
// #include <stdbool.h>

// void init_rob(struct queue *reorderBuffer)
// {
//   reorderBuffer -> head = -1;
//   reorderBuffer -> tail = 0;
// }

// bool isFull(struct queue* reorderBuffer) {
//   if(reorderBuffer->tail == reorderBuffer->head){
//       printf("Queue empty.\n");
//       return 1;
//   } else {
//     return 0;
//   }
// }

// int enqueue(struct queue * reorderBuffer, int element) {
//   if (reorderBuffer -> tail == 7 && isFull(reorderBuffer)) {
//     // stall
//   } else if (reorderBuffer -> tail == 7 ) {
//     reorderBuffer -> tail = 0;
//   } else {
//     reorderBuffer-> array[reorderBuffer->tail] = element;
//     (reorderBuffer->tail)++;
//   }
// }

// int dequeue(struct queue *reorderBuffer, int *status)
// {
//   if(reorderBuffer-> head == -1 && reorderBuffer -> tail == 0){
//       *status=0;
//       return -1;
//   } else{
//       *status=1;
//       int item = reorderBuffer->array[++reorderBuffer->head%8];
//       return item;
//   }
// }


void init_rob()
{
  rob = (REORDER_BUFFER *)malloc(sizeof(REORDER_BUFFER));
  rob->buffer = (ROB_Entry *)malloc(sizeof(ROB_Entry) * REORDER_BUFFER_CAPACITY);
  rob->tail = 0;
  rob->head = -1;
  rob->count = 0;
  for (int i=0; i<REORDER_BUFFER_CAPACITY; i++) {
    rob->buffer[i].valid = 0;
    rob->buffer[i].allocated = 0;
  }
}

int is_rob_head_valid()
{
  return (rob->head != -1) && rob->buffer[rob->head].valid;
}

int update_rob_value(int phy_register_address, int value)
{
  for (int i=0; i<REORDER_BUFFER_CAPACITY; i++) {
    ROB_Entry* entry = &rob->buffer[i];
    if (entry->allocated && !(entry->valid)) {
      if (entry->instruction->p_rd == phy_register_address) {
        entry->instruction->buffer = value;
        entry->valid = 1;
      }
    }
  }
}

static
int _instruction_valid_by_default_(CPU_Stage *instruction)
{
  switch (instruction->opcode)
  {
  case BZ:
  case BNZ:
  case STORE:
  case STR:
  case JUMP:
  case HALT:
  case NOP:
  case _BUBBLE:
    return 1;
  default:
    return 0;
  }
}

int insert_to_rob(CPU_Stage *instruction)
{
  if (!is_rob_full())
  {
    rob->buffer[rob->tail].instruction = instruction;
    rob->buffer[rob->tail].allocated = 1;
    rob->buffer[rob->tail++].valid = _instruction_valid_by_default_(instruction);
    if (rob->tail >= REORDER_BUFFER_CAPACITY)
      rob->tail = rob->tail % REORDER_BUFFER_CAPACITY;
    rob->count++;
    if (rob->head == -1)
      rob->head = 0;
    return 1;
  }
  else
    return 0;
}

int is_rob_full()
{
  return rob->count >= REORDER_BUFFER_CAPACITY;
}

int rob_has_more()
{
  return (rob->head != -1) &&
         (rob->count > 0);
}

ROB_Entry *pop_from_rob()
{
  if (!rob_has_more())
    return NULL;
  ROB_Entry *entry = &rob->buffer[rob->head];
  rob->buffer[rob->head++].allocated = 0;
  rob->count--;
  if (rob->head >= REORDER_BUFFER_CAPACITY)
    rob->head = rob->head % REORDER_BUFFER_CAPACITY;
  return entry;
}
