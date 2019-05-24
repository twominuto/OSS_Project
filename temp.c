#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    UNDEFINED = 0,
    OBJECT = 1,
    ARRAY = 2,
    STRING = 3,
    PRIMITIVE = 4
} type_t ;

typedef struct {
    type_t type ;
    int start ;
    int end ;
    int parent ;
    int size ;
} tok_t ;

int readJSON(int, int) ;
int readPair(int) ;
int readArray(int, int) ;
int readValue(int, int) ;
int readString(int, int, int) ;
int readNumber(int, int) ;

tok_t* token_array = 0x0 ;
int token_index = 0 ;
char *buffer = 0x0 ;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Give file name as args.\n") ;
        exit(-1) ;
    }
    FILE *fp = fopen(argv[1], "r") ;
    char temp_buf[512] ;
    int len=0, s ;
    char *strings[] = {"UNDEFINED", "OBJECT", "ARRAY", "STRING", "PRIMITIVE"} ;

    while((s = fread(temp_buf, sizeof(char), sizeof(temp_buf), fp)) > 0) {
        temp_buf[s] = 0x0;
        if (buffer == 0x0) {
            buffer = strdup(temp_buf);
            len = s;
        } else {
            buffer = realloc(buffer, len+s+1);
            strncpy(buffer+len, temp_buf, s);
            buffer[len+s] = 0x0;
            len += s;
        }
    }

    token_array = (tok_t*)malloc(sizeof(tok_t)*50) ;
    if (token_array == NULL) {
        printf("Error.\n");
        exit(-1) ;
    }
    int start = 0 ;
    while (buffer[start] != '{') {
        start ++ ;
    }
    int end = readJSON(start, -1) ;
    int key = 0 ;
    

    while(key < 3) {
        system("@cls||clear");
        printf("=================================\n") ;
        printf(" Parsing JSON file ...\n") ;
        printf(" 1. Check all token.\n") ;
        printf(" 2. Check a value for key.\n") ;
        printf(" 3. Exit.\n") ;
        printf("=================================\n> ") ;
    
        scanf("%d", &key) ;
        if (key == 1) {
            for (int i=0 ; i<token_index ; i++) {
                // allocate memory to print token's content        
                int strsize = token_array[i].end-token_array[i].start+1 ;
                
                char *content = (char*)malloc((strsize) * sizeof(char)) ;
                memcpy(content, &buffer[token_array[i].start], strsize) ;
                type_t temp = token_array[i].type ;
                content[strsize] = 0x0 ;
                printf("%2d | %s (%s, %d to %d, for key %d, size %d)\n", i, content, strings[temp], token_array[i].start, token_array[i].end, token_array[i].parent, token_array[i].size) ;
                free(content) ;
            }
            getchar() ;
            getchar() ;
        } else if (key == 2) {
            char input[16] ;
            printf("Give a key.\n> ") ;
            scanf("%s", input) ;

            int value = -1 ;

            for (int i=0 ; i<token_index ; i++) {
                int strsize = token_array[i].end-token_array[i].start-1 ;
                char *content = (char*)malloc((strsize) * sizeof(char)) ;
                memcpy(content, &buffer[token_array[i].start+1], strsize) ;
                content[strsize] = 0x0 ;

                if (strcmp(input, content)==0) {
                    value = i ;    
                    break ;
                }
            }
            if (value == -1) {
                printf("No matching value.\n") ;
                getchar() ;
                getchar() ;
                continue ;
            }
            for (int i=0 ; i<token_index ; i++) {
                if (value==token_array[i].parent) {
                    int strsize = token_array[i].end-token_array[i].start+1 ;
                    char *content = (char*)malloc((strsize) * sizeof(char)) ;
                    memcpy(content, &buffer[token_array[i].start], strsize) ;
                    content[strsize] = 0x0 ;

                    printf("%s\n", content) ;
                    break ;
                }
            }
            getchar() ;
            getchar() ;
        }
    }
}


int readPair(int pos) {
    while (buffer[pos]!='"') // 이 부분 " 으로 처리할지 blank space로 처리할지 !
        pos++ ;
    int count = 0 ;
    int start_str = pos ;
    // buffer[start] 부터 쭉 읽어가면서
    // string을 읽어와서 key에 저장
    pos = readString(start_str, -1, 1) ;
    // value를 읽는 함수를 호출
    while (buffer[pos]!=':')
        pos++ ;

    // readString은 다른 함수 호출하지 않으니까 그대로 써도 될듯..?
    int end_val = readValue(pos, token_index-1) ;
    // 전체 structure에 읽은 값 차례대로 저장
    return end_val ;
}

