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

        char processed_line[MAX_LENGTH] = "";
        while (token != NULL)
        {
            memset(output_word, 0, sizeof(output_word));
            clean_word(token);

            if (is_integer(token))
            {
                append_to_line(processed_line, "num");
            }
            else if (find_and_replace(token, reg16, sizeof(reg16) / sizeof(reg16[0]), "reg16", output_word) ||
                     find_and_replace(token, reg8, sizeof(reg8) / sizeof(reg8[0]), "reg8", output_word) ||
                     find_and_replace(token, optbl, sizeof(optbl) / sizeof(optbl[0]), "op", output_word))
            {
                append_to_line(processed_line, output_word);
            }
            else if (find_and_replace(token, pop, sizeof(pop) / sizeof(pop[0]), "pop", output_word))
            {
                is_pop_line = true;
                append_to_line(processed_line, "pop");
            }
            else
            {
                is_sym_line = true;
                append_to_line(processed_line, "sym");
                // add_symbol(&sym, &size, &count, token);
            }
            fprintf(output_file, "%s [%s]", token, processed_line);
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
