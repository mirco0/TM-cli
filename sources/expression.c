#include "../headers/parser/expression.h"
#include "../headers/utils.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void print_expression(const expression* exp){

}

//TODO: RISCRIVERE
char* expression_to_string(expression* exp, int indent){
    char* str;
    if(exp == NULL){
        asprintf(&str,"Empty expression.");
        return str;
    }
    
    char* inner_indent_str = repeat('\t',indent);
    char* parent_indent_str = repeat('\t',indent-1);
    
    switch (exp->type) {
        case INSTRUCTION:
            asprintf(&str,"%sInstruction%s \n%sData: <%s,%s,%s,%s,%s>\n%sQuantifier: (%s).",
                ANSI_COLOR_RED,
                ANSI_COLOR_RESET,
                inner_indent_str,
                exp->instruction.state,
                exp->instruction.read,
                exp->instruction.write,
                exp->instruction.state2,
                TOKEN_TYPES_[exp->instruction.move],
                inner_indent_str,
                expression_to_string(exp->instruction.quantifier,indent+1)
            );
            free(inner_indent_str);
            free(parent_indent_str);
            return str;

        case BINARY:
            {
                char* left_str = expression_to_string(exp->binary.left,indent+1);
                char* oper_str = token_to_string(&exp->binary.operator);
                char* right_str = expression_to_string(exp->binary.right,indent+1);
                
                asprintf(&str,"%sBinary%s %s\n%sA: (%s),\n%sB: (%s).\n%s",
                    ANSI_COLOR_YELLOW,
                    ANSI_COLOR_RESET,
                    oper_str,
                    inner_indent_str,
                    left_str,
                    inner_indent_str,
                    right_str,
                    parent_indent_str
                );
                free(inner_indent_str);
                free(parent_indent_str);
                free(left_str);
                free(oper_str);
                free(right_str);
                return str;
            }
        case LITERAL:
            {
                char* set_str = set_to_string(exp->literal); 
                asprintf(&str,"%sLiteral%s %s",
                    ANSI_COLOR_MAGENTA,
                    ANSI_COLOR_RESET,
                    set_str
                );
                free(inner_indent_str);
                free(parent_indent_str);
                free(set_str);
                return str;
            }            
        case VARIABLE:
            asprintf(&str,"%sVariable%s %s",ANSI_COLOR_GREEN,ANSI_COLOR_RESET,exp->variable);
            free(inner_indent_str);
            free(parent_indent_str);
            return str;
    
        case PROGRAM:

            asprintf(&str,"%sProgram:\n%s",ANSI_COLOR_CYAN,ANSI_COLOR_RESET);
            for(size_t i = 0; i<exp->program->index; i++){
                asprintf(&str,"%s%s\n",str,expression_to_string((exp->program->data[i]),indent));
            }
            free(inner_indent_str);
            free(parent_indent_str);
            return str;

        default: 
            asprintf(&str,"Unknown expression.");
            free(inner_indent_str);
            free(parent_indent_str);
            return str;
        break;
    }

    return str;

}

void destroy_expression(expression* expression){
    // Approccio ricorsivo

    if(expression == NULL) return;

    switch (expression->type) {
        case BINARY:
            destroy_expression(expression->binary.left);
            destroy_expression(expression->binary.right);
            break;
        
        case LITERAL:
            ht_destroy(expression->literal);
            break;

        // char* che non dovrebbero essere allocati dinamicamente
        case VARIABLE:
            break;

        case INSTRUCTION:
            // char* state; char* read; char* write; char* state2 non dovrebbero essere allocati dinamicamente
            destroy_expression(expression->instruction.quantifier);
            break;

        case PROGRAM:
            destroy_expression_list(expression->program);
            break;
        
        //Non ancora implementato
        case GROUP:
            break;
        
        default:        
            printf("Unknown state!\n");
            return;
    }
    free(expression);
}

expression* expression_binary_create(token operator, expression* left, expression* right){
    expression* exp = calloc(1,sizeof(expression));
    if(exp == NULL) return NULL;
    exp->type = BINARY;
    exp->binary.operator = operator;
    exp->binary.left = left;
    exp->binary.right = right;
    return exp;
}

