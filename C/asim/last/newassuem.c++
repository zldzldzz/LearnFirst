#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <unordered_set>

class SymbolTable {
public:
    void AddSymbol(const std::string& label, int address, const std::string& type = "", const std::string& value = "") {
        symbols[label] = {address, type, value};
    }

    bool HasSymbol(const std::string& label) const {
        return symbols.find(label) != symbols.end();
    }

    int GetAddress(const std::string& label) const {
        return symbols.at(label).address;
    }

    void Display() const {
        printf("\n%-10s%-10s%-10s%-10s\n", "Symbol", "Type", "Address", "Value");
        std::vector<std::pair<std::string, SymbolInfo>> sortedSymbols(symbols.begin(), symbols.end());

        // Sort symbols by Symbol name
        std::sort(sortedSymbols.begin(), sortedSymbols.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        for (const auto& entry : sortedSymbols) {
            printf("%-10s%-10s%04X      %-10s\n", 
                   entry.first.c_str(), 
                   entry.second.type.empty() ? " " : entry.second.type.c_str(), 
                   entry.second.address, 
                   entry.second.value.empty() ? " " : entry.second.value.c_str());
        }
    }

private:
    struct SymbolInfo {
        int address;
        std::string type;
        std::string value;
    };

    std::unordered_map<std::string, SymbolInfo> symbols;
};

class InstructionTable {
public:
    struct Instruction {
        std::string operand1;
        std::string operand2;
        std::string size;
        std::string opcode;
        int length;
        std::string binaryPattern;
    };

    void Load(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            Instruction inst;
            std::string mnemonic;
            iss >> mnemonic >> inst.operand1 >> inst.operand2 >> inst.size >> inst.opcode >> inst.length >> inst.binaryPattern;
            instructions[mnemonic].push_back(inst);
        }
    }

    const std::vector<Instruction>& GetInstructions(const std::string& mnemonic) const {
        return instructions.at(mnemonic);
    }

    bool HasInstruction(const std::string& mnemonic) const {
        return instructions.find(mnemonic) != instructions.end();
    }

private:
    std::unordered_map<std::string, std::vector<Instruction>> instructions;
};

class RegisterTable {
public:
    void Load(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string regName, regCode;
            iss >> regName >> regCode;
            registers[regName] = regCode;
        }
    }

    bool HasRegister(const std::string& regName) const {
        return registers.find(regName) != registers.end();
    }

    bool Is16BitRegister(const std::string& regName) const {
        static const std::unordered_set<std::string> reg16 = {"AX", "CX", "DX", "BX", "SP", "BP", "EX", "SI", "DI"};
        return reg16.find(regName) != reg16.end();
    }

    bool Is8BitRegister(const std::string& regName) const {
        static const std::unordered_set<std::string> reg8 = {"AL", "AH", "BL", "BH", "CL", "CH", "DL", "DH"};
        return reg8.find(regName) != reg8.end();
    }

private:
    std::unordered_map<std::string, std::string> registers;
};

class Assembler {
public:
    Assembler(const InstructionTable& instTable, const RegisterTable& regTable)
        : instructionTable(instTable), registerTable(regTable), addressCounter(0) {}

    void ProcessPass1(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;

        std::cout << "Pass 1 Output:\n";
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == ';') {
                continue;
            }

            int currentAddress = addressCounter;
            std::string output = ProcessLinePass1(line);
            std::cout << std::setw(4) << std::setfill('0') << std::hex << currentAddress << " " << output << "\n";
        }

        symbolTable.Display();
    }

private:
    const InstructionTable& instructionTable;
    const RegisterTable& registerTable;
    SymbolTable symbolTable;
    int addressCounter;

    std::string ProcessLinePass1(const std::string& line) {
        std::istringstream iss(line);
        std::string word;
        iss >> word;

        if (word.back() == ':') {
            std::string label = word.substr(0, word.size() - 1);
            symbolTable.AddSymbol(label, addressCounter, " ");
            addressCounter += 2; // Increment address counter for labels
            return word;
        }

        if (instructionTable.HasInstruction(word)) { 
            return ProcessInstructionPass1(word, iss);
        }

        return ProcessDataDefinitionPass1(word, iss);
    }

    std::string ProcessInstructionPass1(const std::string& mnemonic, std::istringstream& iss) {
        std::string operand1, operand2;
        iss >> operand1;

        if (operand1.back() == ',') {
            operand1.pop_back();
            iss >> operand2;
        }

        for (const auto& inst : instructionTable.GetInstructions(mnemonic)) {
            if (MatchOperands(inst, operand1, operand2)) {
                int length = inst.length;
                addressCounter += length;
                std::ostringstream oss;
                oss << mnemonic << " " << operand1 << (operand2.empty() ? "" : ", " + operand2);
                return oss.str();
            }
        }

        addressCounter += 3; // Default to 3 bytes if no match
        return mnemonic + " " + operand1 + (operand2.empty() ? "" : ", " + operand2);
    }

    std::string ProcessDataDefinitionPass1(const std::string& symbol, std::istringstream& iss) {
        std::string size, value;
        iss >> size >> value;
        int length = (size == "DW") ? 3 : 2;
        symbolTable.AddSymbol(symbol, addressCounter, size, value);
        addressCounter += length;
        return symbol + " " + size + " " + value;
    }

    bool MatchOperands(const InstructionTable::Instruction& inst, const std::string& op1, const std::string& op2) {
        if (inst.operand1 != GetOperandType(op1)) return false; 
        if (!inst.operand2.empty() && inst.operand2 != GetOperandType(op2)) return false; 
        return true;
    }

    std::string GetOperandType(const std::string& operand) {
        if (registerTable.HasRegister(operand)) return "R";
        if (operand.front() == '[' && operand.back() == ']') return "M"; 
        if (std::all_of(operand.begin(), operand.end(), [](char c) { return std::isdigit(c) || c == '-'; })) return "I"; 
        if (symbolTable.HasSymbol(operand)) return "M"; 
        return "M"; 
    }
};

int main() {

    char line[255];

	FILE* file = fopen("inst.txt", "r");
	FILE* file1 = fopen("reg.txt", "r");
	

	if (file == NULL) {
		printf("파일열기 실패\n");
		return 1;
	}
		printf("inst table\n");

	while (fgets(line, sizeof(line), file) != NULL ) {
		printf("%s", line);
	}
		printf("\n\n");
		printf("reg table\n");


    while (fgets(line, sizeof(line), file1) != NULL ) {
		printf("%s", line);
	}
		printf("\n");

	fclose(file);
	fclose(file1);
		printf("\n");



    InstructionTable instTable;
    RegisterTable regTable;

    instTable.Load("inst.txt");
    regTable.Load("reg.txt");

    Assembler assembler(instTable, regTable);
    assembler.ProcessPass1("test1.txt");

    return 0;
}