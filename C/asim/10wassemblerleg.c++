#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <ctype.h>//strtok
#include <map>//��ųʸ� ���̺귯��
#include <cctype> //���ĺ����� �� �ܾ�����

using namespace std;

#define SIZE 1024 //1KB

FILE* FileOpenRead(const char*);  //���� ����
FILE* FileOpenWrite(const char* name);
int counter(FILE*);
void find_Registers(char* token, int* cnt);
void storeLabel(const char* label, int line_number);
void replaceLabel(FILE* inputFile, FILE* outFile);
map<string, int> labelMap;  

const char* registers[] = {//�������� �̸�
    "AX", "BX", "CX", "DX",  
    "AL", "AH", "BL", "BH", "CL", "CH", "DL", "DH",  
    "SI", "DI", "SP", "BP", 
    "IP", "FLAGS",  
    "CS", "DS", "SS", "ES", "FS", "GS",  
};

const char* instruc[] = {//��ɾ�
    "MOV", "ADD", "DIV", "INC", "DEC", "CMP", "SUB", "INT", 
    "JA", "JAE", "JB", "JBE", "JC", "JE", "JNE",  
    "DW", "DB"  
};

void storeLabel(const char* label, int line_number) {//��ųʸ��� ����� �Լ�
  
    string clean_label = label;
    if (clean_label.back() == ':') {
        clean_label.pop_back();  
    }


    if (labelMap.find(clean_label) == labelMap.end()) {
        labelMap[clean_label] = line_number; 
    }
}


int counter(FILE* fp) {
    char buffer[SIZE];//�޾ƿ� ������ ��ü ����
    int wordIp = 0;//�ܾ��� ��ġ��
    char* pre_token = NULL;//���� ��ū�� �� ����(���� ã�� Ȱ��)
    int ordIp = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {  
        int instructionLength = sizeof(instruc) / sizeof(instruc[0]);//��ɾ� �迭�� ����
        bool check = true;

        char* token = strtok(buffer, "\t\n,+[] ");//strtok ������ ���ڿ��� "\t\n,+[] "�� �����ڷ� �ڸ�
        
        while (token != NULL) {
            if (token[strlen(token) - 1] == ':') {//���̺����� Ȯ�� :�� �������� :�� ���� �ְ�
                token[strlen(token) - 1] = '\0';  //���̺��� :�� ����
                cout  << token << " " << wordIp <<" 0"<< endl;
                storeLabel(token, wordIp); //key�� value�� ��ųʸ��� ����
            }
            else if (strcmp(token, "DB") == 0 || strcmp(token, "DW") == 0) {
    char* data_value = strtok(NULL, "\t\n,+[] "); // DB�� DW ���� ���� ����
    if (data_value != NULL) {
        cout << pre_token << " " << ordIp << " " << data_value << endl; // pre_token, ordIp, data_value ���
        storeLabel(pre_token, ordIp); // ���� ���̺� ����
    }
}
      
            bool isInstruction = false;

            for (int i = 0; i < instructionLength; i++) {//�ش� ��ū�� ��ɾ����� Ȯ�� ��ɾ ������ isInstruction�� ������
                if (strcmp(token, instruc[i]) == 0) {
                    isInstruction = true;
                    wordIp+=1;//��ɾ ���� üũ
                    break;
                }
            }

            if (!isInstruction) {//��ɾ �ƴ� �ֵ鸸 ������������ Ȯ��
                find_Registers(token, &wordIp);
                  
            }

            pre_token = token;  
            ordIp = wordIp;   
            token = strtok(NULL, "\t\n,+[] ");//���� ��ū �ҷ�����
        }
        wordIp+=1;//üũ
    }
    return 1;
}

void find_Registers(char* token, int* cnt) {
    int register_count = sizeof(registers) / sizeof(registers[0]);
    bool is_register = false;

    for (int i = 0; i < register_count; i++) {
        if (strcmp(token, registers[i]) == 0) {
            is_register = true;
            break;
        }
    }

    if (!is_register) {
        (*cnt)+=1;//��ɾ �ƴϰ� �������͵� �ƴѰ�� cnt����(���̺�, ���, ����) üũ
    }
}


void replaceLabel(FILE* inputFile, FILE* outFile) {//out.txt�� �����ϴ� �κ�
    char buffer[SIZE]; 
    while (fgets(buffer, sizeof(buffer), inputFile)) {
        char* token = strtok(buffer, "\t\n,+[] "); 
        while (token != NULL) {
            if (token[strlen(token) - 1] == ':') {
                token[strlen(token) - 1] = '\0';
            }

            if (labelMap.find(token) != labelMap.end()) {
                int line_number = labelMap[token];
                fprintf(outFile, "%d ", line_number);  
            }
            else {
                fprintf(outFile, "%s ", token);  
            }
            token = strtok(NULL, "\t\n,+[] "); 
        }
        fprintf(outFile, "\n");
    }
}


FILE* FileOpenRead(const char* name) {
	FILE* fp;
	fp = fopen(name, "r");
	// ���� Ȯ��
	if (fp == NULL) {
		printf("%s : �б� ���� ���� ����\n", name);
		return NULL;
	}
	else {
		printf("%s : �б� ���� ���� ����\n", name);
		return fp;
	}
}

FILE* FileOpenWrite(const char* name) {
	FILE* fp;
	fp = fopen(name, "w");
	// ���� Ȯ��
	if (fp == NULL) {
		printf("%s : ���� ���� ���� ����\n", name);
		return NULL;
	}
	else {
		printf("%s : ���� ���� ���� ����\n", name);
		return fp;
	}
}

int main(void) {
	//STRING FILE
	FILE* stringMfp = NULL;
	const char* fileName = "symbol.txt";  //�� ���� ����
	stringMfp = FileOpenRead(fileName);  //���� ���� Part

	//���� ���� �˻�
	if (stringMfp != NULL) {
		//NULL�� �ƴҽ� ����
		counter(stringMfp);
		fclose(stringMfp);  // ���� �ݱ�
	}

	stringMfp = FileOpenRead(fileName);
	FILE* endFp = NULL;
	fileName = "out.txt";
	endFp = FileOpenWrite(fileName);
	if (endFp != NULL) {
		//NULL�� �ƴҽ� ����
		replaceLabel(stringMfp, endFp);
		fclose(stringMfp);  // ���� �ݱ�
		fclose(endFp);  // ���� �ݱ�
	}

	return 0;
}