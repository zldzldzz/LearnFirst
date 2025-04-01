#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#define MAX_WORDS 1000
#define MAX_LINE_LENGTH 100 // �� ���� �ִ� ����

// ���� ���� �Լ�
void trim(char *str)
{
    int start = 0, end = strlen(str) - 1;

    // �պκ� ���� ����
    while (isspace((unsigned char)str[start]))
    {
        start++;
    }

    // �޺κ� ���� ����
    while (end >= start && isspace((unsigned char)str[end]))
    {
        end--;
    }

    // ���ο� ���ڿ��� ��ü
    for (int i = 0; i <= end - start; i++)
    {
        str[i] = str[start + i];
    }
    str[end - start + 1] = '\0'; // null terminator �߰�
}

int main()
{
    FILE *file;
    char lines[MAX_WORDS][MAX_LINE_LENGTH]; // �� ���� �����ϴ� 2���� �迭
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
        printf("������ ���� ���߽��ϴ�.\n");
        return 1;
    }

    // ���Ͽ��� �� �پ� �о lines �迭�� ����
    while (fgets(lines[lineCount], MAX_LINE_LENGTH, file) != NULL && lineCount < MAX_WORDS)
    {
        // ���� ���� ����
        size_t len = strlen(lines[lineCount]);
        if (len > 0 && lines[lineCount][len - 1] == '\n')
        {
            lines[lineCount][len - 1] = '\0'; // ���� ���ڸ� null ���ڷ� ��ü
        }

        // ���� ���� �� ���� ������� Ȯ��
        trim(lines[lineCount]); // �յ� ���� ����
        if (strlen(lines[lineCount]) == 0)
        {
            continue; // �� ���� ����
        }

        lineCount++;
    }

    fclose(file);

    // �о�� �ڵ�� ��ȯ�� �ڵ带 �Բ� ���
    for (int i = 0; i < lineCount; i++)
    {
        char originalLine[MAX_LINE_LENGTH];
        strcpy(originalLine, lines[i]); // ���� ���� ����
        printf("%s\t", originalLine);   // ���� �� ���

        // ���� �� Ȯ��
        if (strlen(lines[i]) == 0 || lines[i][0] == '\n')
        {
            continue; // ���� ���� �Ѿ
        }

        // ��ȯ�� �ڵ� ���
        char *token = strtok(lines[i], " \t,"); // ����, ��, �޸��� ����
        while (token != NULL)
        {
            trim(token); // ��ū���� �յ� ���� ����
            int found = 0;

            // ���̺�(L1:, L2:, ...) ó��
            int len = strlen(token);
            if (token[len - 1] == ':')
            {
                printf("sym ");
                found = 1;
            }

            // [�� ] ���� �� �߰�
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

            // op �迭�� ��
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

            // reg8 �迭�� ��
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

            // reg16 �迭�� ��
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

            // pop �迭�� ��
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

            // �������� Ȯ��
            if (!found)
            {
                char *endptr;
                strtol(cleanToken, &endptr, 10); // 10������ ��ȯ �õ�
                if (*endptr == '\0')
                { // ��ȯ�� �����Ͽ� endptr�� ���ڿ� ���� �����ߴ��� Ȯ��
                    printf("num ");
                    found = 1;
                }
            }

            // [BX+SI] ������ ���� �������� ó��
            if (!found && strchr(cleanToken, '+'))
            {
                char *innerToken = strtok(cleanToken, "+");
                while (innerToken != NULL)
                {
                    trim(innerToken); // ���� ��ū ���� ����
                    found = 0;        // ���� �������� ó��
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
                found = 1; // ���� �������� ó���� �������Ƿ� true ����
            }

            // ���ǿ� ���� �ʴ� ��� sym ���
            if (!found)
            {
                printf("sym ");
            }

            // ���� �ܾ�� �̵�
            token = strtok(NULL, " \t,");
        }
        printf("\n"); // ��ȯ�� �ڵ� ��� �� �ٹٲ�
    }

    return 0;
}