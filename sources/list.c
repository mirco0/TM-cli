#include "../headers/list.h"
#include "../headers/token.h"
#include "../headers/parser/expression.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int token_list_create(token_list** list){
    *list = malloc(sizeof(token_list));
    if(*list == 0){
        return 0;
    }
    (*list)->data = malloc(64*sizeof(token));
    (*list)->capacity = 64;
    (*list)->index = 0;
    return 1;
}

int token_list_add(token_list* list, token token_item){
    if(list->capacity <= list->index){
        if(list->capacity > list->capacity*2){
            return 0;
        }
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

void destroy_token_list(token_list* list){
    if(list == NULL) return;
    free(list->data);
    free(list);
}


int string_list_create(string_list** list){
    *list = malloc(sizeof(string_list));
    if(*list == 0){
        return 0;
    }
    (*list)->data = malloc(64*sizeof(char*));
    (*list)->capacity = 64;
    (*list)->index = 0;
    return 1;
}

/*String must be allocated, will be freed on destroy*/
int string_list_add(string_list* list, char* string){
    if(list->capacity <= list->index){
        if(list->capacity > list->capacity*2){
            return 0;
        }
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

int print_string_list(string_list* list){
    if(list == NULL) return 0;
    for(size_t i = 0; i<list->index; i++){
        printf("%s\n",list->data[i]);
    }
    printf("\n");
    return 0;
}

void destroy_string_list(string_list* list){
    if(list == NULL) return;
    for(size_t i = 0; i<list->index; i++){
        free(list->data[i]);
    }
    free(list->data);
    free(list);
}


int expression_list_create(expression_list** list){
    *list = malloc(sizeof(expression_list));
    if(*list == 0){
        return 0;
    }
    (*list)->data = malloc(64*sizeof(struct expression*));
    (*list)->capacity = 64;
    (*list)->index = 0;
    return 1;
}

/* expression must be allocated, will be freed on destroy */
int expression_list_add(expression_list* list, struct expression* expression){
    if(list->capacity <= list->index){
        if(list->capacity > list->capacity*2){
            return 0;
        }
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

void destroy_expression_list(expression_list* list){
    if(list == NULL) return;
    for(size_t i = 0; i<list->index; i++){
        destroy_expression(list->data[i]);
    }
    free(list->data);
    free(list);
}
