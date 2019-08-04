#ifndef INSTC_H
#define INSTC_H

#define LINE_SIZE 64
#define TOKEN_PARTS 5

typedef struct instruction_t
{
    char  line[LINE_SIZE];
    char  line_original[LINE_SIZE];
    char* tokens[TOKEN_PARTS];
} Instruction;

char* get_label(Instruction* self);
char* get_instc(Instruction* self);
char* get_opfir(Instruction* self);
char* get_opsec(Instruction* self);

void  _set_label(Instruction* self, char* label);
void  _set_instc(Instruction* self, char* instc);
void  _set_opfir(Instruction* self, char* opfir);
void  _set_opsec(Instruction* self, char* opsec);

void  _set_line(Instruction* self, char* line);
void  _set_line_original(Instruction* self);


void _extract_second_operand(Instruction* self);
void _extract_tokens(Instruction* self);
bool _search_token(Instruction* self, char* keyword);

void show_tokens(Instruction* self);
bool is_index_addr(Instruction* self);
char* has_the_instruction(Instruction* self, char* keyword);
long int get_start_addr(Instruction* self);
char* parse_byte_op(Instruction* self);
bool is_comment(Instruction* self);

Instruction* new_Instruction(char* line);

#endif
