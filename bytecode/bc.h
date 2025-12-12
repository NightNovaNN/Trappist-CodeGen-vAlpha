#ifndef TRAPPIST_BC_H
#define TRAPPIST_BC_H

#include <stdio.h>

// -------------------------------------------------------------
// BYTECODE OPCODES
// This is the instruction set. Compact, readable, and just
// functional enough to get us into trouble later.
// -------------------------------------------------------------
typedef enum {
    BC_LOADCONST,   // push integer constant
    BC_STOREVAR,    // store into variable
    BC_LOADVAR,     // load variable into register
    BC_PRINT        // print value in register
} OpCode;

typedef struct {
    OpCode op;
    int    arg_int;
    char   arg_str[64];
} BCInstr;

// -------------------------------------------------------------
// Functions exported by codegen
// -------------------------------------------------------------

// Reads parser/code.ir and fills internal IR buffer
void bc_generate_from_ir(const char *ir_path, const char *output_trb);

// Writes bytecode to .trb file
void bc_write_trb(const char *filename, BCInstr *bc, int count);

// Optional debugging helper
void bc_dump(BCInstr *code, int count);

#endif
