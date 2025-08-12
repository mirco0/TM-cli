#include "../headers/parser/parser.h"
#include "../headers/token.h"
#include "../headers/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

//TODO: GESTIRE MEGLIO ERRORI
#define PEEK_TYPE (peek(tokens,*start)? peek(tokens,*start)->type: -1)
#define MATCH(dest,...) \
    match( tokens,start,dest, sizeof((enum TOKEN_TYPE[]){__VA_ARGS__})/sizeof(enum TOKEN_TYPE), __VA_ARGS__)

/*
program           → statement
statement         → (assignment | instruction)*

instruction       → "<" ID (","ID){3} "," ("Left" | "Stop" | "Right") ">" (domain)?
assignment        → (IDENTIFIER | \Sigma) "=" set_op
domain            → "\forall" variables "\in" set_op

variables         → IDENTIFIER ("," IDENTIFIER)*  
set_op            → set_difference
set_difference    → set_union ( - set_union)* 
set_union         → set_intersection ( "\cup" set_intersection)*
set_intersection  → set_elements ( "\cap" set_elements )*
set_elements      → ( "\{" (IDENTIFIER | "\square") ( "," (IDENTIFIER | "\square") )* "\}" ) | SET
SET               → \Sigma | IDENTIFIER
*/

int match(token_list* tokens, size_t* start, token* dest_token, int count, ...){
    if(tokens->index < *start) return 0;
    va_list params;
    va_start(params,count);
    int peek_type = PEEK_TYPE;
    for(int i=0; i<count; i++){
        enum TOKEN_TYPE type = va_arg(params, enum TOKEN_TYPE);
        if( peek_type == type){
            *dest_token = *consume(tokens, start);
            return 1;
        }
    }
    *dest_token = *consume(tokens, start);
    return 0;
}

token* peek(token_list* tokens, size_t start) {
    if(tokens->index <= start){
        return 0;
    }
    return &tokens->data[start];
}

token* consume(token_list* tokens, size_t* start) {
    return &tokens->data[(*start)++];
}

int operate(token* tokens, int size);

expression* parse(token_list* tokens){
    if(tokens == NULL) return 0;
    size_t start = 0;
    expression* exp = parse_statement(tokens,&start);
    return exp;  

}

expression* parse_statement(token_list* tokens,size_t *start){
    expression* exp = expression_program_create();
    
    while(PEEK_TYPE == IDENTIFIER || PEEK_TYPE == SIGMA){
        expression_list_add(exp->program,parse_assignment(tokens,start));
    }
    
    while(PEEK_TYPE == INSTRUCTION_START){
        expression_list_add(exp->program,parse_instruction(tokens,start));
    }

    if(PEEK_TYPE != IDENTIFIER && PEEK_TYPE != SIGMA && PEEK_TYPE != INSTRUCTION_START && PEEK_TYPE != -1 && PEEK_TYPE != END_OF_FILE){
        printf("Unexpected token: '%s'.\n",token_to_string(peek(tokens, *start)));
        destroy_expression(exp);
        return 0;
    }
    return exp;
}

expression* parse_assignment(token_list* tokens,size_t* start){
    expression* sexp = expression_variable_create(NULL);
    
    token tk;
    if(!MATCH(&tk,IDENTIFIER,SIGMA)){
        print_parser_error("IDENTIFIER or '\\Sigma'",&tk);
        destroy_expression(sexp);
        return 0;
    }
    if(tk.data_size != 0){
        sexp->variable = tk.data;
    }else{
        sexp->variable = "\\Sigma";
    }
    
    if(!MATCH(&tk,ASSIGN)){
        print_parser_error("'='",&tk);
        destroy_expression(sexp);
        return 0;
    }

    expression* right = parse_set_op(tokens,start);
    expression* exp = expression_binary_create(tk, sexp, right);

    return exp;
}

expression* parse_instruction(token_list* tokens,size_t* start){
    token tk;
    if(!MATCH(&tk,INSTRUCTION_START)){
        print_parser_error("'<'",&tk);
        return 0;
    }

    if(!MATCH(&tk,IDENTIFIER)){
        print_parser_error("IDENTIFIER",&tk);
        return 0;
    }
    char* state = tk.data;

    if(!MATCH(&tk,COMMA)){
        print_parser_error("','",&tk);
        return 0;
    }
    
    if(!MATCH(&tk,IDENTIFIER,BLANK)){
        print_parser_error("IDENTIFIER or BLANK",&tk);
        return 0;
    }
    char* read = tk.data;

    if(!MATCH(&tk,COMMA)){
        print_parser_error("','",&tk);
        return 0;
    }
    
    if(!MATCH(&tk,IDENTIFIER,BLANK)){
        print_parser_error("IDENTIFIER or BLANK",&tk);
        return 0;
    }
    char* write = tk.data;

    if(!MATCH(&tk,COMMA)){
        print_parser_error("','",&tk);
        return 0;
    }
    
    if(!MATCH(&tk,IDENTIFIER)){
        print_parser_error("IDENTIFIER",&tk);
        return 0;
    }
    char* state2 = tk.data;


    if(!MATCH(&tk,COMMA)){
        print_parser_error("','",&tk);
        return 0;
    }
    
    if(!MATCH(&tk,ACTION_LEFT,ACTION_STOP,ACTION_RIGHT)){
        print_parser_error("'Left', 'Stop' or 'Right'",&tk);
        return 0;
    }
    enum TOKEN_TYPE move = tk.type;

    if(!MATCH(&tk,INSTRUCTION_END)){
        print_parser_error("'>'",&tk);
        return 0;
    }

    expression* exp = expression_instruction_create(state, read, write, state2, move, NULL);
    
    if (PEEK_TYPE == FORALL) {
        exp->instruction.quantifier = parse_domain(tokens, start);
    }

    return exp;
}

