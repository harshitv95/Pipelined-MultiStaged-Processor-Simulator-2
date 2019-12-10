#include "reorder_buffer.h"

int _is_head_valid() {

    /* Update register file */
    switch(stage->opcode) {
      case _BUBBLE:
      case STORE:
      case STR:
      case NOP:
      case JUMP:
      case BNZ:
      case BZ:
      case HALT:
        // Do Nothing in WB Stage
        break;

      case ADD:
      case ADDL:
      case SUB:
      case SUBL:
      case MUL:
        // Set Z Flag for Arithmetic operations
        set_flag(cpu, ZERO__FLAG, (stage->buffer == 0));
      default:
        register_wite(stage, cpu);
        break;
    }
}