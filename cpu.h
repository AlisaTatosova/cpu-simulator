#ifndef CPU_H
#define CPU_H

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class CPU {
public:
    CPU();
    void mov(std::string& op1, const std::string& op2);
    void add(std::string& op1, const std::string& op2);
    void sub(std::string& op1, const std::string& op2);
    void mul(std::string& op1, const std::string& op2);
    void div(std::string& op1, const std::string& op2);
    void inc(std::string& op);
    void dec(std::string& op);

public:
    void cmp(std::string& op1, const std::string& op2) ;
    void je(std::string& label);
    void jne(std::string& label);
    void jg(std::string& label);
    void jge(std::string& label);
    void jl(std::string& label);

public:
    void fetch(std::ifstream& file);
    void execute(int i = 0);
    

private:
    std::map<int, std::string> ram;
    std::map<std::string, int> registers; 
    std::map<int, std::string> instructions;
    std::map<std::string, bool> eflags;
    std::map<std::string, bool> inside_loops;
    std::map<std::string, int> labels;
    int eip;

private:
    void init_flags();
    void name_instructions();
    void name_registers();
    void check_operands(std::string& op1, std::string& op2, void (CPU::*)(std::string&, const std::string&));
};
