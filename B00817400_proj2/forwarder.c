#include "forwarder.h"
#include "issue_queue.h"
#include "load_store_queue.h"
#include "reorder_buffer.h"

void forward(CPU_Stage* stage) {
    forward_to_rob(stage);
}

void forward_to_lsq(CPU_Stage* stage) {
    update_lsq_value(stage->lsq_index, stage->buffer);
}

void forward_to_iq(CPU_Stage* stage) {
    update_iq_value(stage->p_rd, stage->buffer);
}

void forward_to_rob(CPU_Stage* stage) {
    update_rob_value(stage->rob_index, stage->buffer);
}