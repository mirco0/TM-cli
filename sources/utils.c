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

// Removed buggy implementation TODO: da rifare
void append_str(char** dest, const char* fmt, ...);