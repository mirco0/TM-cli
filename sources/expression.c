#include "../headers/parser/expression.h"
#include <stdio.h>
#include <stdlib.h>
#include "../headers/utils.h"

void print_expression(const expression* exp){

}

//TODO: RISCRIVERE
char* expression_to_string(const expression* exp, int indent){
    
    char* str = malloc(sizeof(char)*1000);
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
                exp->instruction.move,
                inner_indent_str,
                expression_to_string(exp->instruction.quantifier,indent+1)
            );
            free(inner_indent_str);
            return str;

        case BINARY:
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
            free(left_str);
            free(oper_str);
            free(right_str);
            return str;

        case LITERAL:
            char* set_str = set_to_string(&exp->literal); 
            asprintf(&str,"%sLiteral%s %s",
                ANSI_COLOR_MAGENTA,
                ANSI_COLOR_RESET,
                set_str
            );
            free(set_str);
            return str;
            
        case VARIABLE:
            asprintf(&str,"%sVariable%s %s",ANSI_COLOR_GREEN,ANSI_COLOR_RESET,exp->variable);
            return str;
    
        case PROGRAM:

            asprintf(&str,"%sProgram:\n%s",ANSI_COLOR_CYAN,ANSI_COLOR_RESET);
            for(size_t i = 0; i<exp->program.index; i++){
                asprintf(&str,"%s%s\n",str,expression_to_string((exp->program.data[i]),indent));
            }
            return str;

        default: 
            asprintf(&str,"Unknown expression.");
            return str;
        break;
    }

    return str;

}