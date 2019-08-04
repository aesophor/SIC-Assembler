#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"


char* _prepend_addr(char* line, int addr)
{
    char* result_line = (char*)malloc(LINE_SIZE * sizeof(char));
    sprintf(result_line, "%4x", addr);
    strcat(result_line, "\t");
    strcat(result_line, line);
    return result_line;
}

char* _append_addr(char* line, int addr)
{
    char* result_line = (char*)malloc(LINE_SIZE * sizeof(char));
    strcpy(result_line, line);
    strcat(result_line, "\t");
    sprintf(&result_line[strlen(result_line)], "%4x", addr);
    return result_line;
}

void _writeline_with_addr(FileWriter* f, char* line, size_t size)
{
    char* line_with_addr = _prepend_addr(line, LOCCTR);
    write_data(f, line_with_addr);
    write_data(f, "\n");
    LOCCTR += size;

    free(line_with_addr);
    line_with_addr = NULL;
}

void _write_last_line(FileWriter* f, char* line)
{
    // Prepend a tab to the last line but without address.
    char* result_line = (char*)malloc(sizeof(char));
    result_line = strcat(result_line, "    ");
    result_line = strcat(result_line, line);
    
    // And then write it to the file.
    write_data(f, result_line);
    write_data(f, "\n");

    free(result_line);
    result_line = NULL;
}

void _destroy_instruction(char* line_p, Instruction* i)
{
    // This function is intended to perform free() on
    // both readed line and the instruction object.
    free(line_p);
    free(i);
}

int process_first_pass(FileReader* source_f)
{
    // The pointer that holds the address of char pointer
    // which is returned by read_line().
    char* line_p = NULL;

    // Load OPTAB from PATH_OPTAB.
    FileReader* optab_f = new_FileReader(PATH_OPTAB);
    Optab* optab = new_Optab(optab_f); 

    // Prepare SYMTAB.
    FileWriter* symtab_f_w = new_FileWriter(PATH_SYMTAB);
    FileReader* symtab_f_r = new_FileReader(PATH_SYMTAB);

    // Read first input line, and copy it to i->line_original,
    // since i->line will be torn apart in _extract_token() and _search_token().
    // See instruction.c for further detail.
    FileWriter* intermediate_f = new_FileWriter("./.intermediate");
    //write_data(intermediate, "\r"); /* workaround for the intermediate file glitch */

    line_p = read_line(source_f);
    Instruction* i = new_Instruction(trim_newline(line_p));

    if(_search_token(i, "START"))
    {
        // Initialize LOCCTR to starting address.
        // And then save the LOCCTR value in START_ADDR.
        LOCCTR = get_start_addr(i);
        START_ADDR = LOCCTR;
        
        // Prepend address and write the first line to intermediate file.
        // note that we should not increment LOCCTR on the first line.
        _writeline_with_addr(intermediate_f, i->line_original, WORD_SIZE);
        LOCCTR -= WORD_SIZE;
        
        // Free current instruction.
        _destroy_instruction(line_p, i);
        
        // read next input line.
        line_p = read_line(source_f);
        i = new_Instruction(trim_newline(line_p));
    }
    else
    {
        // Initialize LOCCTR to 0.
        LOCCTR = 0;
    }


    // Perform parsing starting from line 2 of assembly source code.
    size_t assign_size = 0;
    OptabRecord* record_p = NULL;
    while(!has_the_instruction(i, "END"))
    {
        if(!is_comment(i))
        {
            char* label_p = NULL;
            if(strcmp((label_p=get_label(i)), "-"))
            {
                if(search_keyword(symtab_f_r, label_p))
                {
                    // Set error flag (duplicate symbol)
                    printf("Error: Duplicate symbol (%s).\n", label_p);
                    return EXIT_FAILURE;
                }
                else
                {
                    // Insert (LABEL,LOCCTR) into SYMTAB
                    // TODO: better insertion and SYMTAB parsing.
                    write_data(symtab_f_w, _append_addr(label_p, LOCCTR));
                    write_data(symtab_f_w, "\n");
                }
            }
            
            if(record_p=get_record(optab, get_instc(i)))
            {
                assign_size = WORD_SIZE;
            }
            else if(has_the_instruction(i, "WORD"))
            {
                assign_size = WORD_SIZE;
            }
            else if(has_the_instruction(i, "RESW"))
            {
                assign_size = (WORD_SIZE * strtol(get_opfir(i), NULL, 10));
            }
            else if(has_the_instruction(i, "RESB"))
            {
                assign_size = (BYTE_SIZE * strtol(get_opfir(i), NULL, 10));
            }
            else if(has_the_instruction(i, "BYTE"))
            {
                /* Check the first chracter of the opfir.
                 * c: character (1 byte per character)
                 * x: hex (1 byte every 2 digits)
                 */
                char data_type = get_opfir(i)[0];
                int  data_len = strlen(parse_byte_op(i));
                switch(data_type)
                {
                    case 'C':
                        assign_size = BYTE_SIZE * data_len;
                        break;
                    case 'X':
                        if(data_len%2 != 0)
                        {
                            assign_size = BYTE_SIZE * 0.5 * (data_len+1);
                        }
                        else
                        {
                            assign_size = BYTE_SIZE * 0.5 * (data_len+1);
                        }
                        break;
                    default:
                        {
                            assign_size = 0;
                        }
                        break;
                }
            }
            else
            {
                printf("Error: Invalid operation code (%s)\n", get_opcode(record_p));
                return EXIT_FAILURE;
            }
            // Prepend address and write line to intermediate file.
            _writeline_with_addr(intermediate_f, i->line_original, assign_size);
        }
        // Free current instruction.
        _destroy_instruction(line_p, i);
        
        // Read next input line.
        line_p = read_line(source_f);
        i = new_Instruction(trim_newline(line_p));
    }
    // Write the last line and cliose the file.
    _write_last_line(intermediate_f, i->line_original);

    // Free current instruction.
    _destroy_instruction(line_p, i);

    // Save program length to a runtime variable,
    // since it will soon be employed later in the Second Pass.
    PROGRAM_LEN = LOCCTR - START_ADDR;

    fw_close(intermediate_f);
    free(intermediate_f);
    intermediate_f = NULL;

    fr_close(optab_f);
    free(optab_f);
    optab_f = NULL;

    fw_close(symtab_f_w);
    free(symtab_f_w);
    symtab_f_w = NULL;

    fr_close(symtab_f_r);
    free(symtab_f_r);
    symtab_f_r = NULL;

    free(optab);
    optab = NULL;

    // TODO: free optab.
    return EXIT_SUCCESS;
}





