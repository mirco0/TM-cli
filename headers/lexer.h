#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "list.h"
#define MAX_VAR_NAME 256

typedef struct {
    int line;
    int inline_char_index;
    int commented;
    size_t real_char_num;
} lexing_info;

int tokenize(const char* string, int string_size, token_list** list, lexing_info* info);
int insert_next_token(const char* string, int start_position, int string_size, token_list* list, lexing_info* info);
int get_identifier(char identifier[MAX_VAR_NAME], int var_name_size);
int add_token(enum TOKEN_TYPE type, char* data, size_t data_size, token_list* tokens, const lexing_info* info);
int character(char c);
void write_end(token_list*, const lexing_info* info);
int create_lexing_info(lexing_info** info);

#endif