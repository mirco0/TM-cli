#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../token.h"
#include "../set.h"
#include <stdio.h>

typedef enum {
    BINARY,         //OPERAZIONI TRA SET
    LITERAL,        //SET
    VARIABLE,       //VARIABILI
    INSTRUCTION,    //ISTRUZIONI
    PROGRAM,        //INSIEME DI DICHIARAZIONI ED ESPRESSIONI
    GROUP           //PARENTESI (TODO: IMPLEMENTARE)
} expression_type;

/* TIPI DI ESPRESSIONI */

typedef set literal_expression; 
typedef char* variable_expression;
typedef expression_list program_expression;

typedef struct instruction {
    char* state;
    char* read;
    char* write;
    char* state2;
    enum TOKEN_TYPE move;
    struct expression* quantifier;
} instruction_expression;

typedef struct {
    struct expression* left;
    struct expression* right;
    token operator;
} binary_expression;

typedef struct {
    struct expression* group;
    size_t group_size;
} group_expression;

typedef struct expression {
    expression_type type;
    union {
        instruction_expression instruction;
        binary_expression binary;
        literal_expression literal;
        variable_expression variable;
        program_expression program;
        group_expression group;
    };
} expression;

void print_expression(const expression* exp);
char* expression_to_string(const expression* exp,int indent);
expression* expression_binary_simplify(expression* exp);
expression* expression_compress(expression* exp);
expression* expression_define(expression* exp);

#endif