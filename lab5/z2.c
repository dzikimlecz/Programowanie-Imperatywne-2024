#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID_LEN 64
#define MAX_IDS 1024

#define FIRST_NUMBER (int)'0'
#define LAST_NUMBER (int)'9'
#define FIRST_UPPERCASE (int)'A'
#define LAST_UPPERCASE (int)'Z'
#define FIRST_LOWERCASE (int)'a'
#define LAST_LOWERCASE (int)'z'

#define TEST 0  // 1 - dla testowania,  0 - dla automatycznej oceny

char *keywords[] = {
    "auto", "break", "case", "char",
    "const", "continue", "default", "do",
    "double", "else", "enum", "extern",
    "float", "for", "goto", "if",
    "int", "long", "register", "return",
    "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"
};

int isKeyword(const char* s) {
    int start = 0;
    int end = 31;
    while (start < end) {
        int test = start + ceil((end - start) / 2.0);
        if (strcmp(keywords[test], s) > 0) {
            end = test - 1;
        } else {
            start = test;
        }
    }
    return strcmp(keywords[start], s) == 0;
}

void seekBlockEnd(FILE *stream) {
    int c1 = fgetc(stream);
    int c2 = fgetc(stream);
    while (c2 != EOF && (c1 != '*' || c2 != '/')) {
        c1 = c2;
        c2 = fgetc(stream);
    }
}

void seekLineEnd(FILE *stream) {
    int c;
    do {
      c = fgetc(stream);
    } while (c != EOF && c != '\n');
}

void seekStringEnd(FILE *stream, char delimiter) {
    int c;
    do {
      c = fgetc(stream);
    } while (c != EOF && c != delimiter);
}

int isDig(int c) {
    return (FIRST_NUMBER <= c && c <= LAST_NUMBER);
}

int tokenSign(int c) {
    return (FIRST_LOWERCASE <= c && c <= LAST_LOWERCASE)
        || (FIRST_UPPERCASE <= c && c <= LAST_UPPERCASE) || (c == '_') || isDig(c);
}

int nextToken(char buff[MAX_ID_LEN + 1], FILE* stream) {
    int _c = fgetc(stream);
    int bufflen = 0;
    while (tokenSign(_c) && bufflen <= MAX_ID_LEN) {
        buff[bufflen++] = _c;
        _c = fgetc(stream);
    }
    switch (_c) {
    case '"':
    case '\'':
        seekStringEnd(stream, _c);
        break;
    case '/':
        switch (fgetc(stream)) {
        case '/':
            seekLineEnd(stream);
            break;
        case '*':
            seekBlockEnd(stream);
            break;
        default:
            fseek(stream, -1, SEEK_CUR); // move one char back
            break;
        }
        break;
    default:
        break;
    }
    return _c != EOF ? bufflen : -1;
}

int isTokenValid(const char* buff, int bufflen) {
    if (bufflen < 1 || !tokenSign(buff[0]) || isDig(buff[0])) {
        return 0;
    }
    int i;
    for (i = 1; i < bufflen && tokenSign(buff[i]); ++i);
    return i == bufflen;
}

int isTokenNew(char tokens[MAX_IDS][MAX_ID_LEN + 1], int newIndex) {
    if (newIndex >= MAX_IDS) {
        return 0;
    }
    int notFound = 1;
    for (int i = 0; notFound && i < newIndex; ++i) {
        notFound = strcmp(tokens[i], tokens[newIndex]) != 0;
    }
    return notFound && !isKeyword(tokens[newIndex]);
}

int find_idents(FILE *stream) {
    char tokens[MAX_IDS][MAX_ID_LEN + 1] = {0};
    int idCount = 0;
    int tokenLen;
    do {
        tokenLen = nextToken(tokens[idCount], stream);
        if (isTokenValid(tokens[idCount], tokenLen) && isTokenNew(tokens, idCount)) {
            ++idCount;
        } else {
            memset(tokens[idCount], 0, (MAX_ID_LEN + 1)*sizeof(char));
        }
    } while (tokenLen != -1);
    return idCount;
}

int main(void) {
    char file_name[40];
    FILE *stream;
    if(TEST) stream = stdin;
    else {
        scanf("%s",file_name);
        stream = fopen(file_name,"r");
        if(stream == NULL) {
            printf("fopen failed\n");
            return -1;
        }
    }
    printf("%d\n", find_idents(stream));
    return 0;
}
