#include "../headers/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* repeat(char ch, int repeat){
    char* str = malloc(sizeof(char)*(repeat+1));
    for(int i = 0; i<repeat; i++){
        str[i] = ch;
    }
    str[repeat] = '\0';
    return str;
}

void append_str(char** dest, const char* fmt, ...){
    va_list args;
    va_start(args, fmt);

    char* temp;
    vasprintf(&temp, fmt, args);
    va_end(args);

    if (*dest == NULL) {
        *dest = temp;
    } else {
        char* new_str;
        asprintf(&new_str, "%s%s", *dest, temp);
        free(*dest);
        free(temp);
        *dest = new_str;
    }

}
