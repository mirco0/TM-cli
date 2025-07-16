#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#define MAX_TOKEN_LEN 10

extern const char* TOKEN_TYPES_[];
enum TOKEN_TYPE {
    END_OF_FILE,
    INSTRUCTION_START,
    INSTRUCTION_END,
    COMMA,
    SET_START,
    SET_END,
    ASSIGN,
    OPEN_PARENTESIS,
    CLOSE_PARENTESIS,

    IDENTIFIER,
    STRING,

    SIGMA,
    FORALL,
    IN,
    SET_UNION,
    SET_DIFFERENCE,
    SET_INTERSECTION,
    BLANK

};

typedef struct {
    int line;
    int inline_char_index;
    int commented;
} parsing_info;

typedef struct {
    enum TOKEN_TYPE type;
    char* data;
    size_t data_size;
    int line_num;
    int char_num;
} token;

char* token_to_string(const token*);
char* error_token_to_string(const token*);

#endif