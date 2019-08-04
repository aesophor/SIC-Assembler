#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "instruction.h"


char* get_label(Instruction* self) { return self->tokens[0]; }
char* get_instc(Instruction* self) { return self->tokens[1]; }
char* get_opfir(Instruction* self) { return self->tokens[2]; }
char* get_opsec(Instruction* self) { return self->tokens[3]; }

void  _set_label(Instruction* self, char* label) { self->tokens[0] = label; } 
void  _set_instc(Instruction* self, char* instc) { self->tokens[1] = instc; } 
void  _set_opfir(Instruction* self, char* opfir) { self->tokens[2] = opfir; } 
void  _set_opsec(Instruction* self, char* opsec) { self->tokens[3] = opsec; }

void  _set_line(Instruction* self, char* line) { strcpy(self->line, line); }
void  _set_line_original(Instruction* self) { strcpy(self->line_original, self->line); }


void _extract_second_operand(Instruction* self)
{
    // Attempt to get second operand.
    if(strstr(get_opfir(self), ","))
    {
        char* token = strtok(get_opfir(self), ",");
        token = strtok(NULL, ",");
        _set_opsec(self, token);
    }
}

void _extract_tokens(Instruction* self)
{
    // Split the instruction into tokens here,
    // and save each token to their corresponding buffer.
    char* token = strtok(self->line, " \t");
    for(int i=0; token; i++)
    {
        self->tokens[i] = token;
        token = strtok(NULL, " \t");
    }
    
    // Also check if second operand exists.
    token = strtok(NULL, ",");
    self->tokens[3] = token;

    _extract_second_operand(self);
}

bool _search_token(Instruction* self, char* keyword)
{
    for(int i=0; self->tokens[i]; i++)
    {
        if(!strcmp(self->tokens[i], keyword)) { return true; }
    }
    return false;
}


void show_tokens(Instruction* self)
{
    for(int i=0; self->tokens[i]; i++)
    {
        printf("[%s] ", self->tokens[i]);
    }
    printf("\n");
}

bool is_index_addr(Instruction* self)
{
    // Check if it is using index addressing mode
    // by comparing its second operand with "X".
    if(!strcmp(get_opsec(self), "X")) { return true; }
    return false;
}

char* has_the_instruction(Instruction* self, char* keyword)
{
    if(!strcmp(get_instc(self), keyword)) { return keyword; }
    return NULL;
}

long int get_start_addr(Instruction* self)
{
    // Remember we have to treat the readed string as hex value,
    // for this we are going to use strtol().
    return strtol(get_opfir(self), NULL, 16);
}

char* parse_byte_op(Instruction* self)
{
    char* operand = get_opfir(self);
    char* token = strtok(operand, "\'");
    token = strtok(NULL, "\'");
    return token;
}

bool is_comment(Instruction* self)
{
    // Check if the first character is a dot(.)
    char comment_symbol = '.';
    if(self->line_original[0] == comment_symbol) { return true; }
    return false;
}


Instruction* new_Instruction(char* line)
{
    Instruction* self = (Instruction*)malloc(sizeof(Instruction));
    self->tokens[3] = NULL;
    _set_line(self, line);
    _set_line_original(self);
    if(!is_comment(self)) { _extract_tokens(self); }
    return self;
}
