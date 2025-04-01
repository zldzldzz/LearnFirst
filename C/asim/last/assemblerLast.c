#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ASMfile "prog4.asm"
#define cp_ASMfile "prog3.asm"

#define INS_txt "instruction.txt"
#define ins_MAX 200
#define REG_txt "register.txt"
#define reg_MAX 16
#define MACROCNT 5

struct reg
{
    char name[3];
    char dType[2];
    char bCode[4];
} reg[reg_MAX];
int regCnt = 0;

struct instruction
{
    char name[6];
    char dType[2];
    char dest[2];
    char sour[2];
    char insLen[2];
    char xCode[3];
    char bCode[9];
    // char etc[5];
} ins[ins_MAX];
int insCnt = 0;

struct symbol
{
    char name[10];
    char dType[2];
    char data[5][9];
    int LCsave;
} sym[5];
int symCnt = 0;

struct label
{
    char name[10];
    int LCsave;
} lab[5];
int labelCnt = 0;

struct sentence
{
    char label[10];
    char instruction[6];
    char data[5][9];
} sen;

int init()
{
    int sen;
    printf("start init.\n");
    printf("\ninstruction initialize.\n");
    FILE *fpins = fopen(INS_txt, "r");
    if (fpins == NULL)
    {
        printf("instruction file error");
        return 0;
    }
    while (!feof(fpins))
    {
        sen = fscanf(fpins, "%6s%2s%2s%2s%2s%3s%8s\n",
                     ins[insCnt].name, ins[insCnt].dType,
                     ins[insCnt].dest, ins[insCnt].sour,
                     ins[insCnt].insLen, ins[insCnt].xCode,
                     ins[insCnt].bCode);
        printf("%s %s %s %s to %s\n", ins[insCnt].name, ins[insCnt].dType, ins[insCnt].xCode, ins[insCnt].sour, ins[insCnt].dest);
        insCnt++;
    }
    fclose(fpins);

    printf("\nregister initialize.\n");
    FILE *fpreg = fopen(REG_txt, "r");
    if (fpreg == NULL)
    {
        printf("register file error");
        return 0;
    }
    while (!feof(fpreg))
    {
        sen = fscanf(fpreg, "%3s%2s%4s\n", reg[regCnt].name, reg[regCnt].dType, reg[regCnt].bCode);
        printf("%s %s\n", reg[regCnt].name, reg[regCnt].bCode);
        regCnt++;
    }
    fclose(fpreg);
    printf("\n");
    return 0;
}

