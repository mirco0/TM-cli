#include "../headers/parser/expression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//TODO: REMOVE
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_BOLD          "\e[1;37m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void print_expression(const expression* exp){

}

void append_str(char** dest, const char* fmt, ...){
    va_list args;
    va_start(args, fmt);

    char* temp;
    vasprintf(&temp, fmt, args);
    va_end(args);

    if (*dest == NULL) {
        *dest = temp;
    } else {
        char* new_str;
        
        asprintf(&new_str, "%s%s", *dest, temp);
        free(*dest);
        free(temp);
        *dest = new_str;
    }

}

//TODO: RISCRIVERE
char* expression_to_string(const expression* exp, int indent){
    
    char* str = malloc(sizeof(char)*1000);
    if(exp == NULL){
        asprintf(&str,"Empty expression.");
        return str;
    }
    
    /*
    switch (exp->type) {
        case INSTRUCTION:
            append_str(&str, "%sInstruction%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
            append_str(&str, "%s<%s,%s,%s,%s,%s>\n",
                exp->instruction.state,
                exp->instruction.read,
                exp->instruction.write,
                exp->instruction.state2,
                exp->instruction.move);
            return str;
        case BINARY:
        case LITERAL:
        case VARIABLE:
        case PROGRAM:
        default: break;
    }

    return str;
*/
    if(exp->type == INSTRUCTION){
        
        asprintf(&str,"%sInstruction%s \n%.*sData: <%s,%s,%s,%s,%s>\n%.*sQuantifier: (%s).",
            ANSI_COLOR_RED,
            ANSI_COLOR_RESET,
            indent,
            "\t\t\t\t\t\t\t\t",
            exp->instruction.state,
            exp->instruction.read,
            exp->instruction.write,
            exp->instruction.state2,
            exp->instruction.move,
            indent,
            "\t\t\t\t\t\t\t\t",
            expression_to_string(exp->instruction.quantifier,indent+1)
        ); 
        return str;
    }

    if(exp->type == BINARY){
        char* left_str = expression_to_string(exp->binary.left,indent+1);
        char* oper_str = token_to_string(&exp->binary.operator);
        char* right_str = expression_to_string(exp->binary.right,indent+1);
        
        //TODO:REWRITE
        asprintf(&str,"%sBinary%s %s\n%.*sA: (%s),\n%.*sB: (%s).\n%.*s   ",
            ANSI_COLOR_YELLOW,
            ANSI_COLOR_RESET,
            oper_str,
            indent,
            "\t\t\t\t\t\t\t\t",
            left_str,
            indent,
            "\t\t\t\t\t\t\t\t",
            right_str,
            indent-1,
            "\t\t\t\t\t\t\t\t"
        ); 
        return str;
    }

    if(exp->type == LITERAL){
        asprintf(&str,"%sLiteral%s %s",ANSI_COLOR_MAGENTA,ANSI_COLOR_RESET,set_to_string(&exp->literal)); 
        return str;
    }
    
    if(exp->type == VARIABLE){
        asprintf(&str,"%sVariable%s %s",ANSI_COLOR_GREEN,ANSI_COLOR_RESET,exp->variable);
        
        return str;
    
    }


    // TODO: SISTEMARE
    if(exp->type == PROGRAM){
        asprintf(&str,"%sProgram:\n%s",ANSI_COLOR_CYAN,ANSI_COLOR_RESET);
        for(size_t i = 0; i<exp->program.index; i++){
            asprintf(&str,"%s%s\n",str,expression_to_string((exp->program.data[i]),indent));
        }
        return str;
    }


    if(exp->type == GROUP){
        printf("MALE2\n");
    }

    asprintf(&str,"Unknown expression.");
    return str;

}