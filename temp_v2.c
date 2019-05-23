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

int cur_pos = 0;
tok_t* token_array = 0x0 ;
int token_index = 0 ;
char *buffer = 0x0 ;

int main(int argc, char *argv[])
{
//    if (argc != 2) {
//        printf("Give file name as args.\n") ;
//        exit(-1) ;
//    }
    FILE *fp = fopen("input.json", "r") ;
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
    printf("%s\n", buffer) ;
    //printf("%c\n", buffer[83]);
    token_array = (tok_t*)malloc(sizeof(tok_t)*50) ;
    if (token_array == NULL) {
        printf("Error.\n");
        exit(-1) ;
    }
    int start = 0 ;
    while (buffer[start] != '{') {
        start ++ ;
    }
    int end = readJSON(start) ;

    for (int i=0 ; i<token_index ; i++) {
        type_t temp = token_array[i].type ;
        printf("%2d | %u | %d %d\n", i, temp, token_array[i].start, token_array[i].end) ;
    }
}


int readPair(int pos) {
    printf("pair hi\n") ;
    while (buffer[cur_pos]!='"') // 이 부분 " 으로 처리할지 blank space로 처리할지 !
        cur_pos++ ;
    printf("\ncurPos is %d",cur_pos);
    printf("\n $$$ %c %c %c $$$$$$",buffer[5],buffer[6],buffer[11]);
    int start_str = cur_pos+1 ;
    // buffer[start] 부터 쭉 읽어가면서
    // string을 읽어와서 key에 저장
    int end_str = readString(start_str) ;
    cur_pos = end_str ;

    // value를 읽는 함수를 호출
    while (buffer[cur_pos]!=':')
        cur_pos++ ;

    int start_val = cur_pos ;
    int end_val = readValue(start_val) ;
    // 전체 structure에 읽은 값 차례대로 저장
    return end_val ;
}

int readJSON(int start) {
    printf("JSON hi\n") ;
    int position = start + 1;
    cur_pos = position;
    printf("\ncurPos in readJSON is %d",cur_pos);
    while(1){
        if (buffer[position]==','){
            printf("\ncurPos in readJSON is %d",position);
            //printf("\n dasdasdasd %c",buffer[42]);
            readPair(position);
        } else if (buffer[position]=='}'){
            break;
        }
        position++;
    }
    token_array[token_index].type = OBJECT ;
    token_array[token_index].start = start ;
    token_array[token_index].end = position ;
    token_index += 1 ;

    return position - 1;
    //  readPair 호출
    // comma가 나오지 않을 때까지 호출

    // }가 나오면 종료 해주기
}

//buffer가 아니라 dynamic memory allocation
int readArray(int curr)
{
    int openCnt = 1;
    int closeCnt = 0;
    printf("array \n") ;
    int start_curr = curr ;
    curr++;
    while (1){
        if(buffer[curr] == '[')
            openCnt++;
        else if(buffer[curr] ==  ']')
            closeCnt++;
        if(openCnt == closeCnt)
            break;
        curr += 1;
    }


    token_array[token_index].type = ARRAY ;
    token_array[token_index].start = start_curr ;
    token_array[token_index].end = curr ;
    token_index += 1 ;

    return curr ;
}

int readValue(int curr)
{
    while (isblank(buffer[curr]) || buffer[curr]==':')
        curr += 1 ;
    printf("value %c\n", buffer[32]) ;
    int return_pos ;

    if (buffer[curr] == '"') {
        return_pos = readString(curr) ;
    } else if ( isdigit(buffer[curr]) || buffer[curr]=='-') {
        return_pos = readNumber(curr) ;
    } else if (buffer[curr] == '{') {
        return_pos = readJSON(curr) ;
    } else if (buffer[curr] == '[') {
        return_pos = readArray(curr) ;
    } else if (buffer[curr] == 't' || buffer[curr] == 'f' || buffer[curr] == 'n') {
        return_pos = curr ;
        while(!isalpha(buffer[return_pos])) {
            return_pos ++ ;
        }

        token_array[token_index].type = PRIMITIVE ;
        token_array[token_index].start = curr ;
        token_array[token_index].end = return_pos ;
        token_index += 1 ;
    }

    return return_pos;
}


int readString(int start) {
    printf("string \n") ;

    int return_pos = start ;
    while(buffer[return_pos]!='"') {
        return_pos += 1 ;
    }
    token_array[token_index].type = STRING ;
    token_array[token_index].start = start ;
    token_array[token_index].end = return_pos -1 ;
    token_index += 1 ;
    return return_pos ;
}

int readNumber(int position) {
    printf("number\n") ;
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
    token_index++;
    return position-1;
}
