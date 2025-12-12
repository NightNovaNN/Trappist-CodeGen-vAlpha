#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "../lexer/lex.h"   // for TokenType enum

// ======================= TOKEN STORAGE =======================
//
// Yes, this is a static array.
// If your program exceeds 4096 tokens, congrats,
// you're writing more code than the average college student.
//

typedef struct {
    TokenType type;
    char text[128];
} PToken;

static PToken tokens[4096];
static int tok_count = 0;
static int pos = 0;


// ======================= TOKEN UTILITIES =======================
//
// These are the usual suspects: peek, advance, match.
//

static PToken *peek() {
    return &tokens[pos];
}

static PToken *advance() {
    return &tokens[pos++];
}

static int match(TokenType t) {
    if (peek()->type == t) {
        advance();
        return 1;
    }
    return 0;
}


// ======================= IR STORAGE =======================
//
// Think of this as the compiler’s gossip journal.
// Every declaration the parser finds gets logged here.
//

static IRNode ir[2048];
static int ir_count = 0;

static void add_ir(IRNode node) {
    ir[ir_count++] = node;
}


// ======================= WRITE IR TO FILE =======================
//
// code.ir is where the parser dumps its secrets.
// The bytecode generator will read this later.
//

static void write_ir_file(const char *filename) {
    FILE *out = fopen(filename, "w");
    if (!out) {
        printf("Cannot write IR file: %s\n", filename);
        return;
    }

    for (int i = 0; i < ir_count; i++) {
        IRNode *n = &ir[i];

        if (n->type == IR_VAR_DECL) {
            // Format:
            // VAR <name> <dtype> <value> <mutability>
            fprintf(out, "VAR %s %s %s %s\n",
                n->name, n->dtype, n->value, n->mutability);
        }
    }

    fclose(out);
    printf("IR written to %s\n", filename);
}


// ======================= PARSER FUNCTIONS =======================
//
// Parses:
//      int x: t8 = 45 :: mut;
//
// If the user writes anything too weird, the parser will complain.
// The comments will probably complain harder.
//

static void parse_var_decl() {
    advance();  // eat 'int'

    // identifier
    PToken *name = advance();
    if (name->type != TOK_IDENT) {
        printf("Expected identifier, got '%s'\n", name->text);
        return;
    }

    // :
    if (!match(TOK_COLON)) {
        printf("Expected ':' after identifier\n");
        return;
    }

    // dtype
    PToken *dtype = advance();
    if (dtype->type != TOK_IDENT) {
        printf("Expected dtype such as t8 or t16\n");
        return;
    }

    // =
    if (!match(TOK_EQUAL)) {
        printf("Expected '=' after dtype\n");
        return;
    }

    // value
    PToken *value = advance();
    if (value->type != TOK_NUMBER && value->type != TOK_IDENT) {
        printf("Expected value after '='\n");
        return;
    }

    // ::
    if (!match(TOK_DCOLON)) {
        printf("Expected '::' after value\n");
        return;
    }

    // mutability
    PToken *mut = advance();
    if (mut->type != TOK_IDENT) {
        printf("Expected mutability specifier (mut or imut)\n");
        return;
    }

    // ;
    if (!match(TOK_SEMI)) {
        printf("Expected ';' at end of declaration\n");
        return;
    }

    // Add to IR
    IRNode n;
    n.type = IR_VAR_DECL;

    strcpy(n.name, name->text);
    strcpy(n.dtype, dtype->text);
    strcpy(n.value, value->text);
    strcpy(n.mutability, mut->text);

    add_ir(n);
}


static void parse_all() {
    while (peek()->type != TOK_EOF) {
        if (peek()->type == TOK_INT) {
            parse_var_decl();
        } else {
            // Unknown syntax. The parser is confused and so are we.
            printf("Unknown syntax at token '%s'\n", peek()->text);
            advance();
        }
    }
}


// ======================= TOKEN FILE LOADER =======================
//
// Loads tok.le into tokens[].
// tok.le is basically the output of the lexer spilled into a file.
// The parser picks it up and pretends it's smart.
//

void parse_tokens(const char *tokfile) {
    FILE *f = fopen(tokfile, "r");
    if (!f) {
        printf("Cannot open token file: %s\n", tokfile);
        return;
    }

    int type;
    char text[128];

    while (fscanf(f, "%d %127s", &type, text) == 2) {
        tokens[tok_count].type = (TokenType)type;
        strcpy(tokens[tok_count].text, text);
        tok_count++;
    }

    fclose(f);

    // Parse the tokens into IR
    parse_all();

    // Now write IR to file
    write_ir_file("code.ir");
}
