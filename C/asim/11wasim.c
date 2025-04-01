#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#define MAX_WORDS 1000
#define MAX_LINE_LENGTH 100 // 한 줄의 최대 길이
// 57      SUB AX 257
int main()
{
    FILE *file;
    char lines[MAX_WORDS][MAX_LINE_LENGTH]; // 각 줄을 저장하는 2차원 배열
    int lineCount = 0;
    char words[MAX_WORDS][MAX_LINE_LENGTH]; // 각 단어를 저장할 2차원 배열
    int wordCount = 1;
    char indexName[MAX_WORDS][MAX_LINE_LENGTH]; // 레이블이나 변수를 저장하는 배열
    char index[MAX_WORDS][MAX_LINE_LENGTH];     // 레이블이나 변수 위치 저장
    int indexCount = 0;

    file = fopen("symbol.txt", "r");

    if (file == NULL)
    {
        printf("파일을 읽지 못했습니다.\n");
        return 1;
    }

    // 파일에서 한 줄씩 읽어서 lines 배열에 저장
    while (fgets(lines[lineCount], MAX_LINE_LENGTH, file) != NULL && lineCount < MAX_WORDS)
    {
        // 개행 문자 제거하지 않고 그대로 저장
        char line[MAX_LINE_LENGTH];
        strcpy(line, lines[lineCount]);

        // 각 줄의 첫 번째 단어를 추출
        char *token = strtok(line, " \t\n");
        if (token != NULL)
        {
            // 첫 단어가 존재할 경우만 검출 (레이블 또는 변수)
            if (strchr(token, ':') != NULL || strstr(token, "DATA") == token)
            {
                // 레이블에서는 ':'를 제거하고 indexName과 index에 저장
                token[strcspn(token, ":")] = '\0';
                strcpy(indexName[indexCount], token);
                sprintf(index[indexCount], "%d", wordCount); // 위치 저장
                indexCount++;
            }
        }

        // words 배열에 줄의 단어를 추가로 저장
        while (token != NULL && wordCount < MAX_WORDS)
        {
            strcpy(words[wordCount], token);
            for (int i = 0; i < strlen(token); i++)
            {
                wordCount++;
            }
            token = strtok(NULL, " \t\n");
        }

        lineCount++;
    }
    fclose(file);

    // Assembly code 원본 형식으로 출력
    printf("Assembly code:\n\n");
    for (int i = 0; i < lineCount; i++)
    {
        printf("%s", lines[i]); // 원래 줄 끝 개행 포함된 상태로 출력
    }

    // Table index 출력
    printf("\nTable index:\n\n");
    for (int i = 0; i < indexCount; i++)
    {
        printf("%s : %s\n", indexName[i], index[i]);
    }

    // Symnolic code 출력
    printf("\nSymnolic code:\n\n");
    for (int i = 0; i < lineCount; i++)
    {
        // 각 줄을 다시 처리하여 레이블을 인덱스로 치환
        char line[MAX_LINE_LENGTH];
        strcpy(line, lines[i]);

        // 단어별로 처리
        char *token = strtok(line, " :,\n");
        while (token != NULL)
        {
            int replaced = 0;
            for (int j = 0; j < indexCount; j++)
            {
                // 레이블이 발견되면 치환
                if (strcmp(token, indexName[j]) == 0)
                {
                    printf("%s ", index[j]);
                    replaced = 1;
                    break;
                }
            }

            // 치환되지 않은 단어는 그대로 출력
            if (!replaced)
            {
                printf("%s ", token);
            }
            token = strtok(NULL, " :,\n");
        }
        printf("\n");
    }

    return 0;
}