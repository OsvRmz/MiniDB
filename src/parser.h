#ifndef PARSER_H
#define PARSER_H

void parse_tokens(const char *line, char ***out_argv, int *out_argc);
void free_tokens(char **argv, int argc);

#endif 
