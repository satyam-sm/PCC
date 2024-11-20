#include <bits/stdc++.h>

using namespace std;

unordered_map<string, int> symbolTable;

unordered_map<string, string> opcodeTable = {
    {"MOV", "MOV"}, {"ADD", "ADD"}, {"SUB", "SUB"},
    {"JMP", "JMP"}, {"MVI", "MVI"}, {"HLT", "HLT"}
};

struct Instruction {
    string label;
    string opcode;
    vector<string> operands;
    int address;
};

vector<string> split(const string& str) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void pass1(vector<Instruction>& instructions, const string& inputFile) {
    ifstream infile(inputFile);
    string line;
    int address = 0;

    while (getline(infile, line)) {
        vector<string> tokens = split(line);
        if (tokens.empty()) continue;

        Instruction instruction;
        int index = 0;

        if (tokens[index].back() == ':') {
            instruction.label = tokens[index].substr(0, tokens[index].size() - 1);
            symbolTable[instruction.label] = address;
            index++;
        }

        if (index < tokens.size()) {
            instruction.opcode = tokens[index++];
        }

        while (index < tokens.size()) {
            instruction.operands.push_back(tokens[index++]);
        }

        instruction.address = address;
        instructions.push_back(instruction);

        address += 2;
    }

    infile.close();
}

void pass2(const vector<Instruction>& instructions, const string& outputFile) {
    ofstream outfile(outputFile);
    
    for (const auto& instr : instructions) {
        stringstream machineCode;

        if (opcodeTable.find(instr.opcode) != opcodeTable.end()) {
            machineCode << opcodeTable[instr.opcode] << " ";
        } else {
            cerr << "Error: Unknown opcode " << instr.opcode << endl;
            continue;
        }

        for (const auto& operand : instr.operands) {
            if (symbolTable.find(operand) != symbolTable.end()) {
                machineCode << setw(4) << setfill('0') << hex << symbolTable[operand] << " ";
            } else {
                machineCode << operand << " ";
            }
        }

        outfile << setw(4) << setfill('0') << hex << instr.address << ": " << machineCode.str() << endl;
    }

    outfile.close();
}

int main() {
    string inputFile = "assembly_code.asm";
    string outputFile = "machine_code.txt";

    vector<Instruction> instructions;

    pass1(instructions, inputFile);
    pass2(instructions, outputFile);

    cout << "Symbol Table:" << endl;
    for (const auto& symbol : symbolTable) {
        cout << symbol.first << " -> " << hex << symbol.second << endl;
    }

    cout << "Machine code generated in " << outputFile << endl;

    return 0;
}