char* _get_prepended_addr(FileReader* intermediate)
{
    char* chars_buf = (char*)malloc(5 * sizeof(char));
    chars_buf = read_n_chars(intermediate, 4);
    chars_buf[strlen(chars_buf)] = 0x00;
    return chars_buf;
}

char* _get_symbol_addr(FileReader* symtab_f, char* symbol)
{
    char* line = (char*)malloc(LINE_SIZE * sizeof(char));
    int org_pos = ftell(symtab_f->fp);

    // Search the keyword in the file line by line.
    // TODO: != NULL
    rewind(symtab_f->fp);
    while(fgets(line, LINE_SIZE, symtab_f->fp) != NULL)
    {
        if(strstr(line, symbol))
        {
            char* token = strtok(line, "\t");
            token = strtok(NULL, "\t");
            trim_newline(token);
            
            fseek(symtab_f->fp, org_pos, SEEK_SET);
            return token;
        }
    }
    
    fseek(symtab_f->fp, org_pos, SEEK_SET);
    return NULL;
}

void _destroy_instc_cache(char* current_addr, char* line_p, Instruction* i)
{
    free(current_addr);
    free(line_p);
    free(i);
}

void _destroy_obj_cache(char* objcode, char* mnemonic, char* operand)
{
    free(objcode);
    free(mnemonic);
    free(operand);
}


