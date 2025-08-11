#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include "../headers/list.h"

typedef struct {
    FILE *fp;
    const char *filename;
    size_t len;
} file_context;

int context_file_open(file_context** context, const char *filename);
string_list* read_all_chunks(file_context* context, size_t chunk_size);
int read_next_chunk(file_context* context, char* buffer, size_t buffer_size);
char* read_around(file_context* context, int index, int radius);
void context_file_close(file_context* context);

#endif