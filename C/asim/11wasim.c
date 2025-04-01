#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#define MAX_WORDS 1000
#define MAX_LINE_LENGTH 100 // �� ���� �ִ� ����
// 57      SUB AX 257
int main()
{
    FILE *file;
    char lines[MAX_WORDS][MAX_LINE_LENGTH]; // �� ���� �����ϴ� 2���� �迭
    int lineCount = 0;
    char words[MAX_WORDS][MAX_LINE_LENGTH]; // �� �ܾ ������ 2���� �迭
    int wordCount = 1;
    char indexName[MAX_WORDS][MAX_LINE_LENGTH]; // ���̺��̳� ������ �����ϴ� �迭
    char index[MAX_WORDS][MAX_LINE_LENGTH];     // ���̺��̳� ���� ��ġ ����
    int indexCount = 0;

    file = fopen("symbol.txt", "r");

    if (file == NULL)
    {
        printf("������ ���� ���߽��ϴ�.\n");
        return 1;
    }

    // ���Ͽ��� �� �پ� �о lines �迭�� ����
    while (fgets(lines[lineCount], MAX_LINE_LENGTH, file) != NULL && lineCount < MAX_WORDS)
    {
        // ���� ���� �������� �ʰ� �״�� ����
        char line[MAX_LINE_LENGTH];
        strcpy(line, lines[lineCount]);

        // �� ���� ù ��° �ܾ ����
        char *token = strtok(line, " \t\n");
        if (token != NULL)
        {
            // ù �ܾ ������ ��츸 ���� (���̺� �Ǵ� ����)
            if (strchr(token, ':') != NULL || strstr(token, "DATA") == token)
            {
                // ���̺����� ':'�� �����ϰ� indexName�� index�� ����
                token[strcspn(token, ":")] = '\0';
                strcpy(indexName[indexCount], token);
                sprintf(index[indexCount], "%d", wordCount); // ��ġ ����
                indexCount++;
            }
        }

        // words �迭�� ���� �ܾ �߰��� ����
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

    // Assembly code ���� �������� ���
    printf("Assembly code:\n\n");
    for (int i = 0; i < lineCount; i++)
    {
        printf("%s", lines[i]); // ���� �� �� ���� ���Ե� ���·� ���
    }

    // Table index ���
    printf("\nTable index:\n\n");
    for (int i = 0; i < indexCount; i++)
    {
        printf("%s : %s\n", indexName[i], index[i]);
    }

    // Symnolic code ���
    printf("\nSymnolic code:\n\n");
    for (int i = 0; i < lineCount; i++)
    {
        // �� ���� �ٽ� ó���Ͽ� ���̺��� �ε����� ġȯ
        char line[MAX_LINE_LENGTH];
        strcpy(line, lines[i]);

        // �ܾ�� ó��
        char *token = strtok(line, " :,\n");
        while (token != NULL)
        {
            int replaced = 0;
            for (int j = 0; j < indexCount; j++)
            {
                // ���̺��� �߰ߵǸ� ġȯ
                if (strcmp(token, indexName[j]) == 0)
                {
                    printf("%s ", index[j]);
                    replaced = 1;
                    break;
                }
            }

            // ġȯ���� ���� �ܾ�� �״�� ���
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