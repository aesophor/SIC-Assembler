#include <stdbool.h>
#include "optab.h"
#include "object.h"

#ifndef PARSER_H
#define PARSER_H

#define PATH_OPTAB "./.optab.tab"
#define PATH_SYMTAB "./.symtab.tab"

#define BYTE_SIZE 1     /* The byte size of a SIC machine. */
#define WORD_SIZE 3     /* The word size of a SIC machine. */

#define OBJ_RECD_SIZE 6 /* The length of a single Object Text Record */


extern int LOCCTR;      /* location counter from sasm.c */
int PROGRAM_LEN;        /* location counter from sasm.c */
int START_ADDR;


char* _prepend_addr(char* line, int addr);
char* _append_addr(char* line, int addr);
void _writeline_with_addr(FileWriter* f, char* line, size_t size);
void _write_last_line(FileWriter* f, char* line);
void _destroy_instruction(char* line_p, Instruction* i);
int process_first_pass(FileReader* source_f);


char* _get_prepended_addr(FileReader* intermediate);
char* _get_symbol_addr(FileReader* symtab_f, char* symbol);
void _destroy_instc_cache(char* current_addr, char* line_p, Instruction* i);
void _destroy_obj_cache(char* objcode, char* mnemonic, char* operand);
int process_second_pass(FileReader* intermediate_f);

#endif
