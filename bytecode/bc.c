#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bc.h"

// Internal bytecode buffer
// If the user generates more instructions than this,
// they are clearly writing a novel instead of code.
static BCInstr bc[4096];
static int bc_count = 0;


// Append a bytecode instruction
static void emit(OpCode op, int val, const char *str)
{
    if (bc_count >= 4096) {
        printf("Bytecode buffer overflow.\n");
        return;
    }

    bc[bc_count].op = op;
    bc[bc_count].arg_int = val;

    if (str)
        strcpy(bc[bc_count].arg_str, str);
    else
        bc[bc_count].arg_str[0] = '\0';

    bc_count++;
}


// -------------------------------------------------------------
// bc_generate_from_ir
// Reads code.ir and generates bytecode into memory.
// This is where IR goes to become a Real Boy.
// -------------------------------------------------------------
void bc_generate_from_ir(const char *ir_path, const char *output_trb)
{
    FILE *f = fopen(ir_path, "r");
    if (!f) {
        printf("Cannot open IR file: %s\n", ir_path);
        return;
    }

    char cmd[16];
    char name[64];
    char dtype[64];
    char value[64];
    char mut[64];

    // reset bc count
    bc_count = 0;

    while (fscanf(f, "%15s", cmd) == 1) {

        if (strcmp(cmd, "VAR") == 0) {

            // Read 4 fields: <name> <dtype> <value> <mut>
            if (fscanf(f, "%63s %63s %63s %63s",
                       name, dtype, value, mut) != 4) {
                printf("Invalid VAR instruction.\n");
                continue;
            }

            // Translate IR → bytecode.
            //
            // VAR x t8 45 mut
            //
            // becomes:
            // A0 45
            // A1 x

            emit(BC_LOADCONST, atoi(value), NULL);
            emit(BC_STOREVAR, 0, name);
        }

        // future expansions go here: PRINT, expressions, etc.
    }

    fclose(f);

    // Write the bytecode into filename.trb
    bc_write_trb(output_trb, bc, bc_count);
}


// -------------------------------------------------------------
// bc_write_trb
// Writes bytecode into a real file for execution later.
// A .trb file is basically "Trappist Bytecode", which sounds
// way more official than it actually is.
// -------------------------------------------------------------
void bc_write_trb(const char *filename, BCInstr *code, int count)
{
    FILE *out = fopen(filename, "w");
    if (!out) {
        printf("Cannot write TRB file: %s\n", filename);
        return;
    }

    for (int i = 0; i < count; i++) {
        BCInstr *b = &code[i];

        switch (b->op) {

        case BC_LOADCONST:
            fprintf(out, "A0 %d\n", b->arg_int);
            break;

        case BC_STOREVAR:
            fprintf(out, "A1 %s\n", b->arg_str);
            break;

        case BC_LOADVAR:
            fprintf(out, "A2 %s\n", b->arg_str);
            break;

        case BC_PRINT:
            fprintf(out, "AVX\n");
            break;
        }
    }

    fclose(out);
    printf("Bytecode written to %s\n", filename);
}


// -------------------------------------------------------------
// Debug helper: not required, but useful when the VM acts up
// -------------------------------------------------------------
void bc_dump(BCInstr *code, int count)
{
    printf("=== BYTECODE DUMP ===\n");

    for (int i = 0; i < count; i++) {
        BCInstr *b = &code[i];

        switch (b->op) {
        case BC_LOADCONST:
            printf("A0 %d\n", b->arg_int);
            break;

        case BC_STOREVAR:
            printf("A1 %s\n", b->arg_str);
            break;

        case BC_LOADVAR:
            printf("A2 %s\n", b->arg_str);
            break;

        case BC_PRINT:
            printf("AVX\n");
            break;
        }
    }
}
