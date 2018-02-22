# SIC-Assembler
My assignment. An assembler for SIC (Simplified Instructional Computer) written in Java.

# Description
 - This assembler is implemented using the algorithm in " System Software: An Introduction to Systems Programming" by Leland L. Beck.
 - Assemble any program "for SIC" (not SIC/XE) written in assembly language with the source code extension ".asm".
 - It requires:
    - OperationTable (tables/OperationTable)
    - SymbolTable uses HashMap.
 - It will produce:
    - an intermediate file (.tmp)
    - an object file (*.obj)

# Try out
A jar is ready inside the dist/ directory.
```
git clone https://github.com/aesophor/SIC-Assembler.git
java -jar dist/Assembler.jar copy.asm
```
