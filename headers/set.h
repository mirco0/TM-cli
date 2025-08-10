#ifndef SET_H
#define SET_H
#include "hashtable.h"
#include "list.h"

typedef hashtable set;

set* create_set();
int set_insert(set* s, char* data);
char* set_to_string(set* s);
int set_contains(set* s, const char* element);
set* set_union(set* a, set* b);
set* set_intersection(set* a, set* b);
set* set_difference(set* a, set* b);
set* set_copy(set* s);
string_list* set_to_list(set* s);

//Implementazioni che distruggono i set dopo l'operazione
set* set_merge_union(set* a, set* b);
set* set_merge_intersection(set* a, set* b);
set* set_merge_difference(set* a, set* b);
void free_set_content(set* set);

#endif