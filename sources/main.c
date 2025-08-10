#include "../headers/list.h"
#include "../headers/lexer.h"
#include "../headers/parser/parser.h"
#include "../headers/file_io.h"
#include "../headers/engine/turing_engine.h"
#include "../headers/engine/environment.h"
#include <stddef.h>
#include <stdlib.h>

int main(){

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

    expression* exp = parse(tokens);

    context* ctx = create_context();
    interpret_program(ctx, exp);

    tm_state* tm_machine;
    init_engine(&tm_machine,NULL);
    printf("%sState: q0\n",tape_to_string(tm_machine->tape));
    execute(ctx, tm_machine);

}