int readJSON(int start, int parent) {
    int position = readPair(start);
    int count = 0 ;
    while(buffer[position]!='}'){
        if (buffer[position]==','){
            position = readPair(position);
            count += 1 ;
        }
        position++;
    }
    
    token_array[token_index].type = OBJECT ;
    token_array[token_index].start = start ;
    token_array[token_index].end = position ;
    token_array[token_index].parent = parent ;
    token_array[token_index].size = count + 1 ;
    token_index += 1 ;

    return position ;
}

int readArray(int curr, int parent)
{
    int start_curr = curr ;
    int count = 0 ;
    while (buffer[curr]!=']') {
        curr += 1;
        if (buffer[curr] == '"') {
            curr = readString(curr, -1, 0) ;
        } else if ( isdigit(buffer[curr]) || buffer[curr]=='-') {
            curr = readNumber(curr, -1) ;
        } else if (buffer[curr] == '{') {
            curr = readJSON(curr, -1) ;
        } else if (buffer[curr] == '[') {
            curr = readArray(curr, -1) ;
        } else if (buffer[curr] == 't' || buffer[curr] == 'f' || buffer[curr] == 'n') {
            while(!isalpha(buffer[curr]))
                curr ++ ;
            token_array[token_index].type = PRIMITIVE ;
            token_array[token_index].start = start_curr ;
            token_array[token_index].end = curr ;
            token_array[token_index].parent = parent ;
            token_array[token_index].size = 0 ;
            token_index += 1 ;
        }
        if (buffer[curr] == ',')
            count += 1 ;
    }
        
    token_array[token_index].type = ARRAY ;
    token_array[token_index].start = start_curr ;
    token_array[token_index].end = curr ;
    token_array[token_index].parent = parent ;
    token_array[token_index].size = count+1 ;
    token_index += 1 ;

    return curr ;
}

int readValue(int curr, int parent)
{
    while (isspace(buffer[curr]) || buffer[curr]==':')
        curr += 1 ;
    int return_pos ;
    
    if (buffer[curr] == '"') {
        return_pos = readString(curr, parent, 0) ;
    } else if ( isdigit(buffer[curr]) || buffer[curr]=='-') {
        return_pos = readNumber(curr, parent) ;
    } else if (buffer[curr] == '{') {
        return_pos = readJSON(curr, parent) ;
    } else if (buffer[curr] == '[') {
        return_pos = readArray(curr, parent) ;
    } else if (buffer[curr] == 't' || buffer[curr] == 'f' || buffer[curr] == 'n') {
        return_pos = curr ;
        while(!isalpha(buffer[return_pos])) {
            return_pos ++ ;
        }
        token_array[token_index].type = PRIMITIVE ;
        token_array[token_index].start = curr ;
        token_array[token_index].end = return_pos ;
        token_array[token_index].parent = parent ;
        token_array[token_index].size = 0 ;
        token_index += 1 ;
    }

    return return_pos;
}


int readString(int start, int parent, int size) {
    int return_pos = start+1 ;
    while(buffer[return_pos]!='"') {
        return_pos += 1 ;
    }
    token_array[token_index].type = STRING ;
    token_array[token_index].start = start ;
    token_array[token_index].end = return_pos ;
    token_array[token_index].parent = parent ;
    token_array[token_index].size = size ;
    token_index += 1 ;
    return return_pos ;
}

int readNumber(int position, int parent) {
    // digit 이 나오지 않을때까지 읽기
    
    // 오류 체크 -> digit이 아닐 경우 함수 탈출
    if(!isdigit(buffer[position]))
        return -1 ;
    int dot = 0;
    int startIndex = position - 1;
    int endIndex;
    while(1){
        if(isdigit(buffer[position]))
            position++;
        else if(buffer[position] == '.'){
            if(++dot == 2){
                //return -1;
            }
            position++;
        }
        else if(buffer[position] == ','){
            endIndex = position -1;
            break;
        }

        else
            break;
    }

    token_array[token_index].type = PRIMITIVE;
    token_array[token_index].start = startIndex;
    token_array[token_index].end = endIndex;
    token_array[token_index].parent = parent ;
    token_array[token_index].size = 0 ;
    token_index++;
    return position-1;
}
