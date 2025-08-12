#include "../headers/engine/environment.h"
#include "../headers/engine/turing_engine.h"
#include "../headers/set.h"
#include "../headers/list.h"
#include "../headers/parser/expression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: AGGIUNGERE CALLBACK PER GESTIONE DI ERRORI RUNTIME

int init_engine(tm_state** machine, string_list* input){
    //TODO: aggiungere stringa di input
    if(machine == NULL){
        return 0;
    }
    *machine = calloc(sizeof(tm_state),1);
    if(*machine == NULL){
        return 0;
    }
    //TODO: 
    // Stato di partenza personalizzato
    // MAX_STEPS personalizzati
    (*machine)->state = "q0";
    (*machine)->tape = create_tape(input);
    (*machine)->max_steps = 1300;
    (*machine)->steps = 0;
    return 1;
}

void execute(const context* context, tm_state* machine){

    instruction_expression* instr = context_get_instruction(context, machine->state,machine->tape->content);
    if(instr == NULL) return;
    
    machine->tape->content = instr->write;
    move_tape(&machine->tape,instr->move);
    machine->state = instr->state2;
    machine->steps++;
    
    printf("%s",tape_to_string(machine->tape));
    printf("State: %s\n\n",machine->state);

    set* final_states = context_get_variabile(context,"QF"); 
    if(set_contains(final_states,machine->state)){
        printf("Final state termination!.\n");
        return;
    }
    
    if(machine->steps < machine->max_steps && strcmp(machine->state,"qF")){
        execute(context,machine);
    }

}

void destroy_engine_context(tm_state* machine, context* context){
    if(machine == NULL) return;
    destroy_tape(machine->tape);
    destroy_context(context);
}

set* eval_binary(const context* context, const expression* exp){
    set* left = evaluate(context,exp->binary.left);
    set* right = evaluate(context,exp->binary.right);
    
    switch (exp->binary.operator.type) {

        case SET_INTERSECTION:
            return set_intersection(left,right);
        
        case SET_UNION:
            return set_union(left,right);

        case SET_DIFFERENCE:
            return set_difference(left,right);
        
        default: return NULL;
    }
}

set* evaluate(const context* context, const expression* exp){
    switch (exp->type) {
        case VARIABLE:
            return ht_get(context->table,exp->variable);
        case LITERAL:
            return  set_copy(exp->literal);
        case BINARY:
            return eval_binary(context,exp);
        default: return NULL;

    }
}

void interpret_program(context* context, const expression* program){
    
    if (!program || program->type != PROGRAM) return;

    for (size_t i = 0; i < program->program->index; ++i) {
        expression* exp = program->program->data[i];

        if (exp->type == BINARY && exp->binary.operator.type == ASSIGN) {
            
            char* name = exp->binary.left->variable;
            set* value = evaluate(context, exp->binary.right);
            
            context_define_variable(context, name, value);
        }

        if (exp->type == INSTRUCTION) {
            evaluate_instructions(context, exp);
        }
    }
}

void evaluate_instructions(const context* context, const expression* exp){
    instruction_expression ex = exp->instruction;
    if(ex.quantifier){
        //TODO: FARE FREE
        string_list* variables = set_to_list(ex.quantifier->binary.left->literal);
        
        char** variables_value = malloc(sizeof(char*)*variables->index);

        expression* iter_expression = malloc(sizeof(expression));
        iter_expression->type = INSTRUCTION;
        int it = 0;
        set *s = evaluate(context,exp->instruction.quantifier->binary.right);
        while(next_combination(variables_value,set_to_list(s),variables->index,it)){
            it++;
            // No copy ne creazione per evitare continue allocazioni
            iter_expression->instruction.read = ex.read;
            iter_expression->instruction.write = ex.write;
            iter_expression->instruction.state = ex.state;
            iter_expression->instruction.state2 = ex.state2;
            iter_expression->instruction.move = ex.move;
            //Per ogni variabile determina il campo da sostituire
            for(int i = 0; i<variables->index; i++){
                if(strcmp(variables->data[i],ex.state) == 0){
                    iter_expression->instruction.state = variables_value[i];
                }
                if(strcmp(variables->data[i],ex.read) == 0){
                    iter_expression->instruction.read = variables_value[i];
                }
                if(strcmp(variables->data[i],ex.write) == 0){
                    iter_expression->instruction.write = variables_value[i];
                }
                if(strcmp(variables->data[i],ex.state2) == 0){
                    iter_expression->instruction.state2 = variables_value[i];
                }
            }
            context_define_instruction(context, iter_expression);
        };
    }else{
        context_define_instruction(context, exp);
    }
}

int next_combination(char** dest, string_list* set, int num_variables, int iteration){
    size_t total_combinations = 1;


    for (size_t i = 0; i < num_variables; i++) {
        total_combinations *= set->index;
    }

    if (iteration >= total_combinations) {
        return 0;
    }

    for (size_t i = 0; i < num_variables; i++) {
        size_t choices = set->index;
        size_t choice_index = iteration % choices;
        iteration /= choices;
        dest[num_variables-i-1] = set->data[choice_index];
    }
    return 1;
}