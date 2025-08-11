#include "../headers/list.h"
#include "../headers/lexer.h"
#include "../headers/parser/parser.h"
#include "../headers/file_io.h"
#include "../headers/engine/turing_engine.h"
#include "../headers/engine/environment.h"
#include <stddef.h>
#include <stdlib.h>

int main(int argv, char** argc){

    file_context* f_context = 0;
    token_list* tokens = 0;
    lexing_info* info = 0;

    context_file_open(&f_context,"Program.tm");
    create_lexing_info(&info);

    char str[1024];
    while(read_next_chunk(f_context,str, 1024)){
        tokenize(str, 1024,&tokens,info);
    }
    write_end(tokens,info);
    context_file_close(f_context);

    expression* exp = parse(tokens);

    context* ctx = create_context();
    interpret_program(ctx, exp);

    tm_state* tm_machine;

    //Input temporaneo: TODO: cli arguments
    string_list* l;
    string_list_create(&l);
    
    for (int i = 0; argc[1][i] != '\0'; i++) {
        char* str = calloc(2,sizeof(char));
        str[0] = argc[1][i];
        string_list_add(l, str);    
    }

    init_engine(&tm_machine,l);
    destroy_string_list(l);

    printf("%s",tape_to_string(tm_machine->tape));
    printf("State: %s\n\n",tm_machine->state);

    execute(ctx, tm_machine);
    destroy_engine_context(tm_machine,ctx);
}