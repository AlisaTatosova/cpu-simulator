#include "cpu.h"

CPU::CPU() : eip{0} {
    name_instructions();
    name_registers();
    init_flags();
}

//fetch function reads from file and saves the containment in map: map<int, string> ram, where key is address of instruction, value is instruction 
void CPU::fetch(std::ifstream& file) {
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string instruction;
        std::string op1;
        std::string op2;
        std::string result = "";
        iss >> instruction;

        if (instruction[instruction.size() - 1] == ':') {
            labels[instruction] = eip;
            result += instruction;
        } else if (instruction == instructions[8] || instruction == instructions[9] || instruction == instructions[10] || instruction == instructions[11]
             || instruction == instructions[12] || instruction == instructions[13]) {
                iss >> op1;
                result += instruction;
                result += " ";
                result += op1;

        } else {
            iss >>  op1 >> op2;
            result += instruction;
            result += " ";
            result += op1;
            result += " ";
            result += op2;
        }

        ram[eip] = result;
        std::cout << ram[eip] << " \n";
        ++eip;
    }
}

void CPU::mov(std::string& op1, const std::string& op2) {
    int num1 = std::stoi(op1);
    int num2 = std::stoi(op2);
    op1 = std::to_string(num2);
}

void CPU::add(std::string& op1, const std::string& op2) {
    int num1 = std::stoi(op1);
    int num2 = std::stoi(op2);
    num1 += num2;
    op1 = std::to_string(num1);
}

void CPU::sub(std::string& op1, const std::string& op2) {
    int num1 = std::stoi(op1);
    int num2 = std::stoi(op2);
    num1 -= num2;
    op1 = std::to_string(num1);
}

void CPU::mul(std::string& op1, const std::string& op2) {
    int num1 = std::stoi(op1);
    int num2 = std::stoi(op2);
    num1 *= num2;
    op1 = std::to_string(num1);
}

void CPU::div(std::string& op1, const std::string& op2) {
    int num1 = std::stoi(op1);
    int num2 = std::stoi(op2);
    num1 /= num2;
    op1 = std::to_string(num1);
}

void CPU::inc(std::string& op) {
    int num = std::stoi(op);
    ++num;
    op = std::to_string(num);
}

void CPU::dec(std::string& op) {
    int num = std::stoi(op);
    --num;
    op = std::to_string(num);
}

void CPU::cmp(std::string& op1, const std::string& op2) {
    std::string tmp = op1;
    sub(op1, op2);
    if (std::stoi(op1) > 0) {
        eflags["SF"] = 1;
        eflags["ZF"] = 0;
    } else if (std::stoi(op1) == 0) {
        eflags["ZF"] = 1;
        eflags["SF"] = 0;
    } else {
        eflags["SF"] = 0;
        eflags["ZF"] = 0;
    }
    op1 = tmp;
}


void CPU::je(std::string& label) {
    if (eflags["ZF"] == 1) {
        label += ':';
        for (auto it = labels.begin(); it != labels.end(); ++it) {
            if (it -> first == label) {
                execute((it -> second + 1)); // the address of jmp next instruction
               inside_loops["inside je"] = true;
            }
        }
    }
}

void CPU::jne(std::string& label) {
    if (eflags["ZF"] == 0) {
        label += ':';
        for (auto it = labels.begin(); it != labels.end(); ++it) {
            if (it -> first == label) {
                execute(it -> second + 1);
               inside_loops["inside jne"] = true;
            }
        }
    }
}

void CPU::jg(std::string& label) {
    if (eflags["SF"] == 1) {
        label += ':';
        for (auto it = labels.begin(); it != labels.end(); ++it) {
            if (it -> first == label) {
                execute(it -> second + 1);
                inside_loops["inside jg"] = true;
            }
        }
    }
}

//checking for the operand: operands must be regiter register or register literal
void CPU::check_operands(std::string& op1, std::string& op2, void (CPU::*fptr)(std::string&, const std::string&)) {
    bool op1_is_register = false;
    for (auto it = registers.begin(); it != registers.end(); ++it) {
        const std::string& val  = it -> first;
        if (op1 == val) {
            op1_is_register = true;
        }

        if (op1_is_register) {
            break;
        }
    }

    if (!op1_is_register) {
        std::cout << "Left operand must not be l-value\n"; // must be handled then
        return;
    }

    bool op2_is_register = false;
    for (auto it = registers.begin(); it != registers.end(); ++it) {
        const std::string& val = it -> first;
        if (op2 == val) {
            op2_is_register = true;
        }

        if (op2_is_register) {
            break;
        }
    }

    if (op2_is_register) {
        std::string str1 = std::to_string(registers[op1]);
        std::string str2 = std::to_string(registers[op2]);
        if (fptr) {
            (this ->* fptr)(str1, str2); // calling the member function pointed to by fptr
        } else {
            std::cout << "Unknown instruction" << std::endl;
        }
        registers[op1] = std::stoi(str1);
    } else { // if literal
        std::string str1 = std::to_string(registers[op1]);
        if (fptr) {
            (this ->* fptr)(str1, op2);
        } else {
            std::cout << "Unknown instruction" << std::endl;
        }
        registers[op1] = std::stoi(str1);
    }
}

void CPU::execute(int i) {
    auto it = ram.find(i);
    for ( ; it != ram.end(); ++it) {
        std::cout << it -> first << " " << it -> second << " \n";
        const std::string& value = it -> second;

        size_t first_space = value.find(' ');
        size_t second_space = value.find(' ', first_space + 1);

        std::string instruction = value.substr(0, first_space);
        std::string op1 = value.substr(first_space + 1, second_space - first_space - 1);
        std::string op2 = value.substr(second_space + 1);
        if (instruction == instructions[0]) {
            check_operands(op1, op2, mov);
        }

        if (instruction == instructions[1]) {
            check_operands(op1, op2, add);
        }

        if (instruction == instructions[2]) {
            check_operands(op1, op2, sub);
        }

        if (instruction == instructions[3]) {
            check_operands(op1, op2, mul);
        }

        if (instruction == instructions[4]) {
            check_operands(op1, op2, div);
        }

        if (instruction == instructions[7]) {
            check_operands(op1, op2, cmp);
        }

        if (instruction == instructions[8]) {
            je(op1);
            if (inside_loops["inside je"] ) {
                break;
            }

            inside_loops["inside je"] = false;
        }

        if (instruction == instructions[9]) {
            jne(op1);
            if (inside_loops["inside jne"] ) {
                break;
            }

            inside_loops["inside jne"] = false;

        }

        if (instruction == instructions[12]) {
            jg(op1);
            if (inside_loops["inside jg"] ) {
                break;
            }
            inside_loops["inside jg"] = false;

        }

        std::cout << std::endl;
        std::cout << "Instruction: " << instruction << std::endl;
        std::cout << "Op1: " << op1 << std::endl;
        std::cout << "Op2: " << op2 << std::endl;
        std::cout << "left register value " << registers[op1] << std::endl;
        std::cout << std::endl;
    }
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
    eflags["ZF"] = 0;
    eflags["SF"] = 0;
}


