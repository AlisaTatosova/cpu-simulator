#include "cpu.h"
#include <iostream>

int main() {
    CPU cpu;
    std::ifstream file("assembly.txt");
    cpu.fetch(file);
    cpu.execute();
    return 0;
}
~                 
