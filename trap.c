#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lex.h"              // lexer module
#include "parser/parser.h"        // parser module
#include "bytecode/bc.h"             // bytecode generator module
#include "parser/reader.h"        // the bridge that loads tok.le and runs parser

// The compiler takes:
//    ./trap input.trp output.trb
// and performs:
//    1. lexing -> tok.le
//    2. parsing -> code.ir
//    3. bytecode -> output.trb
//
// This file is the conductor of the compiler orchestra.
// All other files are musicians who behave (hopefully).

int main(int argc, char **argv) {

    if (argc < 3) {
        printf("Usage: %s <input.trp> <output.trb>\n", argv[0]);
        printf("Trappist Compiler Driver\n");
        return 1;
    }

    const char *input_file = argv[1];
    const char *output_trb = argv[2];

    printf("=== TRAPPIST COMPILER ===\n");
    printf("Source: %s\n", input_file);
    printf("Output Bytecode: %s\n", output_trb);

    // ------------------------------------------------------------
    // 1. LEXER PHASE: input.trp -> tok.le
    // ------------------------------------------------------------
    printf("[1] Lexing source...\n");

    init_lexer_from_file(input_file);

    // Burn through tokens so tok.le gets fully generated.
    Token t;
    while ((t = get_next_token()).type != TOK_EOF) {
        /* nothing here except letting the lexer work */
    }

    free_lexer();
    printf("[1] Lexing complete. Tokens stored in tok.le\n");


    // ------------------------------------------------------------
    // 2. PARSER PHASE: tok.le -> code.ir
    // ------------------------------------------------------------
    printf("[2] Parsing tokens...\n");

    reader_process_tokens();  // loads tok.le and writes code.ir

    printf("[2] Parsing complete. IR stored in code.ir\n");


    // ------------------------------------------------------------
    // 3. BYTECODE PHASE: code.ir -> output.trb
    // ------------------------------------------------------------
    printf("[3] Generating bytecode...\n");

    bc_generate_from_ir("code.ir", output_trb);

    printf("[3] Bytecode written to %s\n", output_trb);


    // ------------------------------------------------------------
    printf("=== Compilation Finished ===\n");

    return 0;
}
