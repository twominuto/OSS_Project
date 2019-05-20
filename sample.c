#include <stdio.h>
#include <string.h>

int curr_pos ;
char *buffer;
int size;

int readJSON(int) ;
int readPair(int) ;
int readArray(int) ;
int readValue(int) ;
int readString(int) ;
int readNumber(int) ;

int main()
{
    FILE *fp = fopen("input.json", "r") ;

    // 파일 크기 구하기
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // 파일 크기 + NULL 공간만큼 메모리를 할당하고 0으로 초기화
    buffer = malloc(size + 1);
    memset(buffer, 0, size + 1);
    fread(buffer, size, 1, fp) ;

    while (buffer[i] != '{') {
        curr_pos ++ ;
    }

    int end = readJSON(curr_pos) ;


}


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
    int position = startIndex;
    while(1){
        if(buffer[position] >= '0' && buffer[position] <= '9')
            position++;
        else
            break;
    }
    return position-1;

}
