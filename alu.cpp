#include "cpu.h"

ALU::ALU(CPU& cpu) : cpu{cpu} {}

void ALU::mov(std::string& op1, const std::string& op2) {
    int num1 = std::stoi(op1);
    int num2 = std::stoi(op2);
    op1 = std::to_string(num2);
}

void ALU::add(std::string& op1, const std::string& op2) {
    int num1 = std::stoi(op1);
    int num2 = std::stoi(op2);
    num1 += num2;
    op1 = std::to_string(num1);
}

void ALU::sub(std::string& op1, const std::string& op2) {
    int num1 = std::stoi(op1);
    int num2 = std::stoi(op2);
    num1 -= num2;
    op1 = std::to_string(num1);
}

void ALU::mul(std::string& op1, const std::string& op2) {
    int num1 = std::stoi(op1);
    int num2 = std::stoi(op2);
    num1 *= num2;
    op1 = std::to_string(num1);
}

void ALU::div(std::string& op1, const std::string& op2) {
    int num1 = std::stoi(op1);
    int num2 = std::stoi(op2);
    num1 /= num2;
    op1 = std::to_string(num1);
}

void ALU::inc(std::string& op1, const std::string& op2) {
    int num = std::stoi(op1);
    ++num;
    op1 = std::to_string(num);
}

void ALU::dec(std::string& op1, const std::string& op2) {
    int num = std::stoi(op1);
    --num;
    op1 = std::to_string(num);
}

void ALU::cmp(std::string& op1, const std::string& op2) {
    std::string tmp = op1; 
    sub(op1, op2);
    if (std::stoi(op1) > 0) {
        cpu.set_flag("UF", 1);
        cpu.set_flag("SF", 0);
        cpu.set_flag("ZF", 0);
    } else if (std::stoi(op1) == 0) {
        cpu.set_flag("ZF", 1);
        cpu.set_flag("SF", 0);
        cpu.set_flag("UF", 0);
    } else if (std::stoi(op1) < 0) {
        cpu.set_flag("SF", 1);
        cpu.set_flag("UF", 0);
        cpu.set_flag("ZF", 0);
    }
    op1 = tmp;
}

bool ALU::je(std::string& label) {
    if (cpu.get_flag("ZF") == 1) {
        label += ':';
        for (auto it = cpu.get_labels().begin(); it != cpu.get_labels().end(); ++it) {
            if (it -> first == label) {
               cpu.execute(it -> second + 1);
               return true;
            }
        }
    } 
    return false;
}

bool ALU::jne(std::string& label) {
    if (cpu.get_flag("ZF") == 0) {
        label += ':';
        for (auto it = cpu.get_labels().begin(); it != cpu.get_labels().end(); ++it) {
            if (it -> first == label) {
               cpu.execute(it -> second + 1); // the address of jmp next instruction
               return true;
            }
        }
    }
    return false;
}

bool ALU::jg(std::string& label) {
    if (cpu.get_flag("UF") == 1) {
        label += ':';
        for (auto it = cpu.get_labels().begin(); it != cpu.get_labels().end(); ++it) {
            if (it -> first == label) {
                cpu.execute(it -> second + 1);
                return true;
            }
        }
    }
    return false;
}

bool ALU::jge(std::string& label) {
    if (cpu.get_flag("UF") == 1 || cpu.get_flag("ZF") == 1) {
        label += ':';
        for (auto it = cpu.get_labels().begin(); it != cpu.get_labels().end(); ++it) {
            if (it -> first == label) {
                cpu.execute(it -> second + 1);
                return true;
            }
        }
    }
    return false;
}

bool ALU::jl(std::string& label) {
    if (cpu.get_flag("SF") == 1) {
        label += ':';
        for (auto it = cpu.get_labels().begin(); it != cpu.get_labels().end(); ++it) {
            if (it -> first == label) {
                cpu.execute(it -> second + 1);
                return true;
            }
        }
    }
    return false;
}

bool ALU::jle(std::string& label) {
    if (cpu.get_flag("SF")== 1 || cpu.get_flag("ZF") == 1) {
        label += ':';
        for (auto it = cpu.get_labels().begin(); it != cpu.get_labels().end(); ++it) {
            if (it -> first == label) {
                cpu.execute(it -> second + 1);
                return true;
            }
        }
    }
    return false;
}

