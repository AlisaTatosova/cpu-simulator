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
    CPU();
    char get_flag(const std::string& flag); 
    void set_flag(const std::string& flag, char code); 
    bool is_register(const std::string& op); // checking is operand register or not
    std::map<std::string, int> get_labels() const; // keeps labels with their addresses

public:
    void fetch(std::ifstream& file); //reads from file and saves the containment in map: map<int, string> ram, where key is address of instruction, value is instruction 
    void execute(int i = 0); // executes instructions
    void print(const std::string& op1);

public:
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
    void name_instructions(); // initialize instructions
    void name_registers(); // initialize registers
    void init_flags(); // initalize flags
    void check_operands(std::string& op1, std::string& op2, void (ALU::*fptr)(std::string&, const std::string&)); //checking for the operand: operands must be regiter register or register literal
    bool is_number(const std::string& str); // checking if string is number
    void remove_comma(std::string& op); // removes comma
    bool has_comma(const std::string& op); // checking if there is comma
};

#endif