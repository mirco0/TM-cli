#ifndef SET_H
#define SET_H

#include <stdlib.h>
#define MAX_ELEMENT_SIZE 1

#include "list.h"
#include <stdio.h>

typedef struct {
    string_list* data;
    size_t data_length;
} set;

// TODO: FARE IMPLEMENTAZIONE SET EFFETTIVA CON HASH

set* create_set();
int set_insert(set* s, char* data, size_t size);
char* set_to_string(const set* s);
int set_contains(const set* s, const char* element, size_t size);
set* set_union(const set* a, const set* b);
set* set_intersection(const set* a, const set* b);
set* set_difference(const set* a, const set* b);

//Implementazioni che distruggono i set dopo l'operazione
set* set_merge_union(set* a, set* b);
set* set_merge_intersection(set* a, set* b);
set* set_merge_difference(set* a, set* b);
void free_set_content(set* set);

#endif