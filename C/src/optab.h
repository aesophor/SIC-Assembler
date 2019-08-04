#include <stdbool.h>
#include "fileprocessor.h"


#ifndef OPTAB_H
#define OPTAB_H

#define OPTAB_REC_SIZE 16
#define OPTAB_REC_COLS 4
#define OPTAB_SIZE 64

typedef struct optab_record_t
{
    char  line[OPTAB_REC_SIZE];              /* copy the original line here. */
    char* records[OPTAB_REC_COLS];
} OptabRecord;

char* get_mnemonic(OptabRecord* self);
char* get_operands(OptabRecord* self);
char* get_format(OptabRecord* self);
char* get_opcode(OptabRecord* self);

void  _set_recdline(OptabRecord* self, char* line);

void _parse_record(OptabRecord* self);
void show_record(OptabRecord* self);

OptabRecord* new_OptabRecord(char* line);


typedef struct optab_t
{
    OptabRecord* optab_records[OPTAB_SIZE];
} Optab;

OptabRecord* get_optab_record(Optab* self, int i);
void _set_optab_record(Optab* self, int i, OptabRecord* record);

void _parse_records(Optab* self, FileReader* optab_file);
void show_optab(Optab* self);
OptabRecord* get_record(Optab* self, char* mnemonic);

Optab* new_Optab(FileReader* optab_file);

#endif