expression* expression_literal_create(){
    expression* exp = calloc(1,sizeof(expression));
    if(exp == NULL) return NULL;
    exp->type = LITERAL;
    exp->literal = create_set();
    return exp;
}

expression* expression_variable_create(variable_expression variable){
    expression* exp = calloc(1,sizeof(expression));
    if(exp == NULL) return NULL;
    exp->type = VARIABLE;
    exp->variable = variable;
    return exp;
}

expression* expression_instruction_create(char* state, char* read, char* write, char* state2, enum TOKEN_TYPE move, expression* quantifier){
    expression* exp = calloc(1,sizeof(expression));
    if(exp == NULL) return NULL;
    exp->type = INSTRUCTION;
    exp->instruction.state  = state;
    exp->instruction.read   = read;
    exp->instruction.write  = write;
    exp->instruction.state2 = state2;
    exp->instruction.move   = move;
    exp->instruction.quantifier = quantifier;
    return exp;
}

expression* expression_program_create(){
    expression* exp = calloc(1,sizeof(expression));
    if(exp == NULL) return NULL;
    exp->type = PROGRAM;
    expression_list_create(&exp->program);
    return exp;
}

expression* expression_group_create();

//Copia ricorsiva e totale
expression* expression_copy(expression* expression){
    if(expression == NULL) return NULL;
    switch (expression->type) {
        case BINARY:
            {
                struct expression* left = expression_copy(expression->binary.left);
                struct expression* right = expression_copy(expression->binary.right);
                return expression_binary_create(expression->binary.operator, left, right);
            }

        case LITERAL:
            {
                struct expression* exp = expression_literal_create();
                free(exp->literal);
                exp->literal = set_copy(expression->literal);
                return exp;
            }

        case VARIABLE:
            return expression_variable_create(expression->variable);

        case INSTRUCTION:
            {
                struct expression* quantifier = expression_copy(expression->instruction.quantifier);
                return expression_instruction_create(expression->instruction.state, expression->instruction.read, expression->instruction.write, expression->instruction.state2, expression->instruction.move,quantifier);
            }

        case PROGRAM:
            {
                struct expression* exp = expression_program_create();
                for(size_t i = 0; i<expression->program->index; i++){
                    struct expression* item_copy = expression_copy(expression->program->data[i]);
                    expression_list_add(exp->program,item_copy);
                }
                return exp;
            }
        
        case GROUP:
            return NULL;
        
        default:
            return NULL;
    }
    return NULL;
}


//TODO: Valutare se rimuovere in favore di eval (turing_engine.h)
//Operazioni di semplificazioni (ottimizzazioni) dell'albero nella fase di parsing 
expression* expression_binary_simplify(expression* exp){
    if(exp == NULL) return exp;
    if(exp->binary.left->type == exp->binary.right->type && exp->binary.left->type != LITERAL)
        return exp;

    expression *new_exp = calloc(1, sizeof(expression));
    new_exp->type = LITERAL;
    set* result;

    switch (exp->binary.operator.type) {
        case SET_INTERSECTION:
            result = set_merge_intersection(exp->binary.left->literal,exp->binary.right->literal);
            new_exp->literal = result;
            free(exp);
            return new_exp;
            break;
        
        case SET_UNION:
            result = set_merge_union(exp->binary.left->literal,exp->binary.right->literal);
            new_exp->literal = result;
            free(exp);
            return new_exp;

        case SET_DIFFERENCE:
            result = set_merge_difference(exp->binary.left->literal,exp->binary.right->literal);
            new_exp->literal = result;
            free(exp);
            return new_exp;
        
        default: return exp;
    }
    return 0;
}

expression* expression_compress(expression* exp){

    if(exp == NULL) return exp;
    switch (exp->type) {
        case PROGRAM:
            for(int i = 0; i<exp->program->index; i++){
                exp->program->data[i] = expression_compress(exp->program->data[i]);
            }
            return exp;
            break;
        case BINARY:
            exp->binary.left = expression_compress(exp->binary.left);
            exp->binary.right = expression_compress(exp->binary.right);
            exp = expression_binary_simplify(exp);
            return exp;
            
        case INSTRUCTION:
            exp->instruction.quantifier = expression_compress(exp->instruction.quantifier);
        default: return exp;
    }
    return exp;
}