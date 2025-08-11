#ifndef LIST_H
#define LIST_H

#include "token.h"
#include <stdio.h>

typedef struct {
    token* data;
    size_t index;
    size_t capacity;
    
} token_list;

typedef struct {
    char** data;
    size_t index;
    size_t capacity;
    
} string_list;

typedef struct {
    struct expression** data;
    size_t index;
    size_t capacity;

} expression_list;

int token_list_create(token_list** list);
int token_list_add(token_list* list, token token);
int print_token_list(token_list* list);


int string_list_create(string_list** list);
int string_list_add(string_list* list, char* string);
int print_string_list(string_list* list);
void destroy_string_list(string_list* list);


int expression_list_create(expression_list** list);
int expression_list_add(expression_list* list, struct expression* expression);
int print_expression_list(expression_list* list);
void destroy_expression_list(expression_list* list);

#endif