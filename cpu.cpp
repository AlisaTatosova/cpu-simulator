#include "cpu.h"

CPU::CPU() : alu(*this), eip(0) {
    name_instructions();
    name_registers();
    init_flags();
}

void CPU::name_instructions() {
    instructions[0] = "mov";
    instructions[1] = "add";
    instructions[2] = "sub";
    instructions[3] = "mul";
    instructions[4] = "div";
    instructions[5] = "inc";
    instructions[6] = "dec";
    instructions[7] = "cmp";
    instructions[8] = "je";
    instructions[9] = "jne";
    instructions[10] = "jl";
    instructions[11] = "jle";
    instructions[12] = "jg";
    instructions[13] = "jge";
    instructions[14] = "print";
    instructions[15] = "end";
}

void CPU::name_registers() {
    registers["r1"] = 0;
    registers["r2"] = 0;
    registers["r3"] = 0;
    registers["r4"] = 0;
    registers["r5"] = 0;
    registers["r6"] = 0;
    registers["r7"] = 0;
    registers["r8"] = 0;
    registers["r9"] = 0;
    registers["r10"] = 0;
}

void CPU::init_flags() {
    eflags["ZF"] = 0; //if == 0 FLAG ZF = 1
    eflags["SF"] = 0; // if < 0 flag SF = 1 
    eflags["UF"] = 0; // if > 0 flag UF = 1
}

//fetch function reads from file and saves the containment in map: map<int, string> ram, where key is address of instruction, value is instruction 
void CPU::fetch(std::ifstream& file) {
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string instruction, op1, op2, comma, result;
        iss >> instruction;

        if (instruction[instruction.size() - 1] == ':') { // if instruction is label keep in map std::map<std::string, int> the address of label
            iss >> op1 >> op2;
            labels[instruction] = eip;
            result = instruction + " " + op1 + " " + op2;
        } else if (instruction == instructions[5] || instruction == instructions[6] || instruction == instructions[8] || instruction == instructions[9] || instruction == instructions[10] || instruction == instructions[11]
             || instruction == instructions[12] || instruction == instructions[13] || instruction == instructions[14] || instruction == instructions[15]) { // instructions with 1 operand
                iss >> op1 >> op2;
                result = instruction + " " + op1 + " " + op2;
        } else if (instruction == instructions[0] || instruction == instructions[1] || instruction == instructions[2] || instruction == instructions[3]
             || instruction == instructions[4] || instruction == instructions[7]) {
            iss >> op1;
            if (!has_comma(op1)) {
                iss >> comma >> op2;
                if (!has_comma(comma)) {
                    return;
                } else {
                    result = instruction + " " + op1 + " " + op2;
                }
            } else {
                iss >> op2;
                result = instruction + " " + op1 + " " + op2;
            }
        } else {
            std::cout << instruction << "No such instruction" << std::endl;
            return;
        }

        std::string unnecessary;
        iss >> unnecessary;
        if (unnecessary != "") {
            std::cout << "Syntax erreor" << std::endl;
            return;
        }
            
        ram[eip] = result;
        ++eip;
    }
}

bool CPU::is_register(const std::string& op) { 
    bool op_is_register = false;
    for (auto it = registers.begin(); it != registers.end(); ++it) {
        const std::string& val  = it -> first;
        if (op == val) {
            op_is_register = true;
        }

        if (op_is_register) {
            break;
        }
    }

    if (!op_is_register) {
        return false;
    }
    return true;
}

