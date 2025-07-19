#include "../headers/file_io.h"
#include "../headers/list.h"
#include <stddef.h>
#include <stdlib.h>

int context_file_open(file_context** context, const char *filename){
    *context = malloc(sizeof(file_context));
    (*context)->filename = filename;
    (*context)->fp = fopen(filename, "r");
    (*context)->len = 0;
    return (*context)->fp != NULL;
}

string_list* read_all_chunks(file_context context, size_t chunk_size){

}

int read_next_chunk(file_context* context, char* buffer, size_t buffer_size){
    context->len = fread(buffer, 1, buffer_size, context->fp);
    if(context->len > 0){
        buffer[buffer_size-1] = '\0';
        
        int last = context->len - 1;
        while (last >= 0 && buffer[last] != ' ' && buffer[last] != '\n') {
            last--;
        }

        if (last >= 0 && last < context->len - 1) {
            fseek(context->fp, last - context->len + 1, SEEK_CUR);
            buffer[last + 1] = '\0';
        } else if (last == -1) {
            buffer[context->len] = '\0';
        }
        return 1;
    }
    return 0;
}

void context_file_close(file_context** context){
    free(*context);
    fclose((*context)->fp);
}

char* read_around(file_context* context, int index, int radius){
    int i = index-radius;
    i = i>0? i:0;
    
    if (fseek(context->fp,  i, SEEK_SET) != 0) {
        printf("Error seeking in file");
        fclose(context->fp);
        return 0;
    }

    int num_chars = radius*2 + 1;
    char *buffer = malloc(sizeof(char)*num_chars);
    size_t bytesRead = fread(buffer, sizeof(char), num_chars, context->fp);
    buffer[bytesRead] = '\0';

    return buffer;
}