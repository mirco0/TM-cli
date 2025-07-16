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

instruction       → "<" ID (","ID){4} ">" (domain)?
assignment        → (IDENTIFIER | \Sigma) "=" set_op
domain            → "\forall" variables "\in" set_op

variables         → IDENTIFIER ("," IDENTIFIER)*  
set_op            → set_difference
set_difference    → set_union ( - set_union)* 
set_union         → set_intersection ( "\cup" set_intersection)*
set_intersection  → set_elements ( "\cap" set_elements )*
set_elements      → ( "\{" (IDENTIFIER | "\square") ( "," (IDENTIFIER | "\square") )* "\}" ) | SET
SET     → \Sigma | IDENTIFIER
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
    expression* exp = calloc(1, sizeof(expression));
    exp->type = PROGRAM;
    expression_list* exp_list = 0;
    expression_list_create(&exp_list);
    
    while(PEEK_TYPE == IDENTIFIER || PEEK_TYPE == SIGMA){
        expression_list_add(exp_list,parse_assignment(tokens,start));
    }
    
    while(PEEK_TYPE == INSTRUCTION_START){
        expression_list_add(exp_list,parse_instruction(tokens,start));
    }

    if(PEEK_TYPE != IDENTIFIER && PEEK_TYPE != SIGMA && PEEK_TYPE != INSTRUCTION_START && PEEK_TYPE != -1 && PEEK_TYPE != END_OF_FILE){
        printf("Unexpected token: '%s'.\n",token_to_string(peek(tokens, *start)));
        return 0;
    }
    exp->program = *exp_list;
    return exp;
}

expression* parse_assignment(token_list* tokens,size_t* start){

    expression* exp = calloc(1, sizeof(expression));
    expression* sexp = calloc(1,sizeof(expression));

    token tk;
    if(!MATCH(&tk,IDENTIFIER,SIGMA)){
        print_parser_error("IDENTIFIER or '\\Sigma'",&tk);
        return 0;
    }
    sexp->type = VARIABLE;
    if(tk.data_size != 0){
        sexp->variable = tk.data;
    }else{
        sexp->variable = "SIGMA";
    }

    exp->type = BINARY;
    exp->binary.left = sexp;
    
    if(!MATCH(&tk,ASSIGN)){
        print_parser_error("'='",&tk);
        return 0;
    }

    exp->binary.operator = tk;
    exp->binary.right = parse_set_op(tokens,start);
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
    
    if(!MATCH(&tk,IDENTIFIER)){
        print_parser_error("INDENTIFIER",&tk);
        return 0;
    }
    char* move = tk.data;

    if(!MATCH(&tk,INSTRUCTION_END)){
        print_parser_error("'>'",&tk);
        return 0;
    }


    expression* exp = calloc(1,sizeof(expression));
    exp->type = INSTRUCTION;
    exp->instruction.state  = state;
    exp->instruction.read   = read;
    exp->instruction.write  = write;
    exp->instruction.state2 = state2;
    exp->instruction.move   = move;
    exp->instruction.quantifier = 0;

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
        
        exp = calloc(1,sizeof(expression));

        exp->type = BINARY;
        exp->binary.operator = tk;
        exp->binary.left = exp2;
        exp->binary.right = right;
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
        
        exp = calloc(1,sizeof(expression));

        exp->type = BINARY;
        exp->binary.operator = tk;
        exp->binary.left = exp2;
        exp->binary.right = right;
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
        
        exp = calloc(1,sizeof(expression));

        exp->type = BINARY;
        exp->binary.operator = tk;
        exp->binary.left = exp2;
        exp->binary.right = right;
    }
    return exp;

}

expression* parse_set_elements(token_list* tokens, size_t* start){

    token tk = *consume(tokens,start);
    expression* exp = calloc(1,sizeof(expression));

    if(tk.type == IDENTIFIER){
        exp->type = VARIABLE;
        exp->variable = tk.data;
        return exp;
    }

    
    set* s = create_set();
    if(tk.type == SIGMA){
        set_insert(s,"S", 2);
        set_insert(s,"I", 2);
        set_insert(s,"G", 2);
        set_insert(s,"M", 2);
        set_insert(s,"A", 2);
        exp->type = LITERAL;
        exp->literal = *s;
        return exp;
    }

    if(tk.type != SET_START){
        print_parser_error("'\\{'",&tk);
        return 0;
    }
    tk = *consume(tokens,start);

    if(tk.type != IDENTIFIER && tk.type != BLANK){
        print_parser_error("IDENTIFIER or BLANK after '\\{'",&tk);
        return 0;
    }
    
    set_insert(s,tk.data, tk.data_size);
    
    while(MATCH(&tk,COMMA)) {
        if(MATCH(&tk,IDENTIFIER,BLANK)){
            set_insert(s,tk.data, tk.data_size);
        }else{
            //Messaggio precedente "No IDENTIFIER or blank after ',', got %s"
            print_parser_error("IDENTIFIER or blank after ','", &tk);
            return 0;
        }
    }
    if(tk.type != SET_END){

        print_parser_error("'\\}'",&tk);
        return 0;
    }

    exp->type = LITERAL;
    exp->literal = *s;
    return exp;
}

expression* parse_domain(token_list* tokens, size_t* start){
    token tk;
    if(!MATCH(&tk,FORALL)){
        print_parser_error("\\forall",&tk);
        exit(0);
        return 0;
    }
    
    expression* exp = calloc(1, sizeof(expression));
    exp->binary.operator = tk;
    exp->type = BINARY;

    exp->binary.left = parse_variables(tokens,start);

    if(!MATCH(&tk,IN)){
        print_parser_error("'\\in'",&tk);
        exit(0);
        return 0;
    }
    exp->binary.right = parse_set_op(tokens,start);;
    
    return exp;
}

expression* parse_variables(token_list* tokens, size_t* start){
    expression* exp = calloc(1,sizeof(expression));
    set* s = create_set();
    token tk;

    if(!MATCH(&tk,IDENTIFIER)) {
        print_parser_error("IDENTIFIER",&tk);
        exit(0);
        return 0;
    }
    
    set_insert(s, tk.data,tk.data_size);
    
    while(PEEK_TYPE == COMMA) {
        tk = *consume(tokens,start);
        if(MATCH(&tk,IDENTIFIER)){
            set_insert(s,tk.data, tk.data_size);
        }
    }
    exp->type = LITERAL;
    exp->literal = *s;
    return exp;
}


void print_parser_error(char* expected, const token* tk){
    char* file_name = "Program.tm";
    printf("%s%s:%d:%d %s Expected %s but got %s.\n",ANSI_BOLD,file_name,tk->line_num,tk->char_num,ANSI_COLOR_RESET,expected,tk->data_size? tk->data: TOKEN_TYPES_[tk->type]);
    exit(1);
}