#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define true 1
#define false 0

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
    int recommend ;
} tok_t ;

typedef struct {
    int player ;
    int price ;
    int age ;
    int genre ;
} tables ;

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
    token_array = (tok_t*)malloc(sizeof(tok_t)*400) ;
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
        printf(" 2. Game Recommendation System.\n") ;
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
            int index = 0 ;
            tables table[20] ;
            printf("Please enter the number of people.\n> ") ;
            int team_member ;
            scanf("%d", &team_member) ;

            printf("Please enter the maximum price you want.\n> ") ;
            int price ;
            scanf("%d", &price) ;

            printf("Please enter the minimum age.\n> ") ;
            int age ;
            scanf("%d", &age) ;

            printf("Please enter favorite genre.\n> ") ;
            char genre[16] ;
            getchar() ;
            gets(genre) ;
            int value ;

            /* finding people */
            for (int i=0 ; i<token_index ; i++) {
                if (token_array[i].parent!=-1)
                    continue ;
                int strsize = token_array[i].end-token_array[i].start-1 ;
                char *content = (char*)malloc((strsize) * sizeof(char)) ;
                memcpy(content, &buffer[token_array[i].start+1], strsize) ;
                content[strsize] = 0x0 ;
                if (strcmp("players", content)==0) {
                    value = i ;   
                    for (int j=0 ; j<token_index ; j++) {
                        if (value==token_array[j].parent) {
                            int strsize_ = token_array[j].end-token_array[j].start+1 ;
                            char *content_ = (char*)malloc((strsize_) * sizeof(char)) ;
                            memcpy(content_, &buffer[token_array[j].start], strsize_) ;
                            content_[strsize_] = 0x0 ;

                            //printf("%s\n", content_) ;
                            int dash = 0 ;
                            for (int k=0 ; k<strlen(content_) ; k++) {
                                if (content_[k]=='-') {
                                    dash = k ;
                                    break ;
                                }
                            }
                            char temp[8] ;
                            memcpy(temp, &content_[1], (dash)*sizeof(char)) ;
                            //printf("%s   ", temp) ;
                            int start_index = atoi(temp) ;
                            memcpy(temp, &content_[dash+1], sizeof(char)*(strlen(content_)-dash)) ;
                            int end_index = atoi(temp) ;
                            //printf("%d %d %d\n", start_index, team_member, end_index) ;
                            if ((start_index<=team_member) && (team_member<=end_index)) {
                                table[index++].player = true ;
                            } else {
                                table[index++].player = false ;
                            }
                            free(content_) ;
                            break ;
                        }
                    } 
                }
                free(content) ;
            }
            index = 0 ;
            /* finding price */
            for (int i=0 ; i<token_index ; i++) {
                if (token_array[i].parent!=-1)
                    continue ;
                int strsize = token_array[i].end-token_array[i].start-1 ;
                char *content = (char*)malloc((strsize) * sizeof(char)) ;
                memcpy(content, &buffer[token_array[i].start+1], strsize) ;
                content[strsize] = 0x0 ;
                if (strcmp("rental fee", content)==0) {
                    value = i ;   
                    for (int j=0 ; j<token_index ; j++) {
                        if (value==token_array[j].parent) {
                            int strsize_ = token_array[j].end-token_array[j].start+1 ;
                            char *content_ = (char*)malloc((strsize_) * sizeof(char)) ;
                            memcpy(content_, &buffer[token_array[j].start], strsize_) ;
                            content_[strsize_] = 0x0 ;
                            int rental = atoi(content_) ;
                            //printf("retal %d, price %d\n", rental, price) ;
                            if (rental<=price) 
                                table[index++].price = true ;
                            else 
                                table[index++].price = false ;
                            free(content_) ;
                            break ;
                        }
                    } 
                }
                free(content) ;
            }

            /*finding age*/
            index = 0 ;
            for (int i=0 ; i<token_index ; i++) {
                if (token_array[i].parent!=-1)
                    continue ;
                int strsize = token_array[i].end-token_array[i].start-1 ;
                char *content = (char*)malloc((strsize) * sizeof(char)) ;
                memcpy(content, &buffer[token_array[i].start+1], strsize) ;
                content[strsize] = 0x0 ;
                if (strcmp("age", content)==0) {
                    value = i ;   
                    for (int j=0 ; j<token_index ; j++) {
                        if (value==token_array[j].parent) {
                            int strsize_ = token_array[j].end-token_array[j].start+1 ;
                            char *content_ = (char*)malloc((strsize_) * sizeof(char)) ;
                            memcpy(content_, &buffer[token_array[j].start], strsize_) ;
                            content_[strsize_] = 0x0 ;
                            int input_age = atoi(content_) ;
                            if (age>=input_age) 
                                table[index++].age = true ;
                            else 
                                table[index++].age = false ;
                            free(content_) ;
                            break ;
                        }
                    } 
                }
                free(content) ;
            }

            /*finding genre*/
            index = 0 ;
            for (int i=0 ; i<token_index ; i++) {
                if (token_array[i].parent!=-1)
                    continue ;
                int strsize = token_array[i].end-token_array[i].start-1 ;
                char *content = (char*)malloc((strsize) * sizeof(char)) ;
                memcpy(content, &buffer[token_array[i].start+1], strsize) ;
                content[strsize] = 0x0 ;
                if (strcmp("genre", content)==0) {
                    value = i ;   
                    for (int j=0 ; j<token_index ; j++) {
                        if (value==token_array[j].parent) {
                            int strsize_ = token_array[j].end-token_array[j].start+1 ;
                            char *content_ = (char*)malloc((strsize_) * sizeof(char)) ;
                            memcpy(content_, &buffer[token_array[j].start], strsize_) ;
                            content_[strsize_] = 0x0 ;

                            char *str = strstr(content_, genre) ;
                            //printf("index %d, %s, %s, %p\n", index+1, content_, genre, str) ;
                            if (str == 0x0) {
                                table[index].genre = false ;
                                //printf("index %d, %s, %s, %p\n", index+1, content_, genre, str) ;
                            } else {
                                table[index].genre = true ;
                            } 
                            index += 1 ;
                            free(content_) ;
                            break ;
                        }
                    } 
                }
                free(content) ;
            }
            system("@cls||clear");
            printf("========= Recommendation =========\n") ;
            int flag = 0 ;
            for (int k=0 ; k<index ; k++) {
                char compare[16] ;
                sprintf(compare, "board game %d", k+1) ;
                //printf("%d %d %d %d\n",table[k].player, table[k].price, table[k].age, table[k].genre) ;
                if (table[k].age==true && table[k].genre==true && table[k].player==true && table[k].price==true) {
                    flag = 1 ;
                    for (int i=0 ; i<token_index ; i++) {
                        if (token_array[i].parent!=-1)
                            continue ;
                        int strsize = token_array[i].end-token_array[i].start-1 ;
                        char *content = (char*)malloc((strsize) * sizeof(char)) ;
                        memcpy(content, &buffer[token_array[i].start+1], strsize) ;
                        content[strsize] = 0x0 ;
                        //printf("%d | %s\n", k, compare) ;
                        if (strcmp(compare, content)==0) {
                            value = i ;   
                            for (int j=0 ; j<token_index ; j++) {
                                if (value==token_array[j].parent) {
                                    int strsize_ = token_array[j].end-token_array[j].start+1 ;
                                    char *content_ = (char*)malloc((strsize_) * sizeof(char)) ;
                                    memcpy(content_, &buffer[token_array[j].start], strsize_) ;
                                    content_[strsize_] = 0x0 ;
                                    printf("%s\n", content_) ;
                                    free(content_) ;
                                    break ;
                                }
                            } 
                        }
                        free(content) ;
                    }
                }
            }

            if (flag == 0)
                printf("There is no matching game.\n") ;
            getchar() ;
        } else {
            break ;
        }
    }
    free(buffer) ;
    free(token_array) ;
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
    int end_val = readValue(pos+1, token_index-1) ;
    // 전체 structure에 읽은 값 차례대로 저장
    return end_val ;
}

int readJSON(int start, int parent) {
    int position = readPair(start);
    int count = 0 ;
    position += 1 ;
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
    token_array[token_index].recommend = false ;
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
            token_array[token_index].recommend = false ;
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
    token_array[token_index].recommend = false ;
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
        token_array[token_index].recommend = false ;
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
    token_array[token_index].recommend = false ;
    token_index += 1 ;
    return return_pos ;
}

int readNumber(int position, int parent) {
    while(!isdigit(buffer[position]) && buffer[position]!='-') {
        position += 1 ;
    }
    int startIndex = position;
    int endIndex = startIndex ;

    while((isdigit(buffer[position]) || buffer[position]=='.' || buffer[position]=='e' || buffer[position]=='E')) {
        position += 1 ;
    }
    endIndex = position-1 ;

    token_array[token_index].type = PRIMITIVE;
    token_array[token_index].start = startIndex;
    token_array[token_index].end = endIndex;
    token_array[token_index].parent = parent ;
    token_array[token_index].size = 0 ;
    token_array[token_index].recommend = false ;
    token_index++;
    return position-1;
}
