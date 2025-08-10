#include "../headers/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* repeat(char ch, int repeat){
    if (repeat <= 0) {
        char* empty = malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }
    char* str = malloc(sizeof(char)*(repeat+1));
    if (str == NULL) return NULL;
    memset(str, ch, repeat);
    str[repeat] = '\0';
    return str;
}
void append_str_raw(char** dest, const char* str){
    size_t len = strlen(str);
    memcpy(*dest, str, len);
    *dest += len;
}

void append_str(char** dest, const char* fmt, ...){
    va_list args;
    va_start(args, fmt);

    char* temp;
    int len = vasprintf(&temp, fmt, args);
    if (len == -1) return; 
    va_end(args);

    if (*dest == NULL) {
        *dest = temp;
    } else {
        size_t old_len = strlen(*dest);
        char* new_str = realloc(*dest, old_len + len + 1);
        if (!new_str) {
            free(temp);
            return;
        }
        memcpy(new_str + old_len, temp, len + 1);
        free(temp);
        *dest = new_str;
    }
}
