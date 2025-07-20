#ifndef HASHTABLE_H
#define HASHTABLE_H

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

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 16

typedef struct {
    const char* key;
    void* value;
} ht_entry;

typedef struct {
    ht_entry* data;
    size_t capacity;
    size_t index;
} hashtable;

typedef struct {
    const char* key;
    void* value;

    hashtable* _table;
    size_t _index;
} hti;

hashtable* ht_create();
uint64_t hash(const char *str);
void* ht_get(hashtable* table, const char* key);
const char* ht_set(hashtable* table, const char* key, void* value);
const char* ht_set_entry(ht_entry* entries, size_t capacity, const char* key, void* value, size_t* plength);
size_t ht_length(hashtable* table);
hti ht_iterator(hashtable* table);
int ht_next(hti* it);
void ht_destroy(hashtable* table);

#endif