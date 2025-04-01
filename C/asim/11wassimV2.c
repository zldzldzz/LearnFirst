#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#define MAX_WORDS 1000
#define MAX_LINE_LENGTH 100 // 한 줄의 최대 길이

// 공백 제거 함수
void trim(char *str)
{
    int start = 0, end = strlen(str) - 1;

    // 앞부분 공백 제거
    while (isspace((unsigned char)str[start]))
    {
        start++;
    }

    // 뒷부분 공백 제거
    while (end >= start && isspace((unsigned char)str[end]))
    {
        end--;
    }

    // 새로운 문자열로 교체
    for (int i = 0; i <= end - start; i++)
    {
        str[i] = str[start + i];
    }
    str[end - start + 1] = '\0'; // null terminator 추가
}

int main()
{
    FILE *file;
    char lines[MAX_WORDS][MAX_LINE_LENGTH]; // 각 줄을 저장하는 2차원 배열
    int lineCount = 0;

    char op[17][4] = {"MOV", "ADD", "SUB", "DIV", "INC", "DEC", "CMP",
                      "INT", "JA", "JAE", "JB", "JBE", "JC", "JE", "JNE",
                      "JZ", "JZE"};
    char reg8[8][3] = {"AL", "AH", "BL", "BH", "CL", "CH", "DL", "DH"};
    char reg16[16][5] = {"AX", "BX", "CX", "DX", "SI", "DI", "SP", "BP",
                         "IP", "FLAGS", "CS", "DS", "SS", "ES", "FS", "GS"};
    char pop[5][10] = {"DB", "DW", "SEGMENT", "ASSUME"};

    file = fopen("symbol.txt", "r");
    if (file == NULL)
    {
        printf("파일을 읽지 못했습니다.\n");
        return 1;
    }

    // 파일에서 한 줄씩 읽어서 lines 배열에 저장
    while (fgets(lines[lineCount], MAX_LINE_LENGTH, file) != NULL && lineCount < MAX_WORDS)
    {
        // 개행 문자 제거
        size_t len = strlen(lines[lineCount]);
        if (len > 0 && lines[lineCount][len - 1] == '\n')
        {
            lines[lineCount][len - 1] = '\0'; // 개행 문자를 null 문자로 대체
        }

        // 공백 제거 및 줄이 비었는지 확인
        trim(lines[lineCount]); // 앞뒤 공백 제거
        if (strlen(lines[lineCount]) == 0)
        {
            continue; // 빈 줄은 무시
        }

        lineCount++;
    }

    fclose(file);

    // 읽어온 코드와 변환된 코드를 함께 출력
    for (int i = 0; i < lineCount; i++)
    {
        char originalLine[MAX_LINE_LENGTH];
        strcpy(originalLine, lines[i]); // 원본 라인 보존
        printf("%s\t", originalLine);   // 원래 줄 출력

        // 공백 줄 확인
        if (strlen(lines[i]) == 0 || lines[i][0] == '\n')
        {
            continue; // 공백 줄은 넘어감
        }

        // 변환된 코드 출력
        char *token = strtok(lines[i], " \t,"); // 공백, 탭, 콤마로 구분
        while (token != NULL)
        {
            trim(token); // 토큰에서 앞뒤 공백 제거
            int found = 0;

            // 레이블(L1:, L2:, ...) 처리
            int len = strlen(token);
            if (token[len - 1] == ':')
            {
                printf("sym ");
                found = 1;
            }

            // [와 ] 제거 후 추가
            char cleanToken[MAX_LINE_LENGTH];
            int idx = 0;
            for (int j = 0; token[j] != '\0'; j++)
            {
                if (token[j] != '[' && token[j] != ']')
                {
                    cleanToken[idx++] = token[j];
                }
            }
            cleanToken[idx] = '\0';

            // op 배열과 비교
            if (!found)
            {
                for (int j = 0; j < 17; j++)
                {
                    if (strcmp(cleanToken, op[j]) == 0)
                    {
                        printf("op ");
                        found = 1;
                        break;
                    }
                }
            }

            // reg8 배열과 비교
            if (!found)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (strcmp(cleanToken, reg8[j]) == 0)
                    {
                        printf("reg8 ");
                        found = 1;
                        break;
                    }
                }
            }

            // reg16 배열과 비교
            if (!found)
            {
                for (int j = 0; j < 16; j++)
                {
                    if (strcmp(cleanToken, reg16[j]) == 0)
                    {
                        printf("reg16 ");
                        found = 1;
                        break;
                    }
                }
            }

            // pop 배열과 비교
            if (!found)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (strcmp(cleanToken, pop[j]) == 0)
                    {
                        printf("pop ");
                        found = 1;
                        break;
                    }
                }
            }

            // 숫자인지 확인
            if (!found)
            {
                char *endptr;
                strtol(cleanToken, &endptr, 10); // 10진수로 변환 시도
                if (*endptr == '\0')
                { // 변환이 성공하여 endptr이 문자열 끝에 도달했는지 확인
                    printf("num ");
                    found = 1;
                }
            }

            // [BX+SI] 형태의 다중 레지스터 처리
            if (!found && strchr(cleanToken, '+'))
            {
                char *innerToken = strtok(cleanToken, "+");
                while (innerToken != NULL)
                {
                    trim(innerToken); // 내부 토큰 공백 제거
                    found = 0;        // 내부 레지스터 처리
                    for (int j = 0; j < 16; j++)
                    {
                        if (strcmp(innerToken, reg16[j]) == 0)
                        {
                            printf("reg16 ");
                            found = 1;
                            break;
                        }
                    }
                    if (!found)
                    {
                        printf("sym ");
                    }
                    innerToken = strtok(NULL, "+");
                }
                found = 1; // 다중 레지스터 처리를 마쳤으므로 true 설정
            }

            // 조건에 맞지 않는 경우 sym 출력
            if (!found)
            {
                printf("sym ");
            }

            // 다음 단어로 이동
            token = strtok(NULL, " \t,");
        }
        printf("\n"); // 변환된 코드 출력 후 줄바꿈
    }

    return 0;
}