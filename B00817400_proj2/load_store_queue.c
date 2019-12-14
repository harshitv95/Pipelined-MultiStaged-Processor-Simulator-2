#include <stdlib.h>
#include "load_store_queue.h"
#include "cpu_commons.h"

int lsq_index_counter = 0;

void init_lsq()
{
    lsq = (LOAD_STORE_QUEUE *)malloc(sizeof(LOAD_STORE_QUEUE));
    lsq->queue = (LSQ_Entry *)malloc(sizeof(LSQ_Entry) * REORDER_BUFFER_CAPACITY);
    lsq->tail = 0;
    lsq->head = -1;
    lsq->count = 0;
    for (int i = 0; i < REORDER_BUFFER_CAPACITY; i++)
    {
        lsq->queue[i].value_valid = 0;
        lsq->queue[i].mem_address_valid = 0;
        lsq->queue[i].allocated = 0;
    }
}

int is_lsq_full()
{
    return lsq->count == LOAD_STORE_QUEUE_CAPACITY;
}

int insert_to_lsq(CPU_Stage *instruction)
{
    if (!is_lsq_full())
    {
        int idx = lsq->tail;
        lsq->queue[lsq->tail].instruction = instruction;
        lsq->queue[lsq->tail].allocated = 1;
        lsq->queue[lsq->tail].mem_address_valid = 0;
        lsq->queue[lsq->tail++].value_valid = 0;
        if (lsq->tail >= REORDER_BUFFER_CAPACITY)
            lsq->tail = lsq->tail % REORDER_BUFFER_CAPACITY;
        lsq->count++;
        if (lsq->head == -1)
            lsq->head = 0;
        return idx;
    }
    else
        return -1;
}

int _lsq_inst_ready_(LSQ_Entry *entry)
{
    switch (entry->instruction->opcode)
    {
    case LOAD:
    case LDR:
        return entry->mem_address_valid;
        break;

    case STORE:
    case STR:
        return entry->mem_address_valid && entry->value_valid;

    default:
        return 1;
        break;
    }
}

int is_lsq_head_valid()
{
    return (lsq->head != -1) &&
           (_lsq_inst_ready_(&lsq->queue[lsq->head]));
}

int lsq_has_more()
{
    return (lsq->head != -1) &&
           (lsq->count > 0);
}

LSQ_Entry *pop_from_lsq()
{
    if (!lsq_has_more())
        return NULL;
    LSQ_Entry *entry = &lsq->queue[lsq->head];
    lsq->queue[lsq->head++].allocated = 0;
    lsq->count--;
    if (lsq->head >= LOAD_STORE_QUEUE_CAPACITY)
        lsq->head = lsq->head % LOAD_STORE_QUEUE_CAPACITY;
    return entry;
}

void update_lsq_mem_address(int lsq_index, int mem_address)
{
    for (int i = 0; i < LOAD_STORE_QUEUE_CAPACITY; i++)
    {
        if (lsq->queue[i].instruction->lsq_index == lsq_index)
        {
            lsq->queue[i].instruction->mem_address = mem_address;
            lsq->queue[i].mem_address_valid = 1;
            return;
        }
    }
}

void update_lsq_value(int phy_reg_address, int value)
{
    for (int i = 0; i < LOAD_STORE_QUEUE_CAPACITY; i++)
    {
        if (!lsq->queue[i].allocated) continue;
        CPU_Stage *instruction = lsq->queue[i].instruction;
        switch (instruction->opcode)
        {
        case STR:
        case STORE:
            instruction->buffer = value;
            lsq->queue[i].value_valid = 1;
            break;

        default:
            break;
        }
    }
}