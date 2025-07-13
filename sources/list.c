#include "../headers/list.h"
#include "../headers/token.h"
#include <stdio.h>
#include <stdlib.h>

int token_list_create(token_list** list){
    *list = malloc(sizeof(token_list));
    (*list)->data = malloc(64*sizeof(token));
    if(*list == 0){
        return 0;
    }
    (*list)->capacity = 64;
    (*list)->index = 0;
    return 1;
}

int token_list_add(token_list* list, token token_item){
    if(list->capacity <= list->index){
        token* new_list = malloc(list->capacity*2*sizeof(token));

        if(new_list == 0){
            return 0;
        }
        list->capacity = list->capacity*2;
        for(size_t i = 0; i<list->index; i++){
            new_list[i] = list->data[i];
        }
        free(list->data);
        list->data = new_list;
    }
    list->data[list->index++] = token_item;
    return 1;
}

int print_token_list(token_list* list){
    if(list == NULL) return 0;
    for(size_t i = 0; i<list->index; i++){
        printf("%s\n",token_to_string(&list->data[i]));
    }
    printf("\n");
    return 0;
}


int string_list_create(string_list** list){
    *list = malloc(sizeof(string_list));
    (*list)->data = malloc(64*sizeof(char*));
    if(*list == 0){
        return 0;
    }
    (*list)->capacity = 64;
    (*list)->index = 0;
    return 1;
}


int string_list_add(string_list* list, char* string){
    if(list->capacity <= list->index){
        char** new_list = malloc(list->capacity*2*sizeof(char*));

        if(new_list == 0){
            return 0;
        }
        list->capacity = list->capacity*2;
        for(size_t i = 0; i<list->index; i++){
            new_list[i] = list->data[i];
        }
        free(list->data);
        list->data = new_list;
    }
    list->data[list->index++] = string;
    return 1;

}

int expression_list_create(expression_list** list){
    *list = malloc(sizeof(expression_list));
    (*list)->data = malloc(64*sizeof(struct expression*));
    if(*list == 0){
        return 0;
    }
    (*list)->capacity = 64;
    (*list)->index = 0;
    return 1;
}


int expression_list_add(expression_list* list, struct expression* expression){
    if(list->capacity <= list->index){
        printf("Malloc-ing %zd bytes:\n",list->capacity);
        struct expression** new_list = malloc(list->capacity*2*sizeof(struct expression*));

        if(new_list == 0){
            return 0;
        }

        list->capacity = list->capacity*2;
        for(size_t i = 0; i<list->index; i++){
            new_list[i] = list->data[i];
        }
        free(list->data);
        list->data = new_list;
    }
    list->data[list->index++] = expression;
    return 1;
}