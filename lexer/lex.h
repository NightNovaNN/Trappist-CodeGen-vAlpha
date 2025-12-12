#ifndef LEX_H
#define LEX_H

// put the necessary includes here for some reason
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
// real shit starts here
typedef enum {
    TOK_INT,     // holy this is gonna be a pain
    TOK_STR,
    TOK_IDENT,
    TOK_NUMBER,
    TOK_STRING,
    TOK_PRINT,

    TOK_COLON,      // :
    TOK_DCOLON,     // ::
    TOK_EQUAL,      // =
    TOK_SEMI,       // ;
    TOK_COMMA,      // ,
    TOK_LPAREN,     // (
    TOK_RPAREN,     // )
    TOK_LBRACK,     // [
    TOK_RBRACK,     // ]
    TOK_STAR,       // *
    TOK_HASH,       // #
    TOK_SS,      // //

    TOK_EOF,
    TOK_UNKNOWN
} TokenType;

// some datatype for code
typedef struct {
    TokenType type;
    char text[128];
} Token;

// main funcs i guess
void init_lexer(const char *source);
Token get_next_token();
void init_lexer_from_file(const char *filename);       // for reading from file
void free_lexer();
#endif // LEX_H