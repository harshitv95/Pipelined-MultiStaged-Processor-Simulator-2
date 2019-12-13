#include <stdlib.h>
#include "issue_queue.h"
#include "instruction_commons.h"
#include "cpu_commons.h"
#include "register_file.h"

int iq_index_counter = 0;

int is_iq_full()
{
    int i = ISSUE_QUEUE_CAPACITY - 1;
    while (i >= 0)
    {
        if (issueQueueList[i] != NULL) //check the issueQueue is empty or not
            i--;
        else
            break; //break as soon as it encounters the empty entry in issueQueue
    }
    return i;
}

int insert_to_iq(CPU_Stage *instruction)
{
    int idx;
    if ((idx = is_iq_full()) == -1)
        return -1;
    else
    {
        instruction->iq_index = iq_index_counter;
        issueQueueList[idx] = instruction;
        return iq_index_counter++;
    }
}

CPU_Stage *get_iq_available_instructions()
{

    CPU_Stage *inst_array[ISSUE_QUEUE_CAPACITY];
    for (int i = 0; i < ISSUE_QUEUE_CAPACITY; i++)
    {

        switch (issueQueueList[i]->opcode)
        {
        case ADD:
        case LDR:
        case SUB:
        case MUL:
        case AND:
        case OR:
        case EXOR:
        case STORE:
        {
            if (issueQueueList[i]->rs1_valid && issueQueueList[i]->rs2_valid)
                inst_array[i] = issueQueueList[i];
            else
                inst_array[i] = NULL;
            break;
        }

        case LOAD:
        case ADDL:
        case SUBL:
        {

            if (issueQueueList[i]->rs1_valid)
                inst_array[i] = issueQueueList[i];
            else
                inst_array[i] = NULL;
            break;
        }
        case STR:
        {
            if (issueQueueList[i]->rs1_valid && issueQueueList[i]->rs2_valid &&
                issueQueueList[i]->rs3_valid)
                inst_array[i] = issueQueueList[i];
            else
                inst_array[i] = NULL;
            break;
        }
        case BZ:
        case BNZ:
            //TODO: Change the implementation, Move flag_valid in commons
            //After moving change implementation of BNZ case. otherwise compilation error will occur
            return !(
                // cpu->forward_zero->valid ||
                flag_valid(ZERO_FLAG));
        }

        inst_array[i] = issueQueueList[i];
    }
    return inst_array;
}

CPU_Stage *pop_from_iq(int index)
{
    CPU_Stage *instruction = issueQueueList[index];
    issueQueueList[index] = NULL;
    return instruction;
}

void update_iq_value(int tag, int value) {
    for (int i=0; i<ISSUE_QUEUE_CAPACITY; i++) {
        CPU_Stage* stage = issueQueueList[i];
        if (stage->rs1_valid && stage->p_rs1 == tag) {
            stage->rs1_value = value;
            stage->rs1_valid = 1;
        }
        if (stage->rs2_valid && stage->p_rs2 == tag) {
            stage->rs2_value = value;
            stage->rs2_valid = 1;
        }
        if (stage->rs3_valid && stage->p_rs3 == tag) {
            stage->rs3_value = value;
            stage->rs3_valid = 1;
        }
    }
}