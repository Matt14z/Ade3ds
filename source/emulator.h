#ifndef H_EMULATOR
#define H_EMULATOR

#define FLAG_ZERO 0b00000001
#define FLAG_NEGATIVE 0b00000010
#define FLAG_OVERFLOW 0b00000100
#define FLAG_CARRY 0b00001000

typedef struct ADE8 {
    unsigned char acc;
    unsigned char pc;
    unsigned char sp;
    unsigned char flag;
    unsigned char memory[256];
} ade8_t;

void ade8_reset(ade8_t *ade8);
ade8_t *ade8_new();

// Unary operations
void NEG(ade8_t *ade8);
void NOT(ade8_t *ade8);

// Load/Store
void LD(ade8_t *ade8);
void LDV(ade8_t *ade8);
void LDI(ade8_t *ade8);
void LDS(ade8_t *ade8);
void LDSI(ade8_t *ade8);
void ST(ade8_t *ade8);
void STI(ade8_t *ade8);
void STS(ade8_t *ade8);
void STSI(ade8_t *ade8);

// Logical
void AND(ade8_t *ade8);
void ANDV(ade8_t *ade8);
void ANDI(ade8_t *ade8);
void ANDS(ade8_t *ade8);
void ANDSI(ade8_t *ade8);
void OR(ade8_t *ade8);
void ORV(ade8_t *ade8);
void ORI(ade8_t *ade8);
void ORS(ade8_t *ade8);

// Arithmetic
void ADD(ade8_t *ade8);
void ADDV(ade8_t *ade8);
void ADDI(ade8_t *ade8);
void ADDS(ade8_t *ade8);
void ADDSI(ade8_t *ade8);
void SUB(ade8_t *ade8);
void SUBV(ade8_t *ade8);
void SUBI(ade8_t *ade8);
void SUBS(ade8_t *ade8);
void SUBSI(ade8_t *ade8);

// Comparison
void CMP(ade8_t *ade8);
void CMPV(ade8_t *ade8);
void CMPI(ade8_t *ade8);
void CMPS(ade8_t *ade8);
void CMPSI(ade8_t *ade8);

// Stack operations
void PUSH(ade8_t *ade8);
void POP(ade8_t *ade8);
void CALL(ade8_t *ade8);
void RET(ade8_t *ade8);
void RES(ade8_t *ade8);
void REL(ade8_t *ade8);
void LAS(ade8_t *ade8);
void LA(ade8_t *ade8);

// Jumps
void JMP(ade8_t *ade8);
void JZ(ade8_t *ade8);
void JN(ade8_t *ade8);
void JO(ade8_t *ade8);
void JC(ade8_t *ade8);
void JE(ade8_t *ade8);
void JLV(ade8_t *ade8);
void JLU(ade8_t *ade8);

void ade8_execute_next(ade8_t *ade8);

#endif