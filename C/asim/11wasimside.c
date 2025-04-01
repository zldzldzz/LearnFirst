#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

// MOV [op ] AX [reg16 ] 257 [num ] 2

#define MAX_LENGTH 100
#define INITIAL_SIZE 10 // 초기 심볼 배열 크기

const char *reg16[] = {
    "AX",
    "BX",
    "CX",
    "DX",
    "SI",
    "DI",
    "SP",
    "BP",
    "IP",
    "FLAGS",
    "CS",
    "DS",
    "SS",
    "ES",
    "FS",
    "GS",
};

const char *reg8[] = {
    "AL",
    "AH",
    "BL",
    "BH",
    "CL",
    "CH",
    "DL",
    "DH",
};

const char *optbl[] = {
    "MOV", "POP", "PUSH", "ADD", "SUB", "MUL", "DIV", "INC", "DEC",
    "AND", "OR", "XOR", "NOT", "CMP", "INT",
    "JA", "JAE", "JB", "JBE", "JC", "JE", "JNE", "JZ", "JZE",
    "CALL", "RET"};

const char *pop[] = {
    "DW", "DB", "SEGMENT", "ASSUME", "END"};

void add_symbol(char ***sym, int *size, int *count, const char *new_symbol)
{
    if (*count >= *size)
    {
        *size *= 2;
        *sym = realloc(*sym, *size * sizeof(char *));
        if (*sym == NULL)
        {
            perror("메모리 할당 실패");
            exit(1);
        }
    }
    (*sym)[*count] = strdup(new_symbol);
    if ((*sym)[*count] == NULL)
    {
        perror("메모리 할당 실패");
        exit(1);
    }
    (*count)++;
}

void free_symbols(char **sym, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(sym[i]);
    }
    free(sym);
}

void pop_back(char *word)
{
    size_t len = strlen(word);
    if (len > 0)
    {
        word[len - 1] = '\0';
    }
}

void pop_front(char *word)
{
    size_t len = strlen(word);
    if (len > 0)
    {
        memmove(word, word + 1, len);
    }
}

void clean_word(char *word)
{
    size_t len = strlen(word);
    if (len > 0 && (word[len - 1] == ',' || word[len - 1] == ':'))
    {
        pop_back(word);
    }
    if (len > 1 && word[0] == '[' && word[strlen(word) - 1] == ']')
    {
        pop_back(word);
        pop_front(word);
    }
}

bool is_integer(const char *word)
{
    for (int i = 0; word[i] != '\0'; i++)
    {
        if (!isdigit(word[i]))
        {
            return false;
        }
    }
    return true;
}

bool find_and_replace(const char *word, const char *arr[], int size, const char *arr_name, char *output)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(word, arr[i]) == 0)
        {
            sprintf(output, "%s", arr_name);
            return true;
        }
    }
    return false;
}

int main()
{
    FILE *input_file = fopen("symbol.txt", "r");
    FILE *output_file = fopen("out.txt", "w");
    if (input_file == NULL || output_file == NULL)
    {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    char line[MAX_LENGTH];
    char output_word[MAX_LENGTH];
    char **sym = malloc(INITIAL_SIZE * sizeof(char *));
    if (sym == NULL)
    {
        perror("메모리 할당 실패");
        return 1;
    }

    int size = INITIAL_SIZE;
    int count = 0;
    int ip = 0; // IP 초기화

    while (fgets(line, sizeof(line), input_file))
    {
        // 줄바꿈만 있는 줄 체크
        if (strlen(line) == 1 && line[0] == '\n')
        {
            continue; // 빈 줄은 IP 증가 없이 무시
        }

        char *token = strtok(line, " \t\n");
        if (token == NULL || token == ";")
        {
            continue; // 비어 있는 줄은 처리하지 않음
        }

        bool is_pop_line = false;
        bool is_sym_line = false;

        char processed_line[MAX_LENGTH] = ""; // 처리된 라인의 결과를 저장
        while (token != NULL)
        {
            memset(output_word, 0, sizeof(output_word)); // output_word 초기화
            clean_word(token);                           // 토큰 정리 (불필요한 문자 제거)

            // 토큰 처리
            if (is_integer(token))
            {
                strcat(processed_line, "num ");
            }
            else if (find_and_replace(token, reg16, sizeof(reg16) / sizeof(reg16[0]), "reg16", output_word) ||
                     find_and_replace(token, reg8, sizeof(reg8) / sizeof(reg8[0]), "reg8", output_word) ||
                     find_and_replace(token, optbl, sizeof(optbl) / sizeof(optbl[0]), "op", output_word))
            {
                strcat(processed_line, output_word);
                strcat(processed_line, " ");
            }
            else if (find_and_replace(token, pop, sizeof(pop) / sizeof(pop[0]), "pop", output_word))
            {
                is_pop_line = true;
                strcat(processed_line, "pop ");
            }
            else
            {
                is_sym_line = true;
                strcat(processed_line, "sym ");
            }

            // 출력 후 processed_line 초기화
            fprintf(output_file, "%s [%s] ", token, processed_line);
            processed_line[0] = '\0'; // 처리된 라인 초기화

            token = strtok(NULL, " \t\n");
        }

        // IP 증분 규칙
        if (is_pop_line)
        {
            // POP이 포함된 경우 IP 변화 없음
        }
        else if (is_sym_line)
        {
            ip += 3; // SYM이 포함된 경우 IP 3씩 증가
        }
        else
        {
            ip += 2; // 나머지 경우 IP 2씩 증가
        }

        fprintf(output_file, "%d\n", ip);
        ip = 0;
    }

    fclose(input_file);
    fclose(output_file);

    printf("변환 결과가 out.txt에 저장되었습니다.\n");

    for (int i = 0; i < count; i++)
    {
        printf("심볼 %d: %s\n", i + 1, sym[i]);
    }

    free_symbols(sym, count);
    return 0;
}
