#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "optab.h"


char* get_mnemonic(OptabRecord* self) { return self->records[0]; }
char* get_operands(OptabRecord* self) { return self->records[1]; }
char* get_format(OptabRecord* self)   { return self->records[2]; }
char* get_opcode(OptabRecord* self)   { return self->records[3]; }

void  _set_recdline(OptabRecord* self, char* line) { strcpy(self->line, line); }


void _parse_record(OptabRecord* self)
{
    const char delim[10] = " \t";
    char* token;

    token = strtok(self->line, delim);
    for(int i=0; i<OPTAB_REC_COLS; i++)
    {
        self->records[i] = token;
        token = strtok(NULL, delim);
    }
}

void show_record(OptabRecord* self)
{
    for(int i=0; i<OPTAB_REC_COLS; i++)
    {
        printf("[%s] ", self->records[i]);
    }
    printf("\n");
}

OptabRecord* new_OptabRecord(char* line)
{
    OptabRecord* optab_r = (OptabRecord*)malloc(sizeof(OptabRecord));
    _set_recdline(optab_r, line);
    _parse_record(optab_r);
    return optab_r;
}





OptabRecord* get_optab_record(Optab* self, int i) { return self->optab_records[i]; }
void _set_optab_record(Optab* self, int i, OptabRecord* record) { self->optab_records[i] = record; }

void _parse_records(Optab* self, FileReader* optab_file)
{
    int index = 0;
    char line[128];
   
    while(fgets(line, sizeof(line), optab_file->fp) != NULL)
    {
        // generate a record, auto-parse,
        // and save the record at the correct position in optab_records[].
        OptabRecord* optab_r = new_OptabRecord(trim_newline(line));
        _set_optab_record(self, index, optab_r);
        index++;
    }
}

void show_optab(Optab* self)
{
    int index = 0;

    printf("Showing operation code table (OPTAB):\n");
    do 
    {
        show_record(self->optab_records[index]);
        index++;
    } while(self->optab_records[index] != NULL);
    printf("\n");
}

OptabRecord* get_record(Optab* self, char* mnemonic)
{
    OptabRecord* current_recd = self->optab_records[0];
    for(int i=0; current_recd; i++)
    {
        if(!strcmp(get_mnemonic(current_recd), mnemonic))
        {
            return current_recd;
        }
        current_recd = self->optab_records[i];
    }
    return NULL;
}


Optab* new_Optab(FileReader* optab_file)
{
    Optab* optab = (Optab*)malloc(sizeof(Optab));
    _parse_records(optab, optab_file);
    return optab;
}
