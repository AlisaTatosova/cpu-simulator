# Simple CPU Simulator
The Simple CPU Simulator is a basic CPU emulation program written in C++. This emulator can execute a set of instructions and perform arithmetic and logical operations. It also includes an ALU (Arithmetic Logic Unit) for handling these operations.

# Instruction Set
The emulator supports a basic instruction set, including instructions like mov, add, sub, mul, div, inc, dec, and cmp. You can refer to the code or the input file for specific instructions and their usage.

# CPU Features
Registers
The CPU emulator includes a set of registers(r1 to r10) for storing data during execution. You can use these registers in your instructions.

# Flags
The CPU emulator also supports flags(ZF, SF, UF) to indicate the result of the most recent operation. These flags can be checked and manipulated in your instructions.
SF - if operand is < 0, SF = 1
UF - if operand is > 0, UF = 1
ZF - if operand is = 0, ZF = 1

# ALU Features
The ALU (Arithmetic Logic Unit) handles arithmetic and logical operations. It supports operations like mov, add, sub, mul, div, inc, dec, cmp, and conditional jumps (je, jne, jg, jge, jl, jle).

# ALU Instructions
Here are the supported ALU instructions:

mov: Move data from one location to another.
add: Add two values.
sub: Subtract two values.
mul: Multiply two values.
div: Divide two values.
inc: Increment a value.
dec: Decrement a value.
cmp: Compare two values.

# ALU Usage
You can use these ALU instructions in your input file to perform arithmetic and logical operations. For example:
mov r1, 10
mov r2, 20
add r1, r2
In this example, we move the values 10 and 20 into registers r1 and r2, respectively, and then add them together, storing the result in r1.
