
#include "../headers/lexer.h"
#include <string.h>
#include <stdlib.h>

/*
TODO: includere le parentesi per l'operazione tra insiemi
*/
int tokenize(char* string, int string_size, token_list** list, lexing_info* info){
    if(*list == 0) token_list_create(list);
    int i = 0;
    while(i < string_size && string[i] != '\0'){
        int new_pos = insert_next_token(string, i, string_size, *list,info); 
        info->inline_char_index += new_pos - i;
        info->real_char_num += new_pos - i;
        i = new_pos;
    }
    return 1;
}

int insert_next_token(char* string, int start_position, int string_size, token_list *list, lexing_info* info){
    char identifier_name[MAX_VAR_NAME] = {0};
    int k = 0;

    for(int i = start_position; i<string_size && string[i] != '\0'; i++){
        char c = string[i++];
        if(info->commented && c != '\n') return i;
        switch (c) {
            case '\n':  info->line++; info->inline_char_index = -1; info->commented = 0; return i;
            case '#':   info->commented = 1;                        return i;
            case '<':   add_token(INSTRUCTION_START,0,0,list,info); return i;
            case '>':   add_token(INSTRUCTION_END  ,0,0,list,info); return i;
            case ',':   add_token(COMMA            ,0,0,list,info); return i;
            case '=':   add_token(ASSIGN           ,0,0,list,info); return i;
            case '-':   add_token(SET_DIFFERENCE   ,0,0,list,info); return i;
            case '(':   add_token(OPEN_PARENTESIS  ,0,0,list,info); return i;
            case ')':   add_token(CLOSE_PARENTESIS ,0,0,list,info); return i;
            //Ignora spazi vuoti
            case ' ':
            case '\r':
            case '\t':
                return i;
            case '\\':
                    if(string[i] == '{'){
                        add_token(SET_START,0,0,list,info); return i+1;
                    }
                    
                    if(string[i] == '}'){
                        add_token(SET_END,0,0,list,info); return i+1;
                    } 

            case 'R':
            case 'L':
            case 'S':
            default:
                identifier_name[k++] = c;
                while(i<string_size && string[i] != '\0' && character(string[i])){
                    identifier_name[k++] = string[i];
                    if(k > MAX_VAR_NAME-1) break;
                    i++;
                }
                int identifier = get_identifier(identifier_name,k);
                if( identifier > 0){
                    
                    //BLANK CON DATI
                    if(identifier == BLANK){
                        add_token(identifier,"\t",2,list,info);
                    }else{
                        add_token(identifier,0,0,list,info);
                    }

                }else{
                    k++;
                    char* str = malloc(sizeof(char)*(k));
                    strncpy(str,identifier_name,k);
                    add_token(IDENTIFIER,str,k,list,info);
                }
        }
        return i;
    }
    return start_position+1;
    
}
//:3

int get_identifier(char identifier[MAX_VAR_NAME], int var_name_size){
    if(var_name_size > MAX_TOKEN_LEN) return -1;
    switch (var_name_size) {
        case 3: 
            if(strncmp(identifier,"\\in", MAX_VAR_NAME) == 0)
                return IN; 
        case 4:
            if(strncmp(identifier,"Left", MAX_VAR_NAME) == 0)
                return ACTION_LEFT;
            if(strncmp(identifier,"Stop", MAX_VAR_NAME) == 0)
                return ACTION_STOP;
            if(strncmp(identifier,"\\cup", MAX_VAR_NAME) == 0)
                return SET_UNION;
            if(strncmp(identifier,"\\cap", MAX_VAR_NAME) == 0)
                return SET_INTERSECTION;
        case 5:
            if(strncmp(identifier,"Right", MAX_VAR_NAME) == 0)
                return ACTION_RIGTH;
            
        case 6:
            if(strncmp(identifier,"\\Sigma", MAX_VAR_NAME) == 0)
                return SIGMA;
        case 7:
            if(strncmp(identifier,"\\square", MAX_VAR_NAME) == 0)
                return BLANK;
            if(strncmp(identifier,"\\forall", MAX_VAR_NAME) == 0)
                return FORALL;
            
        default: return -1;
    }
    
}

//mettite a fa reti dai! 

int character(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
            c == '_';
}

int add_token(enum TOKEN_TYPE type, char* data, size_t data_size, token_list* tokens, const lexing_info* info){
    token_list_add(tokens, (token){type,data,data_size,info->line,info->inline_char_index+1,info->real_char_num});
    return 0;
}

void write_end(token_list* tokens, const lexing_info* info){
    add_token(END_OF_FILE,0,0,tokens,info);
}

int create_lexing_info(lexing_info** info){
    (*info) = malloc(sizeof(lexing_info));
    if(*info == NULL) return 0;
    (*info)->line = 1;
    (*info)->inline_char_index = 0;
    (*info)->commented = 0;
    return 1;
}









//ciola