//checking for the operand: operands must be regiter register or register literal
void CPU::check_operands(std::string& op1, std::string& op2, void (ALU::*fptr)(std::string&, const std::string&)) {
    remove_comma(op1);
    bool is_register_op1 = is_register(op1);
    if (!is_register_op1) {
        throw InvalidOperandException(op1);
    }

    if (op2 == "") { // the case that instruction is inc or dec, faking that it has second arg: which will be empty string, to be able to use same fptr
        std::string str1 = std::to_string(registers[op1]);
        if (fptr) {
            (alu .* fptr)(str1, op2); // calling the member function pointed to by fptr
        } else {
            std::cout << "Unknown instruction" << std::endl;
        }
        registers[op1] = std::stoi(str1);
        return;
    }

    bool is_register_op2 = is_register(op2);
    if (is_register_op2) {
        std::string str1 = std::to_string(registers[op1]);
        std::string str2 = std::to_string(registers[op2]);
        if (fptr) {
            (alu .* fptr)(str1, str2); // calling the member function pointed to by fptr
        } else {
            std::cout << "Unknown instruction" << std::endl;
        }
        registers[op1] = std::stoi(str1);
    } else if (is_number(op2)) { // if seond operand literal
        std::string str1 = std::to_string(registers[op1]);
        if (fptr) {
            (alu .* fptr)(str1, op2);
        } else {
            std::cout << "Unknown instruction" << std::endl;
        }
        registers[op1] = std::stoi(str1);
    } else {
        throw InvalidOperandException(op2);
    }
}

void CPU::execute(int i) {
    auto it = ram.find(i);
    while (it != ram.end()) {
        const std::string& value = it -> second;

        size_t first_space = value.find(' ');
        size_t second_space = value.find(' ', first_space + 1);

        std::string instruction = value.substr(0, first_space);
        std::string op1 = value.substr(first_space + 1, second_space - first_space - 1);
        std::string op2 = value.substr(second_space + 1);

        bool should_break = false; // Flag to indicate whether to break execution

        if (instruction == instructions[0]) { 
            check_operands(op1, op2, ALU::mov);
        } else if (instruction == instructions[1]) { 
            check_operands(op1, op2, ALU::add);
        } else if (instruction == instructions[2]) { 
            check_operands(op1, op2, ALU::sub);
        } else if (instruction == instructions[3]) { 
            check_operands(op1, op2, ALU::mul);
        } else if (instruction == instructions[4]) { 
            check_operands(op1, op2, ALU::div);
        } else if (instruction == instructions[5]) { 
            check_operands(op1, op2, ALU::inc);
        } else if (instruction == instructions[6]) { 
            check_operands(op1, op2, ALU::dec);
        } else if (instruction == instructions[7]) { 
            check_operands(op1, op2, ALU::cmp);
        } else if (instruction == instructions[8]) { 
            if (alu.je(op1)) {
                should_break = true;
            } 
        } else if (instruction == instructions[9]) { 
            if (alu.jne(op1)) {
                should_break = true;
            } 
        } else if (instruction == instructions[10]) { 
            if (alu.jl(op1)) {
                should_break = true;
            } 
        } else if (instruction == instructions[11]) {\
            if (alu.jle(op1)) {
                should_break = true;
            } 
        } else if (instruction == instructions[12]) { 
            if (alu.jg(op1)) {
                should_break = true;
            } 
        } else if (instruction == instructions[13]) { 
            if (alu.jge(op1)) {
                should_break = true;
            } 
        } else if (instruction == instructions[14]) { 
            print(op1);
        } else if (instruction == instructions[15]) { // Exit instruction
            break;
        }

        if (should_break) {
            break;
        }
        
        ++it; // move to the next instruction in memory
    }
}

void CPU::print(const std::string& op1) {
    bool is_register_op = is_register(op1);
    if (is_register_op) {
        std::cout << registers[op1] << std::endl;            
    } else {
        std::cout << op1 << std::endl;
    }
}

char CPU::get_flag(const std::string& flag) {
    return eflags[flag];
}

void CPU::set_flag(const std::string& flag, char code) {
    eflags[flag] = code;
}

bool CPU::is_number(const std::string& str) {
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

std::map<std::string, int> CPU::get_labels() const {
    return labels;
}

void CPU::remove_comma(std::string& op) {
    if (op[op.size() - 1] == ',') {
        op.pop_back();
    }
}

bool CPU::has_comma(const std::string& op) {
    if (op[op.size() - 1] == ',') {
        return true;
    }
    return false;
}