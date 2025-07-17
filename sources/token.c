#include "../headers/token.h"
#include <stdio.h>
#include <stdlib.h>
#include "../headers/utils.h"

const char* TOKEN_TYPES_[] = {
    "EOF",
    "INSTRUCTION_START",
    "INSTRUCTION_END",
    "COMMA",
    "SET_START",
    "SET_END",
    "ASSIGN",

    "OPEN_PARENTESIS",
    "CLOSE_PARENTESIS",

    "ACTION_LEFT",
    "ACTION_STOP",
    "ACTION_RIGTH",

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