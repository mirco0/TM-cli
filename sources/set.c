#include "../headers/set.h"
#include "../headers/hashtable.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

//Valore per identificare l'elemento presente
static void* EXISTS = (void*)1;

set* create_set(){
    return ht_create();
}

int set_insert(set *s, char *data){
    ht_set(s,data,EXISTS);
    return 0;
}

int set_contains(set *s, const char *element){
    return ht_get(s,element) == EXISTS;
}

set* set_union(set* a, set* b){
    hti iter_a = ht_iterator(a);
    hti iter_b = ht_iterator(b);
    hashtable* ht = ht_create();
    set* s = malloc(sizeof(set));
    while(ht_next(&iter_a)){
        ht_set(ht,iter_a.key,iter_a.value);
    }
    
    while(ht_next(&iter_b)){
        ht_set(ht,iter_b.key,EXISTS);
    }
    
    s = ht;
    return s;
}

set* set_intersection(set* a, set* b){
    hti iter_a = ht_iterator(a);
    hti iter_b = ht_iterator(b);
    hashtable* ht = ht_create();
    set* s = malloc(sizeof(set));
    
    while(ht_next(&iter_a)){
        if(set_contains(b,iter_a.key)){
            ht_set(ht,iter_a.key,EXISTS);
        }
    }
    
    while(ht_next(&iter_b)){
        if(set_contains(a,iter_b.key)){
            ht_set(ht,iter_b.key,EXISTS);
        }
    }
    
    s = ht;
    return s;
}

set* set_difference(set* a, set* b){
    hti iter_a = ht_iterator(a);
    hashtable* ht = ht_create();
    set* s = malloc(sizeof(set));
    
    while(ht_next(&iter_a)){
        if(!set_contains(b, iter_a.key)){
            ht_set(ht,iter_a.key,EXISTS);
        }
    }

    s = ht;
    return s;
}

char* set_to_string(set* s){
    if(s == NULL){
        return NULL;
    }
    hti iter_s = ht_iterator(s);
    size_t total_chars = 0;
    size_t count = 0;

    while(ht_next(&iter_s)){
        total_chars += strlen(iter_s.key) + 2;
        count++;
}

    char* str = malloc(total_chars + 1);
    if (str == NULL) {
        return NULL;
    }

    size_t written = 0;
    iter_s = ht_iterator(s);
    size_t i = 0;
    while(ht_next(&iter_s)){
        if (i < count - 1)
            written += snprintf(str + written, total_chars + 1 - written, "%s, ", iter_s.key);
        else
            written += snprintf(str + written, total_chars + 1 - written, "%s", iter_s.key);
        i++;
    }
    return str;

}

set* set_merge_union(set* a, set* b){
    hti iter_b = ht_iterator(b);
    while (ht_next(&iter_b)) {
        ht_set(a,iter_b.key,EXISTS);
    }
    ht_destroy(b);
    return a;
}

set* set_merge_intersection(set* a, set* b){
    set* s = set_intersection(a,b);
    ht_destroy(a);
    ht_destroy(b);
    return s;
    
}

set* set_merge_difference(set* a, set* b){
    set* s = set_difference(a,b);
    ht_destroy(a);
    ht_destroy(b);
    return s;
    
}

void free_set_content(set* set);