expression* parse_set_op(token_list* tokens, size_t* start) {
    return parse_set_difference(tokens, start);
}

expression* parse_set_difference(token_list* tokens, size_t* start){
    expression* exp = parse_set_union(tokens,start);
    
    token tk;
    while(PEEK_TYPE == SET_DIFFERENCE){
        tk = *consume(tokens,start);
        
        expression* exp2 = exp;
        expression* right = parse_set_union(tokens,start);
        exp = expression_binary_create(tk,exp2, right);

    }
    return exp;

}

expression* parse_set_union(token_list* tokens, size_t* start){
    expression* exp = parse_set_intersection(tokens,start);
    
    token tk;
    while(PEEK_TYPE == SET_UNION){
        tk = *consume(tokens,start);

        expression* exp2 = exp;
        expression* right = parse_set_intersection(tokens,start);
        exp = expression_binary_create(tk,exp2, right);

    }
    return exp;

}

expression* parse_set_intersection(token_list* tokens, size_t* start){
    expression* exp = parse_set_elements(tokens,start);
    
    token tk;
    while(PEEK_TYPE == SET_INTERSECTION){
        tk = *consume(tokens,start);
        
        expression* exp2 = exp;
        expression* right = parse_set_elements(tokens,start);
        exp = expression_binary_create(tk,exp2, right);

    }
    return exp;

}

expression* parse_set_elements(token_list* tokens, size_t* start){

    token tk = *consume(tokens,start);

    if(tk.type == IDENTIFIER){
        expression* exp = expression_variable_create(tk.data);
        return exp;
    }

    if(tk.type == SIGMA){
        expression* exp = expression_variable_create("\\Sigma");
        return exp;
    }

    expression* exp = expression_literal_create();
    if(tk.type != SET_START){
        print_parser_error("'\\{'",&tk);
        destroy_expression(exp);
        return 0;
    }

    tk = *consume(tokens,start);

    if(tk.type != IDENTIFIER && tk.type != BLANK){
        print_parser_error("IDENTIFIER or BLANK after '\\{'",&tk);
        destroy_expression(exp);
        return 0;
    }
    
    set_insert(exp->literal,tk.data);
    
    while(MATCH(&tk,COMMA)) {
        if(MATCH(&tk,IDENTIFIER,BLANK)){
            set_insert(exp->literal,tk.data);
        }else{
            print_parser_error("IDENTIFIER or blank after ','", &tk);
            destroy_expression(exp);
            return 0;
        }
    }
    if(tk.type != SET_END){
        print_parser_error("'\\}'",&tk);
        destroy_expression(exp);
        return 0;
    }

    return exp;
}

expression* parse_domain(token_list* tokens, size_t* start){
    token tk;
    if(!MATCH(&tk,FORALL)){
        print_parser_error("\\forall",&tk);
        exit(0);
        return 0;
    }
    
    expression* exp = expression_binary_create(tk, parse_variables(tokens,start), NULL);

    if(!MATCH(&tk,IN)){
        print_parser_error("'\\in'",&tk);
        destroy_expression(exp);
        exit(0);
        return 0;
    }
    exp->binary.right = parse_set_op(tokens,start);;
    
    return exp;
}

expression* parse_variables(token_list* tokens, size_t* start){
    expression* exp = expression_literal_create();
    token tk;

    if(!MATCH(&tk,IDENTIFIER)) {
        print_parser_error("IDENTIFIER",&tk);
        exit(0);
        return 0;
    }
    
    set_insert(exp->literal, tk.data);
    
    while(PEEK_TYPE == COMMA) {
        tk = *consume(tokens,start);
        if(MATCH(&tk,IDENTIFIER)){
            set_insert(exp->literal,tk.data);
        }
    }
    return exp;
}


void print_parser_error(char* expected, const token* tk){
    char* file_name = "Program.tm";
    printf("%s%sError%s in file %s%s:%d:%d%s:\nExpected %s but got '%s%s%s'\n",
        ANSI_BOLD,
        ANSI_COLOR_RED,
        ANSI_COLOR_RESET,
        ANSI_BOLD,
        file_name,
        tk->line_num, 
        tk->char_num,
        ANSI_COLOR_RESET,
        expected,
        ANSI_COLOR_CYAN,
        tk->data_size? tk->data: TOKEN_TYPES_[tk->type],
        ANSI_COLOR_RESET);
    exit(1);
}