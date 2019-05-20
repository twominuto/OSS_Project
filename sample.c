// void main(int argc, char** argv) {

// }
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int curr_pos = 0;
char *buffer;
int size;
int i; 
char** token = 0x0; 
int tokenIndex=0;

int readJSON(int);
int readPair(int);
int readArray(int);
int readValue(int);
int readString(int);
int readNumber(int);

int main()
{

    FILE *fp = fopen("input.json", "r");

    // 파일 크기 구하기
    fseek(fp, 0, SEEK_END); //fseek(filePointer, 처음위치, SEEK_ )
    size = ftell(fp);       //ftell(filePointer) = 현재 위치
    fseek(fp, 0, SEEK_SET);

    // 파일 크기 + NULL 공간만큼 메모리를 할당하고 0으로 초기화
    buffer = malloc(size + 1);   //void * memset ( void * ptr, int value, size_t num );
    memset(buffer, 0, size + 1); //memset(채우고자 하는 메모리의 시작 포인터, 메모리에 채우고자 하는 값(int형이지만, 내부에서는 unsigned char 1byte로 변환되어 저장됨), 채우고자 하는 바이트 수)

    fread(buffer, size, 1, fp);
    for (int i = 0; i < size; i++)
    {
        printf("%c", buffer[i]);
    }

    printf("%d", curr_pos);

    while (buffer[curr_pos] != '{')
    {
        curr_pos++;
    }
    //printf("%d", curr_pos);

    //int end = readJSON(curr_pos) ;
}

/*
int readPair(int) {
    // buffer[start] 부터 쭉 읽어가면서
    // string을 읽어와서 key에 저장
    // value를 읽는 함수를 호출
    // 함수가 종료될떄 끝난 인덱스 리턴

    // 전체 structure에 읽은 값 차례대로 저장
}

int readJSON(int start) {
    //  readPair 호출
    // comma가 나오지 않을 때까지 호출
}
*/

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

/*
int readString(int) {
    // "이 나올때까지 계속해서 읽는데, 이때 \ 유의하면서 읽기
}

int readNumber(int) {
    // digit 이 나오지 않을때까지 읽기
}
*/