# SIC-Assembler
A simple assembler for SIC (Simplified Instructional Computer) written in C.

# Description
 - This assembler is implemented using the algorithm in " System Software: An Introduction to Systems Programming" by Leland L. Beck.
 - Assemble any program "for SIC" (not SIC/XE) written in assembly language with the source code extension ".asm".
 - It will produce the following:
    - an intermediate file (.intermediate)
    - a SYMTAB file (.symtab.tab)
    - an object file (*.obj)


# Try out
Compile the program via the Makefile under src/
```
git clone https://github.com/aesophor/SIC-Assembler.git
cd SIC-Assembler/src
make
```
and then you could use sasm to assemble any assembly program written specifically for SIC.
```
cd ../bin
./sasm <prog>.asm
```
The object code will be written into <prog>.obj
```
cat <prog>.obj
```
