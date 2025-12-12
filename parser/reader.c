#include <stdio.h>
#include <string.h>
#include "parser.h"

// This little helper builds the default path to tok.le.
// If someone moves folders around, this function will complain silently by breaking.
static void build_tokle_path(char *out) {
    strcpy(out, "tok.le");
}

// This function is the public entry point for this module.
// It reads the tok.le file using the parser and produces code.ir.
// No main() here because the actual compiler will call this.
void reader_process_tokens() {

    char tokpath[256];
    build_tokle_path(tokpath);

    printf("Reading token file: %s\n", tokpath);

    // Hand the file to the parser
    parse_tokens(tokpath);

    // At this point code.ir is created by parser.c
    printf("Token processing finished.\n");
}
