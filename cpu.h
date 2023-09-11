#ifndef CPU_H
#define CPU_h

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class CPU {
public:
    CPU();
    void mov(std::string& op1, const std::string& op2);
    void add(std::string& op1, const std::string& op2);
    void sub(std::string& op1, const std::string& op2);
    void mul(std::string& op1, const std::string& op2);
    void div(std::string& op1, const std::string& op2);
    void inc(std::string& op1, const std::string& op2 = "");
    void dec(std::string& op1, const std::string& op2 = "");

public:
    void cmp(std::string& op1, const std::string& op2) ;
    void je(std::string& label);
    void jne(std::string& label);
    void jg(std::string& label);
    void jge(std::string& label);
    void jl(std::string& label);
    void jle(std::string& label);

    class InvalidOperandException : public std::exception {         
    private:
        std::string message;

    public:
        InvalidOperandException(const std::string& operand)
            : message("Invalid operand: " + operand) {}

        const char* what() const noexcept override {
            return message.c_str();
        }
    };

public:
    void fetch(std::ifstream& file);
    void execute(int i = 0);
    void print(std::string& op1, const std::string& op2 = "");
    
private:
    std::map<int, std::string> ram;
    std::map<std::string, int> registers; 
    std::map<int, std::string> instructions;
    std::map<std::string, char> eflags;
    std::map<std::string, int> labels;
    std::map<std::string, bool> inside_loops;
    int eip;

private:
    void init_flags();
    void name_instructions();
    void name_registers();
    bool is_number(const std::string& str);
    void remove_comma(std::string& op);
    bool has_comma(std::string& op);
    void check_operands(std::string& op1, std::string& op2, void (CPU::*fptr)(std::string&, const std::string&));
};

#endif
