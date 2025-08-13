#include "../headers/engine/environment.h"
#include "../headers/utils.h"
#include "../headers/parser/expression.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

context* create_context(){
    context* context_ = calloc(1,sizeof(context));
    context_->table = ht_create();
    
    // Imposta il valore prefefinito di Sigma {0,1} 
    // TODO: Aggiungere valore di Sigma personalizzato
    set* sigma = create_set();
    set_insert(sigma,"0");
    set_insert(sigma,"1");
    context_define_variable(context_, "\\Sigma",sigma);

    context_->instructions_table = ht_create();
    return context_;
}

int context_define_variable(context* context, char* name, void* value){
    return ht_set(context->table,name,value) != NULL;
}

void* context_get_variabile(const context* context, char* name){
    return ht_get(context->table,name);
}

int context_define_instruction(const context* context, const expression* exp){
    int key_len = strlen(exp->instruction.read)+strlen(exp->instruction.state)+2;
    char* key = malloc(sizeof(char)*key_len);
    snprintf(key,key_len,"%s %s",exp->instruction.read,exp->instruction.state);            

    instruction_expression* instr_copy = malloc(sizeof(instruction_expression));
    *instr_copy = exp->instruction;
    
    const char* res = ht_set(context->instructions_table, key, instr_copy);
    free(key);
    return res != NULL;
}

instruction_expression* context_get_instruction(const context* context, char* state, char* read){
    int len = strlen(read)+strlen(state)+2;
    char* read_state = malloc(sizeof(char)*len);
    snprintf(read_state,len,"%s %s",read,state);
    return ht_get(context->instructions_table,read_state);
}

void destroy_context(context *context){
    if (context == NULL) return; 
    ht_destroy(context->table);
    ht_destroy(context->instructions_table);
    free(context);
}

tape* create_empty_tape(){
    tape* t = calloc(sizeof(tape),1);
    t->content = "\t";
    return t;
}

tape* create_tape(const string_list* values){
    tape* t = create_empty_tape();
    if(values == NULL) return t;
    tape* head = t;
    if(values->index == 0) return head;
    t->content = strdup(values->data[0]);
    for(size_t i = 1; i<values->index; i++){
        move_tape(&t, ACTION_RIGHT);
        t->content = strdup(values->data[i]);
    }
    return head;
}

void move_tape(tape** t, enum TOKEN_TYPE move){
    switch (move) {
        case ACTION_LEFT:
            if((*t)->prev == NULL){
                (*t)->prev = create_empty_tape();
                (*t)->prev->next = (*t);
            }
            (*t) = (*t)->prev;
            break;

        case ACTION_RIGHT:
            if((*t)->next == NULL){
                (*t)->next = create_empty_tape();
                (*t)->next->prev = (*t);
            }
            (*t) = (*t)->next;
            break;

        default:
            return;
    }
}

char* tape_to_string(const tape* t) {
    if (!t) return NULL;
    const tape* start = t;

    while (t->prev) t = t->prev;

    size_t count = 0;
    const tape* tmp = t;
    size_t* widths = NULL;
    while (tmp) {
        widths = realloc(widths, sizeof(size_t) * (count + 1));
        widths[count] = strlen(tmp->content);
        count++;
        tmp = tmp->next;
    }

    size_t total_len = count * (4 + 2 + 4) * 3 + 128;
    char* out = malloc(total_len);
    char* p = out;

    append_str_raw(&p, "┌");
    for (size_t i = 0; i < count; i++) {
        for (size_t j = 0; j < widths[i] + 2; j++) append_str_raw(&p, "─");
        append_str_raw(&p, (i + 1 < count) ? "┬" : "┐");
    }

    append_str_raw(&p, "\n│");
    tmp = t;
    for (size_t i = 0; i < count; i++) {
        const char* content = (strcmp(tmp->content, "\t") == 0) ? "□" : tmp->content;

        append_str_raw(&p, " ");
        if (tmp == start) {
            append_str_raw(&p, ANSI_COLOR_YELLOW);
            memcpy(p, content, strlen(content));
            p += strlen(content);
            append_str_raw(&p, ANSI_COLOR_RESET);
        } else {
            memcpy(p, content, strlen(content));
            p += strlen(content);
        }
        append_str_raw(&p, " │");
        tmp = tmp->next;
    }
    append_str_raw(&p, "\n└");
    for (size_t i = 0; i < count; i++) {
        for (size_t j = 0; j < widths[i] + 2; j++) append_str_raw(&p, "─");
        append_str_raw(&p, (i + 1 < count) ? "┴" : "┘");
    }
    append_str_raw(&p, "\n");

    *p = '\0';
    free(widths);
    return out;
}

void destroy_tape(tape* tape){
    if(tape == NULL) return;
    while(tape->prev != NULL){
        tape = tape->prev;
    }

    while(tape->next != NULL){
        tape = tape->next;
        free(tape->prev);
    }
    free(tape);
}