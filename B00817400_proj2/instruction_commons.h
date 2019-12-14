
#ifndef _APEX_INST_COMM_H_
#define _APEX_INST_COMM_H_


typedef enum {
    // Arithmetic
    ADD,
    ADDL,
    SUB,
    SUBL,
    MUL,

    // Data-Moving
    MOVC,
    STORE,
    STR,
    LOAD,
    LDR,

    // Logical
    EXOR,
    AND,
    OR,

    // Control Flow
    JUMP,
    BZ,
    BNZ,
    HALT,

    // Blanks (Due to Stalls etc.)
    NOP, // To be used in .asm input file
    _BUBBLE, // Only to be generated by the simulated CPU, do not use it in input.asm file

    // NOT AN INSTRUCTION
    __NUM_INSTRUCTIONS
} opcode;

/* Format of an APEX instruction  */
typedef struct APEX_Instruction
{
  // char opcode[128];	// Operation Code
  opcode opcode;
  int rd;		    // Destination Register Address
  int rs1;		    // Source-1 Register Address
  int rs2;		    // Source-2 Register Address
  int rs3;		    // Source-2 Register Address
  int imm;		    // Literal Value
    
  // For display purpose only
  int instNum;
  char instText[128];
} APEX_Instruction;

#endif