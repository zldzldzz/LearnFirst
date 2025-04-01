#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

// op reg16 num [IP=2]

#define MAX_LENGTH 100
#define INITIAL_SIZE 10 // �ʱ� �ɺ� �迭 ũ��

typedef struct
{
    char *symbol;
    int address;
} Symbol;

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

void add_symbol(Symbol **sym, int *size, int *count, const char *new_symbol, int address)
{
    if (*count >= *size)
    {
        *size *= 2;
        *sym = realloc(*sym, *size * sizeof(Symbol));
        if (*sym == NULL)
        {
            perror("�޸� �Ҵ� ����");
            exit(1);
        }
    }
    (*sym)[*count].symbol = strdup(new_symbol);
    (*sym)[*count].address = address;
    if ((*sym)[*count].symbol == NULL)
    {
        perror("�޸� �Ҵ� ����");
        exit(1);
    }
    (*count)++;
}

void free_symbols(Symbol *sym, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(sym[i].symbol);
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

int find_symbol_address(Symbol *sym, int count, const char *word)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(sym[i].symbol, word) == 0)
        {
            return sym[i].address;
        }
    }
    return -1;
}

int main()
{
    FILE *input_file = fopen("symbol.txt", "r");
    FILE *output_file = fopen("out.txt", "w");
    if (input_file == NULL || output_file == NULL)
    {
        printf("������ �� �� �����ϴ�.\n");
        return 1;
    }

    char line[MAX_LENGTH];
    char output_word[MAX_LENGTH];
    Symbol *sym = malloc(INITIAL_SIZE * sizeof(Symbol));
    if (sym == NULL)
    {
        perror("�޸� �Ҵ� ����");
        return 1;
    }

    int size = INITIAL_SIZE;
    int count = 0;
    int ip = 0; // IP �ʱ�ȭ

    while (fgets(line, sizeof(line), input_file))
    {
        if (strlen(line) == 1 && line[0] == '\n')
        {
            continue; // �� ���� IP ���� ���� ����
        }

        char *token = strtok(line, " \t\n");
        if (token == NULL || token[0] == ';')
        {
            continue; // ��� �ִ� �� �Ǵ� �ּ��� ó������ ����
        }

        bool is_pop_line = false;
        bool is_sym_line = false;

        char processed_line[MAX_LENGTH] = "";
        while (token != NULL)
        {
            memset(output_word, 0, sizeof(output_word));
            clean_word(token);

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
                int addr = find_symbol_address(sym, count, token);
                if (addr == -1)
                { // ���ο� �ɺ�
                    add_symbol(&sym, &size, &count, token, ip);
                    addr = ip;
                }
                char addr_str[MAX_LENGTH];
                sprintf(addr_str, "[%d] ", addr);
                strcat(processed_line, addr_str);
            }
            token = strtok(NULL, " \t\n");
        }

        if (is_pop_line)
        {
            // POP�� ���Ե� ��� IP ��ȭ ����
        }
        else if (is_sym_line)
        {
            ip += 3; // SYM�� ���Ե� ��� IP 3�� ����
        }
        else
        {
            ip += 2; // ������ ��� IP 2�� ����
        }

        fprintf(output_file, "%s[IP=%d]\n", processed_line, ip);
    }

    fclose(input_file);
    fclose(output_file);

    printf("��ȯ ����� out.txt�� ����Ǿ����ϴ�.\n");

    for (int i = 0; i < count; i++)
    {
        printf("�ɺ� %d: %s -> �ּ�: %d\n", i + 1, sym[i].symbol, sym[i].address);
    }

    free_symbols(sym, count);
    return 0;
}
