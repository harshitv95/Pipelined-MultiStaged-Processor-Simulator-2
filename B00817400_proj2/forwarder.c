#include "forwarder.h"
#include "issue_queue.h"
#include "load_store_queue.h"
#include "reorder_buffer.h"


int forward_bus_counter = 0;
int fwd_broadcast_counter = 0;

/* Forwarding */
// void broadcast_tag(int forward_bus, int tag)
// {
//   broadcast[forward_bus].tag = tag;
//   broadcast[forward_bus].valid = 1;
// }

void forward_data(CPU_Stage *stage)
{
    forward_to_rob(stage);
    forward_to_lsq(stage);
    forward_to_iq(stage);
}

void forward_to_lsq(CPU_Stage *stage)
{
    update_lsq_value(stage->p_rd, stage->buffer);
    opcode op = stage->opcode;
    if (op == STORE || op == STR || op == LDR || op == LOAD)
        update_lsq_mem_address(stage->lsq_index, stage->mem_address);
}

void forward_to_iq(CPU_Stage *stage)
{
    update_iq_value(stage->p_rd, stage->buffer);
}

void forward_to_rob(CPU_Stage *stage)
{
    update_rob_value(stage->p_rd, stage->buffer);
}

void init_forward_buses()
{
    forward_zero = (FWD_BUS *)malloc(sizeof(FWD_BUS));
    forward_zero->valid = 0;
    invalidate_forward_buses();
    // forward[0].valid = forward[1].valid = 0;
    // for (int i=0; i<NUM_FWD_BUSES; i++) {
    //     forward[i].valid = 0;
    // }
}

void invalidate_forward_buses()
{
    for (int i = 0; i < NUM_FWD_BUSES; i++)
        forward[i].valid = 0;
    for (int i = 0; i < NUM_FWD_BUSES * 2; i++)
        broadcast[i] = -1;
    forward_zero->valid = 0;
    forward_bus_counter = 0;
    fwd_broadcast_counter = 0;
}

void broadcast_tag(int tag) {
    broadcast[fwd_broadcast_counter++] = tag;
}

int check_forwarded_register(int regNum)
{
  for (int i = 0; i < NUM_FWD_BUSES; i++)
  {
    if (broadcast[i] == regNum)
      return i;
  }
  return -1;
}

int check_forwarded_bus_data(int regNum)
{
  for (int i = 0; i < NUM_FWD_BUSES; i++)
  {
    if (forward[i].valid && forward[i].tag == regNum)
      return i;
  }
  return -1;
}
