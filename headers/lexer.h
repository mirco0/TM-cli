#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "list.h"
#define MAX_VAR_NAME 256

int tokenize(char* string, int string_size, token_list** list);
int insert_next_token(char* string, int start_position, int string_size, int* line, int ch, token_list* list);
int get_identifier(char identifier[MAX_VAR_NAME], int var_name_size);
int add_token(enum TOKEN_TYPE type, char* data, size_t data_size, int line_num, int char_num, token_list* tokens);
int character(char c);

#endif