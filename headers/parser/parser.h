#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "expression.h"
#include "../list.h"

int match(token_list* tokens, size_t* start, token* dest_token, int count, ...);
token* peek(token_list* tokens, size_t start);
token* consume(token_list* tokens, size_t* start);

expression* parse(token_list* tokens);

expression* parse_statement(token_list* tokens,size_t* start);
expression* parse_assignment(token_list* tokens,size_t* start);
expression* parse_instruction(token_list* tokens,size_t* start);

expression* parse_set_op(token_list* tokens, size_t* start);
expression* parse_set_difference(token_list* tokens, size_t* start);
expression* parse_set_union(token_list* tokens, size_t* start);
expression* parse_set_intersection(token_list* tokens, size_t* start);
expression* parse_set_elements(token_list* tokens, size_t* start);
expression* parse_domain(token_list* tokens, size_t* start);
expression* parse_variables(token_list* tokens, size_t* start);

expression* new_binary_expression();
expression* new_binary_expression();

void print_parser_error(char* expected, token tk);
// instruction* parse_instructions(char* string, size_t string_size, size_t* arr_size);
// char* stringify_instructions(instruction* instructions, size_t instructions_size);

#endif