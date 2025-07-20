#include "../headers/hashtable.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

// Resource: https://benhoyt.com/writings/hash-table-in-c/
/*
MIT License

Copyright (c) 2021 Ben Hoyt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

hashtable* ht_create() {
    hashtable* table = malloc(sizeof(hashtable));
    if (table == NULL) {
        return NULL;
    }
    table->index = 0;
    table->capacity = INITIAL_CAPACITY;

    table->data = calloc(table->capacity, sizeof(ht_entry));
    if (table->data == NULL) {
        free(table);
        return NULL;
    }
    return table;
}

uint64_t hash(const char *str){
    uint64_t hash = 5381;
    int c;
    while ((c = *str++))
         hash = ((hash << 5) + hash) + (uint64_t)c; // hash * 33 + c
    return hash;
}

void* ht_get(hashtable* table, const char* key){
    uint64_t hash_value = hash(key);
    size_t index = (size_t)(hash_value & (uint64_t)(table->capacity - 1));

    while (table->data[index].key != NULL) {
        if (strcmp(key, table->data[index].key) == 0) {
            return table->data[index].value;
        }
        index++;
        if (index >= table->capacity) {
            index = 0;
        }
    }
    return NULL;
}

const char* ht_set(hashtable* table, const char* key, void* value) {
    if (value == NULL) {
        return NULL;
    }

    if (table->index >= table->capacity / 2) {
        size_t new_capacity = table->capacity * 2;
        if (new_capacity < table->capacity) {
            return NULL;
        }
        
        ht_entry* new_data = calloc(new_capacity, sizeof(ht_entry));
        if (new_data == NULL) {
            return NULL;
        }

        for (size_t i = 0; i < table->capacity; i++) {
            ht_entry entry = table->data[i];
            if (entry.key != NULL) {
                ht_set_entry(new_data, new_capacity, entry.key, entry.value, NULL);
            }
        }

        free(table->data);
        table->data = new_data;
        table->capacity = new_capacity;
    }
    return ht_set_entry(table->data, table->capacity, key, value,&table->index);
}

const char* ht_set_entry(ht_entry* entries, size_t capacity, const char* key, void* value, size_t* plength) {
    uint64_t hash_value = hash(key);
    size_t index = (size_t)(hash_value & (uint64_t)(capacity - 1));

    while (entries[index].key != NULL) {
        if (strcmp(key, entries[index].key) == 0) {
            entries[index].value = value;
            return entries[index].key;
        }
        index++;
        if (index >= capacity) {
            index = 0;
        }
    }

    if (plength != NULL) {
        key = strdup(key);
        if (key == NULL) {
            return NULL;
        }
        (*plength)++;
    }
    entries[index].key = (char*)key;
    entries[index].value = value;
    return key;
}

size_t ht_length(hashtable* table) {
    return table->index;
}

hti ht_iterator(hashtable* table) {
    hti it;
    it._table = table;
    it._index = 0;
    return it;
}

int ht_next(hti* it) {
    hashtable* table = it->_table;
    while (it->_index < table->capacity) {
        size_t i = it->_index;
        it->_index++;
        if (table->data[i].key != NULL) {
            ht_entry entry = table->data[i];
            it->key = entry.key;
            it->value = entry.value;
            return 1;
        }
    }
    return 0;
}

void ht_destroy(hashtable* table) {
    if (table == NULL) return; 
    for (size_t i = 0; i < table->capacity; i++) {
        free((void*)table->data[i].key);
    }

    free(table->data);
    free(table);
}