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
} tok_t ;

int readJSON(int) ;
int readPair(int) ;
int readArray(int) ;
int readValue(int) ;
int readString(int) ;
int readNumber(int) ;

tok_t* token_array = 0x0 ;
int token_index = 0 ;
char *buffer = 0x0 ;
int curr_pos ;

int main(int argc, char *argv[])
{
    if (argc == 1)
        argv[1] = "input.json" ;
    FILE *fp = fopen(argv[1], "r") ;
    char temp_buf[512] ;
    int len=0, s ;

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

    int i=0 ;
    while (buffer[i] != '{') {
        curr_pos ++ ;
    }
    int start = curr_pos ;
    int end = readJSON(curr_pos) ;

    token_array[token_index].type = OBJECT ;
    token_array[token_index].start = start ;
    token_array[token_index].end = end ;
    token_index += 1 ;
}


int readPair(int pos) {
    while (buffer[pos]!='\"') // 이 부분 " 으로 처리할지 blank space로 처리할지 !
        pos++ ;

    int start_str = pos ;
    // buffer[start] 부터 쭉 읽어가면서
    // string을 읽어와서 key에 저장
    int end_str = readString(start_str) ;
    pos = end_str ;

    // value를 읽는 함수를 호출
    while (buffer[pos]!=':')
        pos++ ;

    int start_val = pos ;
    int end_val = readValue(start_val) ;
    // 전체 structure에 읽은 값 차례대로 저장
    return end_val ;
}

int readJSON(int start) {
    int position = start + 1;
    while(1){
        if (buffer[start]==','){
            readPair(position);
            position++;
            }
        else if (buffer[start]=='}'){
            break;
            }
    }
    return position - 1;
    //  readPair 호출
    // comma가 나오지 않을 때까지 호출

    // }가 나오면 종료 해주기
}

//buffer가 아니라 dynamic memory allocation 
int readArray(int curr)
{
    // if(buffer[i] == '[') 이면 readArray()호출 할 때 curr_pos++;
    while(buffer[curr_pos] != ']') {
        if(buffer[curr_pos] == ',') readArray(curr_pos++);
        if(buffer[curr_pos] == '"'){
            char *begin = buffer +curr_pos+1;
            char *end = strchr(begin, '"'); //가장 먼저 나오는 " 찾기
            if(end == NULL) break; // "가 없을 경우 반복 종료
            int stringLength = end-begin; //ex) "abc" begin=a, end="
 
           // token = (char**)realloc(sizeof(char*)*1);
            token = (char**)malloc(1+stringLength);
            memcpy(&token[tokenIndex][0], begin, stringLength); 
            //void *memcpy(void *dest, const void *src, size_t count);
            //src의 count byte를 dest로 복사

            tokenIndex++;// 토큰 인덱스 증가 
            curr_pos += stringLength + 1; //현재 위치 + 문자열 길이
        }
        curr_pos++;
    }
    // value를 읽어냄
    // comma를 또 읽으면 append한다고 생각하고 자기자신 재 호출
    // ']'를 읽으면 현재 position return
    return curr_pos;
}

int readValue(int curr)
{
    if(buffer[curr_pos] == '"') 
        readString(curr_pos++);
    //if(buffer[curr_pos] == '-' || buffer[curr_pos] == "[0-9]") 숫자랑 비교해서 Regexp 안됨
    if(buffer[curr_pos] == '-' || buffer[curr_pos] == '0'|| buffer[curr_pos] == '1'
    || buffer[curr_pos] == '2'|| buffer[curr_pos] == '3'|| buffer[curr_pos] == '4'
    || buffer[curr_pos] == '5'|| buffer[curr_pos] == '6'|| buffer[curr_pos] == '7'
    || buffer[curr_pos] == '8'|| buffer[curr_pos] == '9')
        readNumber(curr_pos++);
    if(buffer[curr_pos] == '{') 
        readJSON(curr_pos++);
    if(buffer[curr_pos] == '[')
        readArray(curr_pos++);
    if(buffer[curr_pos] == 't' || buffer[curr_pos] == 'f' || buffer[curr_pos] == 'n'){ 
        char *begin = &buffer[curr_pos];
        if(strncmp(begin, "true", 4) == 0) {
            token = (char**)realloc(token,4);
            memcpy(&token[tokenIndex++][0], begin, 4);
            curr_pos += 4;
        }
        if(strncmp(begin, "false", 5) == 0) {
            token = (char**)realloc(token,5);            
            memcpy(&token[tokenIndex++][0], begin, 5);
            curr_pos += 5;
        }     
        if(strncmp(begin, "null", 4) == 0) {
            token = (char**)realloc(token,4);            
            memcpy(&token[tokenIndex++][0], begin, 4);
            curr_pos += 4;
        }        
    }
    return curr_pos;

    // '"' 를 읽으면 readString을
    // digit or '-'를 읽으면 readNumber를
    // '{' 를 읽으면  readJSON
    // '[' 를 읽으면 readArray를 호출
    // true / false / null 등이 json에서 어떻게 표현되는지 확인
    return curr_pos;
}


int readString(int start) {
    int pos = start + 1;
    while(1){
        if (isalpha(buffer[pos]) || buffer[pos] == ' '){
            pos++;
        }
        else if (buffer[pos]=='\"'){
            break;
        }
        return pos - 1;
    }
    // "이 나올때까지 계속해서 읽는데, 이때 \ 유의하면서 읽기
    //?
}

int readNumber(int startIndex) {
    // digit 이 나오지 않을때까지 읽기
    int dot = 0;
    int position = startIndex + 1;

    while(1){
        if(isdigit(buffer[position]))
            position++;
        else if(buffer[position] == '.'){
            if(++dot == 2){
                return -1;
            }
            position++;
        }
        else if(buffer[position] == ',')
            break;
        else
            return -1;
    }
    return position-1;
}