char *cutp;
int wflag = 0;
int sentenceProcess(char buffer[])
{
    char line[100];
    int tempCnt = 0;
    strcpy(sen.label, "");
    strcpy(sen.instruction, "");
    for (int k = 0; k < 5; k++)
        strcpy(sen.data[k], "\0\0\0\0\0\0\0\0");
    strcpy(line, buffer);
    if (line[strlen(line) - 1] == '\n')
        line[strlen(line) - 1] = '\0'; // 문장 끝 \n 제거

    cutp = strtok(line, "\t ");

    if (line[0] == '\t')
    { // label이 존재하지 않을 경우
    }
    else
    { // label이 존재
        strcpy(sen.label, cutp);
        cutp = strtok(NULL, ",\t ");
    }
    strcpy(sen.instruction, cutp);
    if (stricmp(sen.instruction, "assume") == 0)
    {
        wflag = 1;
        return -1;
    }
    if (stricmp(sen.instruction, "ENDS") == 0)
        wflag = 0;
    cutp = strtok(NULL, ",\t ");
    while (cutp != NULL)
    {
        if (tempCnt == 5)
        {
            printf("\nerror : to much data in this sentence\n");
            return 0;
        }
        strcpy(sen.data[tempCnt], cutp);
        tempCnt++;
        cutp = strtok(NULL, ",\t ");
    }

    return tempCnt; // return data cnt.
}
void macroProcess()
{
    char buffer[51], buffer1[51], bufferM[51], mac[15];
    char *MACList[MACROCNT];
    int macCnt = 0, wFlag = 0, sFlag = 0;
    FILE *fpA = fopen(ASMfile, "r");
    if (fpA == NULL)
    {
        printf("base ASM file error");
        return;
    }
    FILE *fpB = fopen(cp_ASMfile, "w");
    while (!feof(fpA))
    {
        sFlag = 0;
        fgets(buffer, 51, fpA);
        sentenceProcess(buffer);
        if (stricmp(sen.instruction, "SEGMENT") == 0)
            wFlag = 1;
        for (int k = 0; k < macCnt; k++)
            if (strnicmp(sen.instruction, MACList[k], strlen(sen.instruction)) == 0)
            {
                sFlag = 1;
                strcpy(mac, sen.instruction);
                FILE *fpM = fopen(strcat(mac, ".txt"), "r");
                int a = 0;
                while (!feof(fpM))
                {
                    fgets(bufferM, 51, fpM);
                    a++;
                }
                rewind(fpM);
                int b = 0;
                while (b < a - 1)
                {
                    fgets(bufferM, 51, fpM);
                    fputs(bufferM, fpB);
                    b++;
                }
                fclose(fpM);
            }
        if (sFlag == 1)
            continue;
        if (wFlag == 1)
            fputs(buffer, fpB);
        if (stricmp(sen.instruction, "END") == 0)
            wFlag = 0;
        if (stricmp(sen.instruction, "MACRO") == 0)
        {
            FILE *fpM = fopen(strcat(sen.label, ".txt"), "w");
            MACList[macCnt] = (char *)malloc(sizeof(char));
            strcpy(MACList[macCnt], sen.label);
            while (1)
            {
                fgets(buffer, 51, fpA);
                strcpy(buffer1, buffer);
                cutp = strtok(buffer1, " \n\t,");
                cutp = strtok(NULL, "\n\t, ");
                if (stricmp(cutp, "ENDS") == 0)
                    break;
                fputs(buffer, fpM);
            }
            macCnt++;
            fclose(fpM);
        }
    }
    fclose(fpA);
    fclose(fpB);
    for (int k = 0; k < macCnt; k++)
        printf("%s\n", MACList[k]);
}
int sympass()
{
    char buffer[51];
    FILE *fpR = fopen(cp_ASMfile, "r");
    while (!feof(fpR))
    {
        fgets(buffer, 51, fpR);
        int wordCnt = sentenceProcess(buffer);
        if (stricmp(sen.instruction, "dw") == 0 || stricmp(sen.instruction, "db") == 0)
        {
            strcpy(sym[symCnt].name, sen.label);
            if (stricmp(sen.instruction, "dw") == 0)
            {
                strcpy(sym[symCnt].dType, "w");
            }
            else if (stricmp(sen.instruction, "db") == 0)
            {
                strcpy(sym[symCnt].dType, "b");
            }
            symCnt++;
        }
    }
    for (int k = 0; k < symCnt; k++)
        printf("%s %s\n", sym[k].name, sym[k].dType);
    return 0;
}
int insSel(int passflag)
{
    int select = 0;
    char dest[2] = {'\0'}, sour[2] = {'\0'}, dataType[2] = {'\0'}, instruction[6] = {'\0'};
    if (isdigit(sen.data[0][0]) != 0)
        strcpy(dest, "i");
    if (isdigit(sen.data[1][0]) != 0)
        strcpy(sour, "i");
    for (int k = 0; k < labelCnt; k++)
    {
        if (stricmp(lab[k].name, sen.data[0]) == 0)
            strcpy(dest, "L");
        if (stricmp(lab[k].name, sen.data[1]) == 0)
            strcpy(sour, "L");
    }
    for (int k = 0; k < regCnt; k++)
    { // register distinguish
        // printf("|%s %s %s|\n", sen.data[0], sen.data[1], reg[k].name );
        if (stricmp(sen.data[0], reg[k].name) == 0)
        {
            if (stricmp(reg[k].name, "AL") == 0 || stricmp(reg[k].name, "AX") == 0)
            {
                strcpy(dest, "a");
                strcpy(dataType, reg[k].dType);
            }
            else
            {
                strcpy(dest, "r");
                strcpy(dataType, reg[k].dType);
            }
            // printf("%s %s %s\n", reg[k].name, sen.data[0], reg[k].dType);
        }
        if (stricmp(sen.data[1], reg[k].name) == 0)
        {
            if (stricmp(reg[k].name, "AL") == 0 || stricmp(reg[k].name, "AX") == 0)
            {
                strcpy(sour, "a");
                strcpy(dataType, reg[k].dType);
            }
            else
            {
                strcpy(sour, "r");
                strcpy(dataType, reg[k].dType);
            }
            // printf("%s %s %s\n", reg[k].name, sen.data[1], reg[k].dType);
        }
    }
    if (passflag == 1 && (strcmp(dest, "") == 0 || strcmp(sour, "") == 0))
    { // symbol distinguish pass1
        // for (int k = 0; k < symCnt; k++)if (stricmp(sen.data[0], sym[k].name) == 0 || stricmp(sen.data[1], sym[k].name) == 0) strcpy(dataType, sym[k].dType);
        /*strcpy(dataType, "w");
        if (strcmp(dest, "") == 0) {
            strcpy(dest, "m");
        }
        if (strcmp(sour, "") == 0) {
            strcpy(sour, "m");
        }*/
        for (int k = 0; k < symCnt; k++)
        {
            if (stricmp(sen.data[0], sym[k].name) == 0)
            {
                strcpy(dest, "m");
                strcpy(dataType, sym[k].dType);
            }
            if (stricmp(sen.data[1], sym[k].name) == 0)
            {
                strcpy(sour, "m");
                strcpy(dataType, sym[k].dType);
            }
        }
        // ex INC DL -> sour == NULL -> s(single)
        if (strcmp(sour, "") == 0)
        {
            strcpy(sour, "s");
        }
    }
    if (passflag == 2)
    { // symbol distinguish pass2
        for (int k = 0; k < symCnt; k++)
        {
            if (stricmp(sen.data[0], sym[k].name) == 0)
            {
                strcpy(dest, "m");
                strcpy(dataType, sym[k].dType);
            }
            if (stricmp(sen.data[1], sym[k].name) == 0)
            {
                strcpy(sour, "m");
                strcpy(dataType, sym[k].dType);
            }
        }
    }
    if (wflag == 1)
    {
        // printf("dest %s, sour %s dataType %s\n", dest, sour, dataType);
        // printf("sentence 1%s 2%s\n", sen.label, sen.instruction);
        if (sen.data[1][0] == '\0')
        {
            // printf("sour null\n");
            for (select = 0; select < insCnt + 1; select++)
            {
                if (stricmp(sen.instruction, "jmp") == 0 || stricmp(sen.instruction, "je") == 0 || stricmp(sen.instruction, "ja") == 0 || stricmp(sen.instruction, "jb") == 0 || stricmp(sen.instruction, "int") == 0)
                    if (stricmp(sen.instruction, ins[select].name) == 0 && stricmp(ins[select].dest, dest) == 0)
                    {
                        break;
                    }
                if (stricmp(sen.instruction, ins[select].name) == 0 && stricmp(ins[select].dest, dest) == 0 &&
                    stricmp(ins[select].dType, dataType) == 0)
                {
                    break;
                }
                if (select == insCnt)
                {
                    return -1;
                }
            }
        }
        else
        {
            for (select = 0; select < insCnt + 1; select++)
            {
                if (stricmp(sen.instruction, ins[select].name) == 0 && stricmp(ins[select].dest, dest) == 0 &&
                    stricmp(ins[select].sour, sour) == 0 && stricmp(ins[select].dType, dataType) == 0)
                    break;
                if (select == insCnt)
                {
                    return -1;
                }
            }
        }
    }
    return select;
}
int passi()
{
    char buffer[51];
    int LC = 0, select = 0, symsub = 0;
    printf("start pass1\n");
    FILE *fpR = fopen(cp_ASMfile, "r");
    if (fpR == NULL)
    {
        printf("copy ASM file error.");
        return 0;
    }
    while (!feof(fpR))
    {
        fgets(buffer, 51, fpR);
        int wordCnt = sentenceProcess(buffer), LCType = 1;
        // printf("inctruction : %s\ndest : %s\n",sen.instruction,sen.data[0]);
        int mflag = 0;
        if (sen.label[strlen(sen.label) - 1] == ':')
        {
            sen.label[strlen(sen.label) - 1] = '\0';
            strcpy(lab[labelCnt].name, sen.label);
            lab[labelCnt].LCsave = LC;
            labelCnt++;
        }
        if (stricmp(sen.instruction, "dw") == 0 || stricmp(sen.instruction, "db") == 0)
            mflag = 1;
        if (wordCnt == -1)
            continue;
        if (mflag != 1)
            select = insSel(1);
        if (select == -1)
        {
            printf("error: instruction not exist.\n\n");
            continue;
        }
        cutp = strtok(NULL, ",\t ");
        if (mflag == 1)
        {
            sym[symsub].LCsave = LC;
            if (stricmp(sen.instruction, "dw") == 0)
            {
                strcpy(sym[symsub].dType, "w");
                LCType = 2;
            }
            else if (stricmp(sen.instruction, "db") == 0)
            {
                strcpy(sym[symsub].dType, "b");
                LCType = 1;
            }
            // printf("\ndata type :%s, LC save :%.4x\n", sym[symCnt].dType, sym[symCnt].LCsave);
            for (int k = 0; k < wordCnt; k++)
                strcpy(sym[symsub].data[k], sen.data[k]);
            symsub++;
        }
        if (wflag == 1)
        {
            printf("LC:|%.4x|  %s %s ", LC, sen.label, sen.instruction);
            for (int k = 0; k < wordCnt; k++)
                printf("%s ", sen.data[k]);
            printf("\n");
            if (mflag != 1)
            {
                // printf("%s %s %s %s to %s\n", ins[select].name, ins[select].dType, ins[select].xCode, ins[select].sour, ins[select].dest);
                LC += atoi(ins[select].insLen);
                // printf("%s\n", ins[select].insLen);
            }
            if (mflag == 1)
            {
                LC += LCType * wordCnt;
            }
        }
        printf("\n");
    }
    fclose(fpR);
    return 0;
}
void btox(char *byte_bit)
{
    int acc = 0, l = 0;
    char buffer[9];
    strcpy(buffer, byte_bit);
    // printf("%s ", buffer);
    for (int k = 7; 0 <= k; k--)
    {
        if (buffer[k] == '1')
            acc += 1 << l;
        l++;
    }
    printf("%02X ", acc);
}
int passii()
{
    char buffer[51], binary_buffer[9], destB[4] = "\0\0\0", sourB[4] = "\0\0\0";
    int select = 0, zero = 0;
    ;
    printf("start pass2\n");
    FILE *fpR = fopen(cp_ASMfile, "r");
    if (fpR == NULL)
    {
        printf("copy ASM file error.");
        return 0;
    }
    while (!feof(fpR))
    {
        fgets(buffer, 51, fpR);
        int wordCnt = sentenceProcess(buffer);
        if (wordCnt == -1)
            continue;
        if (stricmp(sen.instruction, "dw") == 0 || stricmp(sen.instruction, "db") == 0)
            continue;

        select = insSel(2);
        cutp = strtok(NULL, ",\t ");
        if (wflag == 1)
        {
            if (stricmp(ins[select].sour, "r") == 0)
            {
                for (int k = 0; k < regCnt; k++)
                {
                    if (stricmp(sen.data[1], reg[k].name) == 0)
                    {
                        strcpy(sourB, reg[k].bCode);
                    }
                }
                strcpy(sen.data[1], "");
            }
            if (stricmp(ins[select].dest, "r") == 0)
            {
                for (int k = 0; k < regCnt; k++)
                {
                    if (stricmp(sen.data[0], reg[k].name) == 0)
                    {
                        strcpy(destB, reg[k].bCode);
                    }
                }
                strcpy(sen.data[0], "");
            }
            strcpy(binary_buffer, ins[select].bCode);
            // printf("%s ", ins[select].bCode);
            int count = 1;
            count = atoi(ins[select].insLen);
            // for (int k = 0; k < count; k++) printf("%d ", k);
            // printf("ins len : %d\n", count);
            for (int k = 0; k < 8; k++)
            {
                if (stricmp(ins[select].dest, "a") == 0 || stricmp(ins[select].dest, "m") == 0 || stricmp(ins[select].dest, "L") == 0 || stricmp(ins[select].dest, "L") == 0)
                    continue;
                if (strcmp(destB, "\0"))
                    continue;
                if (binary_buffer[k] == '?')
                {
                    for (int i = k; i < k + 3; i++, zero++)
                    {
                        binary_buffer[i] = destB[zero];
                    }
                    zero = 0;
                    break;
                }
            }
            for (int k = 0; k < 8; k++)
            {
                if (stricmp(ins[select].sour, "a") == 0)
                    continue;
                if (binary_buffer[k] == '?')
                {
                    for (int i = k; i < k + 3; i++, zero++)
                    {
                        binary_buffer[i] = sourB[zero];
                    }
                    zero = 0;
                    break;
                }
            }
            // printf("%s ",sen.label);
            if (stricmp(ins[select].xCode, "00") == 0)
            {
            }
            else
                printf("%s ", ins[select].xCode);

            if (stricmp(binary_buffer, "00000000") != 0)
                btox(binary_buffer);
            int n = 0, xFlag = 0;
            ;
            char xbuffer[5];
            for (int k = 0; k < 5; k++)
                if (stricmp(sen.data[k], "") != 0)
                {
                    if (isdigit(sen.data[k][0]) != 0)
                    { // 숫자인 경우
                        // printf("%s |%s| ", sen.data[k], ins[select].dType);
                        for (n = 0; n < strlen(sen.data[k]); n++)
                            if (sen.data[k][n] == 'h' || sen.data[k][n] == 'H')
                            {
                                xFlag = 1;
                                sen.data[k][n] = '\0';
                                break;
                            }
                        if (xFlag == 1)
                        {
                            if (stricmp(ins[select].dType, "w") == 0)
                                switch (strlen(sen.data[k]))
                                {
                                case 1:
                                    printf("0%c 00", sen.data[k][0]);
                                    break;
                                case 2:
                                    printf("%c%c 00", sen.data[k][0], sen.data[k][1]);
                                    break;
                                case 3:
                                    printf("%c%c 0%c", sen.data[k][1], sen.data[k][2], sen.data[k][0]);
                                    break;
                                default:
                                    printf("%c%c %c%c", sen.data[k][2], sen.data[k][3], sen.data[k][0], sen.data[k][1]);
                                    break;
                                }
                            else
                                switch (strlen(sen.data[k]))
                                {
                                case 1:
                                    printf("0%c ", sen.data[k][0]);
                                    break;
                                default:
                                    printf("%c%c ", sen.data[k][0], sen.data[k][1]);
                                    break;
                                }
                        }
                        else
                        {
                            if (stricmp(ins[select].dType, "w") == 0)
                            {
                                sprintf(xbuffer, "%04X", atoi(sen.data[k]));
                                printf("%c%c %c%c ", xbuffer[2], xbuffer[3], xbuffer[0], xbuffer[1]);
                            }
                            else
                                ;
                            printf("%02X ", atoi(sen.data[k]));
                        }
                    }
                    else
                    { // symbol, label인 경우
                        // printf("%s ", sen.data[k]);
                        for (int t = 0; t < labelCnt; t++)
                            if (stricmp(sen.data[k], lab[t].name) == 0)
                            {
                                sprintf(xbuffer, "%04X", lab[t].LCsave);
                                printf("%c%c %c%c ", xbuffer[2], xbuffer[3], xbuffer[0], xbuffer[1]);
                            }
                        for (int t = 0; t < symCnt; t++)
                            if (stricmp(sen.data[k], sym[t].name) == 0)
                            {
                                sprintf(xbuffer, "%04X", sym[t].LCsave);
                                printf("%c%c %c%c ", xbuffer[2], xbuffer[3], xbuffer[0], xbuffer[1]);
                            }
                    }
                }
        }
        printf("\n\n");
    }
    fclose(fpR);
    return 0;
}
int main(void)
{
    macroProcess();
    init();
    sympass();
    passi();
    for (int k = 0; k < symCnt; k++)
        printf("Symbol :%s, Type :%s, LC : %.4x\n", sym[k].name, sym[k].dType, sym[k].LCsave);
    passii();
    return 0;
}