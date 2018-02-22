# SIC-Assembler
My assignment. An assembler for SIC (Simplified Instructional Computer) written in Java.

# Description
 - This assembler is implemented using the algorithm in " System Software: An Introduction to Systems Programming" by Leland L. Beck.
 - Assemble any program "for SIC" (not SIC/XE) written in assembly language with the source code extension ".asm".
 - OPTAB and SYMTAB
    - OperationTable can be located at tables/OperationTable. It is loaded from this file.
    - SymbolTable uses HashMap, so no file is required.
 - It will produce:
    - an intermediate file (.tmp)
    - an object file (*.obj)

# Try out
A jar is ready and can be found under dist/
```
git clone https://github.com/aesophor/SIC-Assembler.git
java -jar dist/Assembler.jar copy.asm
```
