#define _GNU_SOURCE
#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// parse respecting quoted strings ("..."), tokens separated by whitespace.
void parse_tokens(const char *line, char ***out_argv, int *out_argc) {
    char **argv = NULL;
    int argc = 0;
    const char *p = line;

    while (*p) {
        while (isspace((unsigned char)*p)) p++;
        if (!*p) break;

        char *token = NULL;
        if (*p == '\"') {
            p++; 
            const char *start = p;
            while (*p && *p != '\"') p++;
            size_t len = p - start;
            token = (char*)malloc(len + 1);
            memcpy(token, start, len);
            token[len] = '\0';
            if (*p == '\"') p++; 
        } else {
            const char *start = p;
            while (*p && !isspace((unsigned char)*p)) p++;
            size_t len = p - start;
            token = (char*)malloc(len + 1);
            memcpy(token, start, len);
            token[len] = '\0';
        }
        char **tmp = (char**)realloc(argv, sizeof(char*) * (argc + 1));
        if (!tmp) { free(token); break; }
        argv = tmp;
        argv[argc++] = token;
    }

    *out_argv = argv;
    *out_argc = argc;
}

void free_tokens(char **argv, int argc) {
    if (!argv) return;
    for (int i = 0; i < argc; ++i) free(argv[i]);
    free(argv);
}
