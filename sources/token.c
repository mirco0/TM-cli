#include "../headers/token.h"
#include <stdio.h>
#include <stdlib.h>


//TODO: REMOVE
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_BOLD          "\e[1;37m"
#define ANSI_COLOR_RESET   "\x1b[0m"


const char* TOKEN_TYPES_[] = {

    "INSTRUCTION_START",
    "INSTRUCTION_END",
    "COMMA",
    "SET_START",
    "SET_END",
    "ASSIGN",

    "OPEN_PARENTESIS",
    "CLOSE_PARENTESIS",

    "IDENTIFIER",
    "STRING",

    "SIGMA",
    "FORALL",
    "IN",
    "SET_UNION",
    "SET_DIFFERENCE",
    "SET_INTERSECTION",
    "BLANK"
};

char* token_to_string(const token* token_item){
    char* str;
    if(token_item == NULL){
        asprintf(&str,"NULL TOKEN.");    
    }
    asprintf(&str,"(%s,%s,%zd,%d,%d)",TOKEN_TYPES_[token_item->type], token_item->data, token_item->data_size,token_item->line_num,token_item->char_num);
    return str;
}

char* error_token_to_string(const token* token_item){
    char* str;
    if(token_item == NULL){
        asprintf(&str,"NULL TOKEN.");    
    }
    asprintf(&str,"%sError%s:%d:%d '%s'",
        ANSI_BOLD,
        ANSI_COLOR_RESET,
        token_item->line_num +1,
        token_item->char_num,
        token_item->data_size ? token_item->data : TOKEN_TYPES_[token_item->type]
    );
    return str;
}