#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../hashtable.h"
#include "../parser/expression.h"

typedef struct {
    hashtable* table;
    hashtable* instructions_table;
} context;


struct cell {
    struct cell* prev;
    char* content;
    struct cell* next; 
};
typedef struct cell tape;

context* create_context();
int context_define_variable(context* context, char* name, void* value);
void* context_get_variabile(const context* context, char* name);
int context_define_instruction(const context* context, const expression* instruction_expression);
instruction_expression* context_get_instruction(const context* context, char* state, char* read);
void destroy_context(context* context);

tape* create_empty_tape();
tape* create_tape(const string_list* values);
void move_tape(tape** t, enum TOKEN_TYPE move);
char* tape_to_string(const tape* tape);
void destroy_tape(tape *tape);

#endif