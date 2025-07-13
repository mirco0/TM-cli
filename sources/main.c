#include "../headers/list.h"
#include "../headers/lexer.h"
#include "../headers/parser/parser.h"
#include <stdio.h>

int main(){
    // TODO: FIX 
    FILE *fptr;
    fptr = fopen("Program.tm", "r");
    char str[1024];
    
    token_list *tokens = 0;
    while(fgets(str, 1024, fptr)){
        if(str[0] == '#') continue;
        tokenize(str, 1024,&tokens);
    }
    
    print_token_list(tokens);
    expression* exp = parse(tokens);
    printf("%s\n",expression_to_string(exp,1));
}