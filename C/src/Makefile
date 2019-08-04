# Makefile for SASM (SIC Assembler).

prog_name="sasm"
bin_dir="../bin/"

$(prog_name): sasm.c parser.c fileprocessor.c instruction.c object.c optab.c
	gcc -o $(bin_dir)sasm sasm.c parser.c fileprocessor.c instruction.c object.c optab.c
	cp .optab.tab $(bin_dir)
	cp copy.asm $(bin_dir)
