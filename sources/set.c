#include "../headers/set.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

set* create_set(){
    set* s = calloc(1,sizeof(set));
    string_list_create(&s->data); 
    s->data_length = -1;
    // FIXED LENGTH TO MAX_ELEMENT_SIZE
    s->data_length = MAX_ELEMENT_SIZE+1;
    
    return s;
};

int set_insert(set* s, char* data, size_t size){
    if(s->data_length == -1){
        s->data_length = size;
    }
    
    if(size != s->data_length){
        return 0;
    }

    string_list_add(s->data,data);
    return 1;
}

char* set_to_string(const set* s){
    // Ogni stringa è data_length di lunghezza +2 per " ,"
    size_t size = sizeof(char)*( (s->data_length+2)*(s->data->index)+1);
    char* str = malloc(size);

    int written = 0;
    for(size_t i = 0; i<s->data->index; i++){
        written += snprintf(
            str + written,
            size - written,
            (i < s->data->index - 1) ? "%.*s, " : "%.*s",
            (int)s->data_length,
            s->data->data[i]);
    }
    return str;
}

int set_contains(const set* s, const char* element, size_t size){
    for(size_t i = 0; i<s->data->index; i++){
        if(strncmp(s->data->data[i],element,size) == 0) return 1;
    }
    return 0;
}

set* set_union(const set* a, const set*  b){

    set* s = create_set();
    if(a->data_length != b->data_length) return s;

    
    for(size_t i = 0; i<a->data->index; i++){
        char* elem = a->data->data[i];
        if(!set_contains(s,elem,b->data_length)){
            set_insert(s,elem,s->data_length);
        }
    }

    for(size_t i = 0; i<b->data->index; i++){
        char* elem = b->data->data[i];
        if(!set_contains(s,elem,b->data_length)){
            set_insert(s,elem,b->data_length);
        }
    }
    return s;
    
}

set* set_intersection(const set* a, const set* b){
    set* s = create_set();
    if(a->data_length != b->data_length) return s;
    
    s->data_length = a->data_length;
    for(size_t i = 0; i<a->data->index; i++){
        char* elem = a->data->data[i];
        if(set_contains(b,elem,a->data_length)){
            set_insert(s,elem,a->data_length);
        }
    }
    return s;
}

set* set_difference(const set* a, const set* b){
    set* s = create_set();
    if(a->data_length != b->data_length) return s;
    
    for(size_t i = 0; i<a->data->index; i++){
        char* elem = a->data->data[i];
        
        if(!set_contains(b,elem,a->data_length)){
            set_insert(s,elem,a->data_length);
        }
    }
    return s;
}


set* set_merge_union(set* a, set* b){
    set* s = set_union(a,b);
    free_set_content(a);
    free_set_content(b);
    return s;
}

set* set_merge_intersection(set* a, set* b){
    set* s = set_intersection(a,b);
    free_set_content(a);
    free_set_content(b);
    return s;
}

set* set_merge_difference(set* a, set* b){
    set* s = set_difference(a,b);
    free_set_content(a);
    free_set_content(b);
    return s;
}

void free_set_content(set *set){
    free(set->data->data);
    free(set->data);
    // free(set);
}