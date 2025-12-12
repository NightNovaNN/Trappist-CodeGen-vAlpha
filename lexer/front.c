#include "lex.h"
#define RET(tok) do { Token __t = tok; log_token(__t); return __t; } while(0)

static const char *src;    // the source code
static int pos = 0;        // for keeping track of the pointer
static FILE *tok_file = NULL;   // file to dump tokens, like a crime scene log

// helper to read a whole file into memory (private to lexer)
static char *read_file(const char *filename) {
    FILE *f = fopen(filename, "rb");   // open a binary file (safer)
    if (!f) {
        printf("Can't open file %s\n", filename);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);      // file thickness
    fseek(f, 0, SEEK_SET);

    char *buf = malloc(size + 1);
    if (!buf) {
        printf("Malloc failed to allocate %ld bytes\n", size);
        fclose(f);
        return NULL;
    }

    fread(buf, 1, size, f);
    buf[size] = '\0';          // null-terminate the chaos

    fclose(f);
    return buf;
}

// NEW FUNCTION: init lexer using A FILE instead of a string
void init_lexer_from_file(const char *filename) {
    char *data = read_file(filename);  // read the file
    if (!data) {
        printf("File Read failed\n");
        exit(1);
    }

    init_lexer(data);   // feed file contents into existing init
}

void init_lexer(const char *source) {
    src = malloc(strlen(source) + 1);
    strcpy((char*)src, source);
    pos = 0;

    tok_file = fopen("tok.le", "w"); // open token dump file
    if (!tok_file) {
        printf("Can't open tok.le \n");
        exit(1);
    }
}

void free_lexer() {
    free((void*)src);   // free the source code memory
    if (tok_file) fclose(tok_file);   // close file like a gentleman
}


// helpers
static char peek() {  // lookahead like in horror games
    return src[pos];
}

static char advance() {   // move forward
    return src[pos++];
}

static void log_token(Token t) {
    if (!tok_file) return;
    fprintf(tok_file, "%d %s\n", t.type, t.text); 
    // writes "TYPE TEXT"
    // e.g. 1 int
}

static int match(char c) {   // conditional advance
    if (src[pos] == c) {
        pos++;
        return 1;
    }
    return 0;
}

Token make_tok(TokenType type, const char *text) {      // the weird datatype we defined in lex.h
    Token t;                     // make a token
    t.type = type;               // assign type
    strcpy(t.text, text);        // assign text

    // log token to tok.le(snitch behavior)
    if (tok_file) {
        fprintf(tok_file, "%d %s\n", t.type, t.text);
    }

    return t;                   // return token like a good citizen
}

Token get_next_token() {    
    // skip whitespace
    while (isspace(peek())) advance();   // skip spaces

    char c = peek();
    if (c == '\0') return make_tok(TOK_EOF, "EOF");   // makes a token and returns it

    // identifiers + keywords
    if (isalpha(c) || c == '_') {   // i have no idea tf isalpha does but i found it online so yeah
        char buf[128];   // buffer for identifier 
        int i = 0;    // index for buffer (again)

        while ((isalnum(peek()) || peek() == '_') && i < 127)  // isalnum checks for alphanumeric characters
            buf[i++] = advance();

        buf[i] = '\0';     // null-terminate the string

        if (strcmp(buf, "int") == 0) return make_tok(TOK_INT, buf);     // checks for keywords
        if (strcmp(buf, "str") == 0) return make_tok(TOK_STR, buf);    // same here
        if (strcmp(buf, "printf") == 0) return make_tok(TOK_PRINT, buf);

        return make_tok(TOK_IDENT, buf);     // if not a keyword, it's an identifier
    }

    // numbers
    if (isdigit(c)) {
        char buf[128];
        int i = 0;

        while (isdigit(peek()) && i < 127)   // checks for digits
            buf[i++] = advance();

        buf[i] = '\0';
        return make_tok(TOK_NUMBER, buf);
    }

    // strings
    if (c == '"') {
        advance(); // skip "

        char buf[128];
        int i = 0;

        while (peek() != '"' && peek() != '\0' && i < 127) {  
            // minimal escape sequence handling for \" (for future sanity)
            if (peek() == '\\' && src[pos+1] == '"') {
                advance(); // skip backslash
                buf[i++] = advance(); // take the quote char
            } else {
                buf[i++] = advance();
            }
        }
        buf[i] = '\0';

        if (peek() == '"') advance(); // skip closing "

        return make_tok(TOK_STRING, buf);
    }

    // double-colon ::
    if (c == ':' && src[pos+1] == ':') {   // for the immutable namespace operator
        advance(); advance();
        return make_tok(TOK_DCOLON, "::");     // return double-colon token
    }

    // comments //
    if (c == '/' && src[pos+1] == '/') {       // bruh same as C
        advance(); advance(); // skip "//"

        // Skip until newline because comments are useless (just like half of mine)
        while (peek() != '\n' && peek() != '\0')
            advance();

        // recursion because we ball hard
        return get_next_token();  
    }

    // single symbols
    switch (advance()) {
        case ':': return make_tok(TOK_COLON, ":");
        case '=': return make_tok(TOK_EQUAL, "=");
        case ';': return make_tok(TOK_SEMI, ";");
        case ',': return make_tok(TOK_COMMA, ",");
        case '(': return make_tok(TOK_LPAREN, "(");
        case ')': return make_tok(TOK_RPAREN, ")");
        case '[': return make_tok(TOK_LBRACK, "[");
        case ']': return make_tok(TOK_RBRACK, "]");
        case '*': return make_tok(TOK_STAR, "*");
        case '#': return make_tok(TOK_HASH, "#");
    }

    // unknown tokens enter the shadow realm
    char tmp[2] = { c, '\0' };
    return make_tok(TOK_UNKNOWN, tmp);
}
