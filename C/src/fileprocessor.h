#include <stdbool.h>
#include "instruction.h"


#ifndef FILE_PROC_H
#define FILE_PROC_H

#define LINE_SIZE 64

typedef struct file_reader_t
{
    FILE* fp;
    int size;
} FileReader;

char* read_n_chars(FileReader* self, int n);
char* read_line(FileReader* self);
char* trim_newline(char* line);
bool search_keyword(FileReader* self, char* keyword);
void fr_close(FileReader* self);

FileReader* new_FileReader(char* filepath);


typedef struct file_writer_t
{
    FILE* fp;
    int size;
} FileWriter;

bool write_data(FileWriter* self, char* line);
void fw_close(FileWriter* self);

FileWriter* new_FileWriter(char* filepath);

#endif
