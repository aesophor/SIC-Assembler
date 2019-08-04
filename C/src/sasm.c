#include <stdio.h>
#include <stdlib.h>
#include "sasm.h"

#define PATH_INTERMDT "./.intermediate"
#define WORD_SIZE 3

int LOCCTR = 0;

void print_info()
{
    printf("SASM Assembler for SIC. Version 0.8\n");
    printf("Created by @aesophor 2017\n\n");
}

int main(int argc, char** args)
{
    print_info();

    if(argc < 2)
    {
        printf("usage: %s <assembly-src>\n", args[0]);
        return EXIT_FAILURE;
    }
    else
    {        
        // Perform First Pass using Source Program file.
        FileReader* asm_src_f = new_FileReader(args[1]);
        if(!asm_src_f) { return EXIT_FAILURE; }
        process_first_pass(asm_src_f);
        fr_close(asm_src_f);
        free(asm_src_f);
        asm_src_f = NULL;
        
        
        printf(" Assembling ... %s\n", args[1]);
        
        // Perform Second Pass using Intermediate file.
        FileReader* intermediate_f = new_FileReader(PATH_INTERMDT);
        process_second_pass(intermediate_f);
        fr_close(intermediate_f);
        free(intermediate_f);
        intermediate_f = NULL;
        
        return EXIT_SUCCESS;
    }
}
