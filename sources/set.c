#include "../headers/set.h"
#include "../headers/hashtable.h"
#include "../headers/list.h"
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
    if(s == NULL) return 0;
    return ht_get(s,element) == EXISTS;
}

set* set_union(set* a, set* b){
    hti iter_a = ht_iterator(a);
    hti iter_b = ht_iterator(b);
    hashtable* ht = ht_create();
    while(ht_next(&iter_a)){
        ht_set(ht,iter_a.key,EXISTS);
    }
    
    while(ht_next(&iter_b)){
        ht_set(ht,iter_b.key,EXISTS);
    }
    
    return ht;
}

set* set_intersection(set* a, set* b){
    hti iter_a = ht_iterator(a);
    hti iter_b = ht_iterator(b);
    hashtable* ht = ht_create();
    
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
    
    return ht;
}

set* set_difference(set* a, set* b){
    hti iter_a = ht_iterator(a);
    hashtable* ht = ht_create();
    
    while(ht_next(&iter_a)){
        if(!set_contains(b, iter_a.key)){
            ht_set(ht,iter_a.key,EXISTS);
        }
    }

    return ht;
}

set* set_copy(set* s){
    hti iter = ht_iterator(s);
    hashtable* ht = ht_create();
    while(ht_next(&iter)){
        ht_set(ht,iter.key,EXISTS);
    }
    return ht;
}

string_list* set_to_list(set* s){
    if(s == NULL) return NULL;
    string_list* list;
    if(!string_list_create(&list)){
        return NULL;
    }
    
    hti iter = ht_iterator(s);
    while(ht_next(&iter)){
        char* value = strdup(iter.key);
        string_list_add(list, value);
    }
    return list;
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