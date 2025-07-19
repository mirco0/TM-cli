#include "../headers/list.h"
#include "../headers/lexer.h"
#include "../headers/parser/parser.h"
#include "../headers/file_io.h"
#include <stdio.h>

int main(){

    file_context* context = 0;
    token_list* tokens = 0;
    lexing_info* info = 0;
    context_file_open(&context,"Program.tm");
    create_lexing_info(&info);


    char str[1024];
    while(read_next_chunk(context,str, 1024)){
        tokenize(str, 1024,&tokens,info);
    }
    write_end(tokens,info);
    
    expression* exp = parse(tokens);
    exp = expression_compress(exp);

    printf("%s\n",expression_to_string(exp,1));

}