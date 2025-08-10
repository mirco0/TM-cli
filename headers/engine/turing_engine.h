#ifndef TURING_ENGINE_H
#define TURING_ENGINE_H

#include "environment.h"
#include "../parser/expression.h"
#include "../list.h"
#include <stddef.h>

typedef struct {
    char* state;
    tape* tape;
    size_t steps;
    size_t max_steps;
} tm_state;

int init_engine(tm_state** machine);
void execute(const context* instructions, tm_state* machine);
set* evaluate(const context* context, expression* exp);
set* eval_binary(const context* context, expression* exp);
void set_variable_expression(context* context, expression* expression);
void interpret_program(context* context, expression* program);
void evaluate_instructions(const context* context, expression* exp);
int next_combination(char** dest, string_list* set, int num_variables, int iteration);

#endif