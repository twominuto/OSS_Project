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
int array_index = 0 ;
char *buffer = 0x0 ;
int curr_pos ;

int main(int argc, char *argv[])
{
    if (argc == 1)
        argv[1] = "input.json" ;
    FILE *fp = fopen(argv[1], "r") ;
    char temp_buf[512] ;
    int len=0, s ;

    // read input json and save in buffer
    // `len` means whole length
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

    token_array = (tok_t*)malloc(sizeof(tok_t)*len) ;
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

    token_array[array_index++] = {OBJECT, start, end} ;
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
    token_array[array_index++] = {start_str, end_str} ;
    token_array[array_index++] = {start_val, end_val} ;

    return end_val ;
}

int readJSON(int start) {
    //  readPair 호출
    // comma가 나오지 않을 때까지 호출

    // }가 나오면 종료 해주기
}

int readArray(int) {
    // value를 읽어냄
    // comma를 또 읽으면 append한다고 생각하고 자기자신 재 호출
    // ']'를 읽으면 현재 position return
}

int readValue(int) {
    // '"' 를 읽으면 readString을
    // digit or '-'를 읽으면 readNumber를
    // '{' 를 읽으면  readJSON
    // '[' 를 읽으면 readArray를 호출
    // true / false / null 등이 json에서 어떻게 표현되는지 확인
}

int readString(int) {
    // "이 나올때까지 계속해서 읽는데, 이때 \ 유의하면서 읽기
}

int readNumber(int startIndex) {
    // digit 이 나오지 않을때까지 읽기
    int position = startIndex + 1;
    while(1){
        if(isdigit(buffer[position]))
            position++;
        else
            break;
    }
    return position - 1;
}
