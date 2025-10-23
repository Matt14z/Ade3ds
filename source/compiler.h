#ifndef H_COMPILER
#define H_COMPILER

int is_whitespace(char c);
int is_num(char c);
int is_hex(char c);
int is_alpha(char c);
int is_alpha_num(char c);
unsigned char ident_to_opcode(char *ident, int length);

enum TType {
    NUM,
    STRING,
    IDENT,
    LABEL,
};

typedef struct Token {
    char *lexeme;
    int type;
    int line;
} token_t;

typedef struct Lexer {
    token_t *tokens;
    int size;
} lexer_t;

typedef struct Symbol {
    struct Symbol *next;
    char *label;
    int position;
} symbol_t;

typedef struct Error {
    char message[256];
    int line;
    int code;
} error_t;


lexer_t *lexer_new();
void lexer_push_token(lexer_t *lexer, char *lexeme, int type, int line);
void lexer_retrieve_tokens(const char* src, lexer_t *lexer, error_t *status);

symbol_t *symbol_new(symbol_t *old, char *label, int position, symbol_t *next);
void symbol_free(symbol_t *symbol);
int find_symbol(char *lexeme, symbol_t *head);

void first_pass(lexer_t *lexer, symbol_t *head);
unsigned char *second_pass(lexer_t *lexer, error_t *status);



#endif