// Yay we back at it again
#ifndef PARSER_H
#define PARSER_H

typedef enum {   // Intermediate Representation Types cuz why not
    IR_VAR_DECL,
    IR_ASSIGN,
    IR_PRINT
} IRType;     // types of IR nodes (name)

typedef struct {    // Intermediate Representation Node (not AST i hate it)
    IRType type;    // type of IR node (prolly dtype of VAR_DECL type heck)
    char name[64];   // variable name buffer
    char value[128];    // value buffer (for assignments and prints)
    char mutability[8];   // "mut" or "imut"
    char dtype[8];        // t8, t16, t32, etc
} IRNode;

void parse_tokens(const char *tokfile);    // function to parse tokens from a file and generate IR
void free_ir_nodes();   // function to free allocated IR nodes

#endif
