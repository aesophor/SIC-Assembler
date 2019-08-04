#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "fileprocessor.h"


char* read_n_chars(FileReader* self, int n)
{
    char* buffer = (char*)malloc((n+1) * sizeof(char));

    for(int i=0; i<n; i++)
    {
        buffer[i] = fgetc(self->fp);
    }
    buffer[n] = 0x00;
    return buffer;
}

char* read_line(FileReader* self)
{
    // Call malloc() here and free() later in order to
    // prevent memory leaks.
    char* line = (char*)malloc(LINE_SIZE * sizeof(char));
    fgets(line, LINE_SIZE, self->fp);
    return line;
}

char* trim_newline(char* line)
{
    // Strip off both 0xd(CR) and 0xa(LF).
    char* newline = strchr(line, 0xd);
    if(newline) { *newline = 0; }

    newline = strchr(line, 0xa);
    if(newline) { *newline = 0; }

    return line;
}

bool search_keyword(FileReader* self, char* keyword)
{
    char* line = (char*)malloc(LINE_SIZE * sizeof(char));
    int org_pos = ftell(self->fp);

    // Search for the keyword in the file line by line.
    rewind(self->fp);
    while(fgets(line, LINE_SIZE, self->fp) != NULL)
    {
        if(strstr(line, keyword))
        {
            fseek(self->fp, org_pos, SEEK_SET);
            return true;
        }
    }

    fseek(self->fp, org_pos, SEEK_SET);
    return false;
}

void fr_close(FileReader* self)
{
    fclose(self->fp);
    self->fp = NULL;
}


FileReader* new_FileReader(char* filepath)
{
    FileReader* self = (FileReader*)malloc(sizeof(FileReader));

    if((self->fp=fopen(filepath, "r")) == NULL)
    {
        printf("Failed to open file: %s\n", filepath);
        return NULL;
    }
    else
    {
        // Get file length first.
        fseek(self->fp, 0, SEEK_END);
        self->size = ftell(self->fp);
        rewind(self->fp);
    }

    return self;
}





bool write_data(FileWriter* self, char* line)
{
    if(fputs(line, self->fp) > 0) { return true; }
    return false;
}

void fw_close(FileWriter* self)
{
    fclose(self->fp);
    self->fp = NULL;
}


FileWriter* new_FileWriter(char* filepath)
{
    FileWriter* self = (FileWriter*)malloc(sizeof(FileWriter));

    if((self->fp=fopen(filepath, "w")) == NULL)
    {
        printf("Failed to create/open file: %s\n", filepath);
        return NULL;
    }
    return self;
}

