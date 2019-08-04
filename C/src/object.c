#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "object.h"


char* get_header_recd(ObjectProgram* self) { return self->header_recd; }
char* get_text_recd(ObjectProgram* self)   { return self->text_recd; }
char* get_end_recd(ObjectProgram* self)    { return self->end_recd; }


void init_header_recd(ObjectProgram* self, char* prog_name, char* starting_addr, int prog_len)
{
    // Allocate memory for Header Record, and also clear the memory.
    self->header_recd = (char*)malloc(HEADER_RECD_TOTAL * sizeof(char));
    memset(self->header_recd, 0x00, HEADER_RECD_TOTAL);

    // Write Header Record indicator "H".
    strcat(self->header_recd, "H");

    // Write program name, and append whitespace to it,
    // making the total length of header_recd six characters.
    strcat(self->header_recd, prog_name);
    for(int i=0; i<RECD_SIZE-strlen(prog_name); i++) { strcat(self->header_recd, " "); }

    // Write starting addr, and also preserve it.
    for(int i=0; i<RECD_SIZE-strlen(starting_addr); i++) { strcat(self->header_recd, "0"); }
    strcat(self->header_recd, starting_addr);

    self->starting_addr = (char*)malloc(4 * sizeof(char));
    self->starting_addr[0] = 0x00;
    strcpy(self->starting_addr, starting_addr);

    // Write the total length of the program in Header Record.
    sprintf(&self->header_recd[1+RECD_SIZE+RECD_SIZE], "%06x", prog_len);
}


void init_text_record(ObjectProgram* self, char* current_addr)
{
    // T(1) | START_ADDR(6) | RECD_SIZE(2) | RECDS(6, UP TO 10 RECORDS)
    self->text_recd = (char*)malloc(TEXT_RECD_TOTAL * sizeof(char));
    memset(self->text_recd, 0x00, TEXT_RECD_TOTAL);

    // Write Text Record indicator "T".
    strcat(self->text_recd, "T");

    // Write starting addr.
    for(int i=0; i<RECD_SIZE-strlen(current_addr); i++) { strcat(self->text_recd, "0"); }
    strcat(self->text_recd, current_addr);

    // Initialize record length with zero.
    // The acutal length will be filled in here later.
    strcat(self->text_recd, "00");

    // Calculate the maximum address for current Text Record
    // in *BYTE*.
    self->max_addr_threshold = strtol(current_addr, NULL, 16);
    self->max_addr_threshold += (TEXT_RECD_CONTENT/2);
}

void init_new_text_recd(ObjectProgram* self, char* current_addr)
{
    // Reset the length of "Current Text Record".
    self->current_text_recd_size = 0;

    // Initialize new Text Record.
    init_text_record(self, current_addr);
}


void _update_text_recd_length(ObjectProgram* self, int update_byte_size)
{
    char tmp_buf[2];

    // Update Text record size.
    self->current_text_recd_size += update_byte_size;
    sprintf(tmp_buf, "%02x", self->current_text_recd_size);
    memcpy(&self->text_recd[1+RECD_SIZE], tmp_buf, strlen(tmp_buf));
}

void append_text_record(ObjectProgram* self, char* new_recd, int update_byte_size)
{    
    // Append acutal record at the end.
    strcat(self->text_recd, new_recd);

    // If the mnemonic is a RESB or RESW, do NOT update length!
    _update_text_recd_length(self, update_byte_size);
}


void init_end_recd(ObjectProgram* self)
{
    // Write the End Record Indicator.
    self->end_recd = (char*)malloc(END_RECD_TOTAL * sizeof(char));
    self->end_recd[0] = 0x00;

    // Write Header Record indicator "E".
    strcat(self->end_recd, "E");

    // Write starting addr.
    for(int i=0; i<RECD_SIZE-strlen(self->starting_addr); i++) { strcat(self->end_recd, "0"); }
    strcat(self->end_recd, self->starting_addr);

    // Free self->starting_addr which was malloc()-ed earlier.
    free(self->starting_addr);
    self->starting_addr = NULL;
}


bool _check_text_recd_size(ObjectProgram* self, char* current_addr, int update_byte_size)
{
    // Compare current_addr with self->max_addr_threshold.
    int next_addr = strtol(current_addr, NULL, 16) + update_byte_size;
    
    if(next_addr <= self->max_addr_threshold) { return true; }
    return false;
}

bool will_record_fit(ObjectProgram* self, char* current_addr, int update_byte_size)
{
    if(_check_text_recd_size(self, current_addr, update_byte_size)) { return true; }
    return false;
}


void delete_ObjectProgram(ObjectProgram* self)
{
    free(self->header_recd);
    free(self->text_recd);
    free(self->end_recd);
    self->header_recd = NULL;
    self->text_recd = NULL;
    self->end_recd = NULL;
}

ObjectProgram* new_ObjectProgram()
{
    ObjectProgram* self = (ObjectProgram*)malloc(sizeof(ObjectProgram));
    return self;
}
