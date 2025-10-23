#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"

int is_whitespace(char c) {
    return (c == '\n' || c == '\t' || c == ' ' || c == '\r');
}

int is_num(char c) {
    return (c >= '0' && c <= '9') || c == '-';
}

int is_hex(char c) {
    return is_num(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_alpha_num(char c) {
    return is_num(c) || is_alpha(c);
}

unsigned char ident_to_opcode(char *ident, int length) {
    for(int i = 0; i < length; i++) {
        ident[i] = toupper(ident[i]);
    }

    if (strcmp(ident, "NEG") == 0) return 0x70;
    if (strcmp(ident, "NOT") == 0) return 0x71;
    if (strcmp(ident, "LDV") == 0) return 0x01;
    if (strcmp(ident, "ANDV") == 0) return 0x21;
    if (strcmp(ident, "ORV") == 0) return 0x31;
    if (strcmp(ident, "ADDV") == 0) return 0x41;
    if (strcmp(ident, "SUBV") == 0) return 0x51;
    if (strcmp(ident, "CMPV") == 0) return 0x61;
    if (strcmp(ident, "LD") == 0) return 0x00;
    if (strcmp(ident, "ST") == 0) return 0x10;
    if (strcmp(ident, "AND") == 0) return 0x20;
    if (strcmp(ident, "OR") == 0) return 0x30;
    if (strcmp(ident, "ADD") == 0) return 0x40;
    if (strcmp(ident, "SUB") == 0) return 0x50;
    if (strcmp(ident, "CMP") == 0) return 0x60;

    if (strcmp(ident, "JMP") == 0) return 0x90;
    if (strcmp(ident, "JZ") == 0) return 0x91;
    if (strcmp(ident, "JN") == 0) return 0x92;
    if (strcmp(ident, "JO") == 0) return 0x93;
    if (strcmp(ident, "JC") == 0) return 0x94;
    if (strcmp(ident, "JE") == 0) return 0x91;
    if (strcmp(ident, "JLV") == 0) return 0x95;
    if (strcmp(ident, "JLU") == 0) return 0x96;

    if (strcmp(ident, "LDI") == 0) return 0x02;
    if (strcmp(ident, "STI") == 0) return 0x12;
    if (strcmp(ident, "ANDI") == 0) return 0x22;
    if (strcmp(ident, "ORI") == 0) return 0x32;
    if (strcmp(ident, "ADDI") == 0) return 0x42;
    if (strcmp(ident, "SUBI") == 0) return 0x52;
    if (strcmp(ident, "CMPI") == 0) return 0x62;

    if (strcmp(ident, "PUSH") == 0) return 0x80;
    if (strcmp(ident, "POP") == 0) return 0x81;
    if (strcmp(ident, "CALL") == 0) return 0x82;
    if (strcmp(ident, "RET") == 0) return 0x83;
    if (strcmp(ident, "LDS") == 0) return 0x03;
    if (strcmp(ident, "STS") == 0) return 0x13;
    if (strcmp(ident, "ANDS") == 0) return 0x23;
    if (strcmp(ident, "ORS") == 0) return 0x33;
    if (strcmp(ident, "ADDS") == 0) return 0x43;
    if (strcmp(ident, "SUBS") == 0) return 0x53;
    if (strcmp(ident, "CMPS") == 0) return 0x63;

    if (strcmp(ident, "LA") == 0) return 0x01;
    if (strcmp(ident, "LAS") == 0) return 0x86;
    if (strcmp(ident, "RES") == 0) return 0x84;
    if (strcmp(ident, "REL") == 0) return 0x85;
    if (strcmp(ident, "LDSI") == 0) return 0x04;
    if (strcmp(ident, "STSI") == 0) return 0x14;
    if (strcmp(ident, "ANDSI") == 0) return 0x24;
    if (strcmp(ident, "ORSI") == 0) return 0x34;
    if (strcmp(ident, "ADDSI") == 0) return 0x44;
    if (strcmp(ident, "SUBSI") == 0) return 0x54;
    if (strcmp(ident, "CMPSI") == 0) return 0x64;

    return -1;
}

symbol_t *symbol_new(symbol_t *old, char *label, int position, symbol_t *next) {
    symbol_t *result = (symbol_t*)malloc(sizeof(symbol_t));
    result->label = label;
    result->position = position;
    result->next = next;

    if(old != NULL) {
        old->next = result;
    }

    return result;
}

void symbol_free(symbol_t *symbol) {
    free(symbol);
}

lexer_t *lexer_new() {
    lexer_t *lexer = (lexer_t*) malloc(sizeof(lexer_t));
    lexer->size = 0;
    lexer->tokens = NULL;

    return lexer;
}

void lexer_push_token(lexer_t *lexer, char *lexeme, int type, int line) {
    lexer->tokens = (token_t *)realloc(lexer->tokens, (lexer->size + 1) * (sizeof(token_t)));
    lexer->tokens[lexer->size].lexeme = lexeme;
    lexer->tokens[lexer->size].type = type;
    lexer->tokens[lexer->size].line = line;
    lexer->size += 1;
}

void lexer_retrieve_tokens(const char* src, lexer_t *lexer, error_t *status) {
    int current_line = 0;
    int current = 0;
    int src_length = strlen(src);
    sprintf(status->message, "");

    status->code = -1;
    status->line = -1;

    while(current <= src_length) {
        if(is_whitespace(src[current])) {
            while(is_whitespace(src[current])) {
                if(src[current] == '\n') current_line++;

                current++;
            }
        } 
        else if(is_alpha(src[current])) {
            int start = current;
            int end = current;

            while(is_alpha(src[current])) {
                end++;
                current++;
            }

            char *word = (char*)malloc(end-start+1);

            for(int i = 0; i < end - start; i++) {
                word[i] = src[start + i];
            }

            word[end-start] = '\0';

            if(current <= src_length) {
                if(src[current] == ':') {
                    lexer_push_token(lexer, word, LABEL, current_line);
                    current++;
                }else {
                    lexer_push_token(lexer, word, IDENT, current_line);
                }
            }
        }
        else if(is_num(src[current])) {
            int start, end;

            if(src[current] == '-' && current + 2 <= src_length && src[current+1] == '0' && src[current+2] == 'x') {
                status->code=1;
                status->line = current_line;
                sprintf(status->message, "Cannot negate hexadecimal.");
                return;
            }

            if(current + 1 <= src_length && src[current] == '0' && src[current+1] == 'x') {
                start = current;
                end = current + 2;
                current += 2;
                
                while(is_hex(src[current])) {
                    end++;
                    current++;   
                }   
            } else {
                start = current;
                end = current;

                while(is_num(src[current])) {
                    end++;
                    current++;
                }
            }

            char *word = (char*)malloc(end-start);

            for(int i = 0; i < end - start; i++) {
                word[i] = src[start + i];
            }

            lexer_push_token(lexer, word, NUM, current_line);
        }else if(src[current] == '"') {
            current++;
            int start = current;
            int end = current;

            while(src[current] != '"') {
                end++;
                current++;
            }

            current++;

            char *word = (char*)malloc(end-start);

            for(int i = 0; i < end - start; i++) {
                word[i] = src[start + i];
            }

            lexer_push_token(lexer, word, STRING, current_line);
        } 
        else {
            current++;
        }
    }
}

int find_symbol(char *lexeme, symbol_t *head) {
    symbol_t *current = head;

    while (current != NULL) {
        if (strcmp(lexeme, current->label) == 0) {
            return current->position;
        }
        current = current->next;
    }

    return -1;
}

void first_pass(lexer_t *lexer, symbol_t *head) {
    int current_position = 0;
    symbol_t *current = head;

    for(int i = 0; i < lexer->size; i++) {
        if(lexer->tokens[i].type != LABEL) {
            current_position++;
        }

        if (lexer->tokens[i].type == LABEL) {
            current->label = lexer->tokens[i].lexeme;
            current->position = current_position;
            current->next = symbol_new(current, "", -1, NULL);
            current = current->next;
        }
    }

    for(int i = 0; i < lexer->size; i++) {
        if(lexer->tokens[i].type == IDENT) {
            int position = find_symbol(lexer->tokens[i].lexeme, head);
            
            if(position != -1) {
                char buffer[16];
                sprintf(buffer, "%x", position);
                lexer->tokens[i].lexeme = strdup(buffer);   
                lexer->tokens[i].type = NUM;         
            }
        }
    }
}

unsigned char *second_pass(lexer_t *lexer, error_t *status) {
    unsigned char *output = (unsigned char*)malloc(sizeof(unsigned char) * 256);
    int current_byte = 0;

    status->code = -1;
    status->line = -1;

    for(int i = 0; i < 256; i++) {
        output[i] = 0;
    }

    for(int i = 0; i < lexer->size; i++) {
        if(current_byte >= 256) {
            status->code = 0;
            status->line = lexer->tokens[i].line;
            sprintf(status->message, "Exceeding memory limit of 256 bytes.");
            return NULL;
        }

        if(lexer->tokens[i].type == IDENT) {
            unsigned char opcode = (unsigned char)ident_to_opcode(lexer->tokens[i].lexeme, strlen(lexer->tokens[i].lexeme));

            if(opcode == 255) {
                status->code = 2;
                status->line = lexer->tokens[i].line;
                sprintf(status->message, "Unknown mnemonic. (%s)", lexer->tokens[i].lexeme);
                return NULL;
            } else {
                output[current_byte] = opcode;
                current_byte++;

                if(opcode == 0x70 || opcode == 0x71 || opcode == 0x80 || opcode == 0x81 ||
                    opcode == 0x83) {
                        if(current_byte + 1 >= 256) {
                            status->code = 0;
                            status->line = lexer->tokens[i].line;
                            sprintf(status->message, "Exceeding memory limit of 256 bytes.");
                            return NULL;
                        }
                        current_byte++;
                    }
            }
        }

        if(lexer->tokens[i].type == STRING) {
            int length = strlen(lexer->tokens[i].lexeme);
            for(int j = 0; j < length; j++) {
                output[current_byte] = lexer->tokens[i].lexeme[j];
                current_byte++;

                if(current_byte >= 256) {
                    status->code = 0;
                    status->line = lexer->tokens[i].line;
                    sprintf(status->message, "Exceeding memory limit of 256 bytes.");
                    return NULL;
                }
            }
        }

        if(lexer->tokens[i].type == NUM) {
            unsigned char number = (unsigned char)strtol(lexer->tokens[i].lexeme, NULL, 0);
            int num_int = strtol(lexer->tokens[i].lexeme, NULL, 0);

            if(num_int > 255) {
                status->code = 0;
                status->line = lexer->tokens[i].line;
                sprintf(status->message, "Number %d is too big.", number);
                return NULL;
            } else {
                output[current_byte] = number;
                current_byte++;
            }
        }
    }

    return output;
}

