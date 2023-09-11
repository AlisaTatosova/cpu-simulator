#ifndef CPU_H
#define CPU_h

#include "alu.h"

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class CPU {
public:   
    char get_flag(const std::string& flag);
    void set_flag(const std::string& flag, char code);
    bool is_register(const std::string& op);
    std::map<std::string, int> get_labels() const;

public:
    void fetch(std::ifstream& file);
    void execute(int i = 0);
    void print(const std::string& op1);

public:
    CPU();

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

private:
    ALU alu;
    std::map<int, std::string> ram;
    std::map<std::string, int> registers; 
    std::map<int, std::string> instructions;
    std::map<std::string, bool> eflags;
    std::map<std::string, int> labels;
    int eip;

private:
    //helpers
    void name_instructions();
    void name_registers();
    void init_flags();
    void check_operands(std::string& op1, std::string& op2, void (ALU::*fptr)(std::string&, const std::string&));
    bool is_number(const std::string& str);
    void remove_comma(std::string& op);
    bool has_comma(const std::string& op);
};


#endif