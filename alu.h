#ifndef CPU_H
#define CPU_h

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class CPU;

class ALU {
public:
    ALU(CPU& cpu);

    void mov(std::string& op1, const std::string& op2);
    void add(std::string& op1, const std::string& op2);
    void sub(std::string& op1, const std::string& op2);
    void mul(std::string& op1, const std::string& op2);
    void div(std::string& op1, const std::string& op2);
    void inc(std::string& op1, const std::string& op2 = "");
    void dec(std::string& op1, const std::string& op2 = "");
    void cmp(std::string& op1, const std::string& op2);

public:
    bool je(std::string& label);
    bool jne(std::string& label);
    bool jg(std::string& label);
    bool jge(std::string& label);
    bool jl(std::string& label);
    bool jle(std::string& label);
    
private:
    CPU& cpu;
};

#endif