int process_second_pass(FileReader* intermediate_f)
{ 
    // Load OPTAB from PATH_OPTAB.
    FileReader* optab_f = new_FileReader(PATH_OPTAB);
    Optab* optab = new_Optab(optab_f); 

    // Load SYMTAB from PATH_SYMTAB.
    FileReader* symtab_f = new_FileReader(PATH_SYMTAB);

    // TODO: split filename to get prog_name.
    // Prepare object program.
    FileWriter* object_f = new_FileWriter("./copy.obj");
    ObjectProgram* object_prog = new_ObjectProgram();


    // Read first input line from intermediate file.
    // Get current_addr before creating an instruction object.
    char* current_addr = NULL;
    char* line_p = NULL;

    current_addr = _get_prepended_addr(intermediate_f);
    line_p = read_line(intermediate_f);
    Instruction* i = new_Instruction(trim_newline(line_p));

    if(!_search_token(i, "START"))
    {
        return EXIT_FAILURE;
    }
    
    // Write Header record to object program.
    // The prog_name can be retrieved by calling get_label(i).
    init_header_recd(object_prog, get_label(i), current_addr, PROGRAM_LEN);
    write_data(object_f, get_header_recd(object_prog));
    write_data(object_f, "\n");

    // Destroy current instruction cache.
    _destroy_instc_cache(current_addr, line_p, i);

    // Read next input line.
    current_addr = _get_prepended_addr(intermediate_f);
    line_p = read_line(intermediate_f);
    i = new_Instruction(trim_newline(line_p));


    // Initialize first text record.
    init_text_record(object_prog, current_addr);
    while(!has_the_instruction(i, "END"))
    {
        if(!is_comment(i))
        {
            char* objcode  = (char*)malloc(RECD_SIZE * sizeof(char));
            char* mnemonic = NULL;
            char* operand  = NULL;
            char* cache    = NULL;
            int   update_byte_size = 6;
            
            // Search OPTAB for OPCODE.
            if(get_record(optab, (mnemonic=get_instc(i))))
            {
                // If there is a symbol in OPERAND field.
                if(strcmp((operand=get_opfir(i)), "-"))
                {
                    // Search SYMTAB for OPERAND.
                    if(search_keyword(symtab_f, operand))
                    {
                        // Store symbol value as operand address.
                        operand = _get_symbol_addr(symtab_f, operand);
                        
                        // Check if the instruction format
                        // is Index Addressing Mode.
                        //if(is_index_addr(i))
                        if(get_opsec(i) != NULL && is_index_addr(i))
                        {
                            // | OPCODE(4) | X(1) | ADDRESS(15) |
                            int operand_i = strtol(operand, NULL, 16);
                            sprintf(operand, "%04x", (operand_i += 0x8000));
                        }
                    }
                    else
                    {
                        // Set error flag (undefined symbol).
                        printf("Error: Undefined symbol (%s)\n", operand);
                        return EXIT_FAILURE;
                        
                        // Store 0 as operand address.
                        strcpy(operand, "0000");
                    }
                }
                else
                {
                    strcpy(operand, "0000");
                }
                // Assemble the object code instruction.
                strcpy(objcode, get_opcode(get_record(optab, mnemonic)));
                strcat(objcode, operand);
                update_byte_size = 6/2;
            }
            else if(!strcmp(mnemonic, "BYTE") || !strcmp(mnemonic, "WORD"))
            {
                /* Check the first chracter of the opfir.
                 * c: convert the data to ascii codes.
                 * x: convert the data to binary number.
                 */
                operand = (char*)malloc(OPTAB_REC_SIZE * sizeof(char));
                
                char data_type = get_opfir(i)[0];
                char tmp_buf[6];
                int data_len = 0;
                
                operand[0] = 0x00;
                switch(data_type)
                {
                    case 'C':
                        // Convert data to ascii codes.
                        cache = parse_byte_op(i);
                        data_len = strlen(cache);
                        
                        for(int i=0; i<3; i++)
                        {
                            sprintf(tmp_buf, "%02x", cache[i]);
                            memcpy(&operand[i*2], tmp_buf, 2);
                        }
                        update_byte_size = data_len;
                        break;
                    case 'X':
                        // Raw bytes definition.
                        cache = parse_byte_op(i);
                        data_len = strlen(cache);
                        strcat(operand, cache);
                        update_byte_size = data_len/2;
                        break;
                    default:
                        // Convert constant to object code.
                        data_len = strlen(get_opfir(i));
                        long int operand_i = strtol(get_opfir(i), NULL, 10);
                        sprintf(tmp_buf, "%06x", operand_i);
                        strcat(operand, tmp_buf);
                        update_byte_size = 3;
                        break;
                }
                // Convert constant to object code.
                strcpy(objcode, operand);   
                
                // Free operand buffer.
                free(operand);
            }
            else
            {
                update_byte_size = 0;
            }
            
            // If object code won't fit into the current Text Record.
            if(!will_record_fit(object_prog, current_addr, update_byte_size))
            {
                // Write Text Record to object program.
                write_data(object_f, get_text_recd(object_prog));
                write_data(object_f, "\n");
                
                // Initialize new Text Record.
                init_new_text_recd(object_prog, current_addr);
            }
            
            // Add object code to Text Record.
            append_text_record(object_prog, objcode, update_byte_size);
            
            // Free objcode buffer.
            free(objcode);
        }
        // Write listing line.
        // Free currrent instruction cache.
        _destroy_instc_cache(current_addr, line_p, i);
        
        // Read next input line.
        current_addr = _get_prepended_addr(intermediate_f);
        line_p = read_line(intermediate_f);
        i = new_Instruction(trim_newline(line_p));
    }
    // Write last Text record to object program.
    write_data(object_f, get_text_recd(object_prog));
    write_data(object_f, "\n");
    
    // Write End Record to object program.
    init_end_recd(object_prog);
    write_data(object_f, get_end_recd(object_prog));
    write_data(object_f, "\n");
    
    // Final Cleanup.
    delete_ObjectProgram(object_prog);

    fw_close(object_f);
    free(object_f);
    object_f = NULL;

    fr_close(optab_f);
    free(optab_f);
    optab_f = NULL;

    fr_close(symtab_f);
    free(symtab_f);
    symtab_f = NULL;

    free(optab);
    optab = NULL;
    
    return EXIT_SUCCESS;
}
