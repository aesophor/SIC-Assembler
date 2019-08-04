#ifndef OBJECT_H
#define OBJECT_H

#define RECD_SIZE           6

#define HEADER_RECD_HEAD    1
#define HEADER_RECD_CONTENT 3*RECD_SIZE
#define HEADER_RECD_TOTAL   HEADER_RECD_HEAD + HEADER_RECD_CONTENT

#define TEXT_RECD_HEAD      1+RECD_SIZE+2
#define TEXT_RECD_CONTENT   10*RECD_SIZE
#define TEXT_RECD_TOTAL     TEXT_RECD_HEAD + TEXT_RECD_CONTENT

#define END_RECD_TOTAL      1+RECD_SIZE


typedef struct object_t
{
    char* header_recd;
    char* text_recd;
    char* end_recd;
    char* starting_addr;
    int   current_text_recd_size;  /* Number of hex digit. */
    int   max_addr_threshold;      /* The maximum address of current record. */
} ObjectProgram;

char* get_header_recd(ObjectProgram* self);
char* get_text_recd(ObjectProgram* self);
char* get_end_recd(ObjectProgram* self);

void init_header_recd(ObjectProgram* self, char* prog_name, char* starting_addr, int prog_len);

void init_text_record(ObjectProgram* self, char* current_addr);
void init_new_text_recd(ObjectProgram* self, char* current_addr);

void _update_text_recd_length(ObjectProgram* self, int update_byte_size);
void append_text_record(ObjectProgram* self, char* new_recd, int update_byte_size);

void init_end_recd(ObjectProgram* self);

bool _check_text_recd_size(ObjectProgram* self, char* current_addr, int update_byte_size);
bool will_record_fit(ObjectProgram* self, char* current_addr, int update_byte_size);

void delete_ObjectProgram(ObjectProgram* self);
ObjectProgram* new_ObjectProgram();

#endif
