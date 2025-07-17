#include "../headers/list.h"
#include "../headers/lexer.h"
#include "../headers/parser/parser.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    // TODO: FIX 
    FILE *fptr;
    fptr = fopen("Program.tm", "r");
    char str[1024];
    size_t len;

    token_list *tokens = 0;
    lexing_info* info = 0;
    create_lexing_info(&info);
    

    //TODO FIX
    while ((len = fread(str, 1, sizeof(str) - 1, fptr)) > 0) {
        str[len] = '\0';

        int last = (int)len - 1;
        while (last >= 0 && str[last] != ' ' && str[last] != '\n') {
            last--;
        }

        if (last >= 0 && last < (int)len - 1) {
            fseek(fptr, last - len + 1, SEEK_CUR);
            str[last + 1] = '\0';
        } else if (last == -1) {
            str[len] = '\0';
        }

        tokenize(str, 1024,&tokens,info);   
    }
    write_end(tokens,info);
    fclose(fptr);
    print_token_list(tokens);
    expression* exp = parse(tokens);
    printf("%s\n",expression_to_string(exp,1));
}