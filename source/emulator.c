#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "emulator.h"

void ade8_reset(ade8_t *ade8) {
    memset(ade8->memory, 0, 256);
    ade8->acc = 0;
    ade8->pc = 0;
    ade8->sp = 255;
    ade8->flag = 0;
}

ade8_t *ade8_new() {
    ade8_t *ade8 = (ade8_t*)malloc(sizeof(ade8_t));
    ade8_reset(ade8);

    return ade8;
}

// Unary operations
void NEG(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc = -ade8->acc;

    if((ade8->acc &0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;
    }
}
void NOT(ade8_t *ade8) {
    ade8->pc ++;
    ade8->acc = ~ade8->acc;

    if((ade8->acc &0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;
    }
}

// Load/Store
void LD(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc = ade8->memory[ade8->memory[ade8->memory[ade8->pc]]];
}
void LDV(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc = ade8->memory[ade8->pc];
}
void LDI(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc = ade8->memory[ade8->memory[ade8->memory[ade8->pc]]];
}
void LDS(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc = ade8->memory[ade8->memory[ade8->pc] + ade8->sp];
}
void LDSI(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc = ade8->memory[ade8->memory[ade8->memory[ade8->pc] + ade8->sp]];
}
void ST(ade8_t *ade8) {
    ade8->pc++;
    ade8->memory[ade8->memory[ade8->pc]] = ade8->acc;
}
void STI(ade8_t *ade8) {
    ade8->pc++;
    ade8->memory[ade8->memory[ade8->memory[ade8->pc]]] = ade8->acc;
}
void STS(ade8_t *ade8) {
    ade8->pc++;
    ade8->memory[ade8->memory[ade8->pc]+ ade8->sp] = ade8->acc;
}
void STSI(ade8_t *ade8) {
    ade8->pc++;
    ade8->memory[ade8->memory[ade8->memory[ade8->pc] + ade8->sp]] = ade8->acc;
}

// Logical
void AND(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc &= ade8->memory[ade8->memory[ade8->pc]];

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc &0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void ANDV(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc &= ade8->memory[ade8->pc];

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc &0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void ANDI(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc &= ade8->memory[ade8->memory[ade8->memory[ade8->pc]]];

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc &0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void ANDS(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc &= ade8->memory[ade8->memory[ade8->pc] + ade8->sp];

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc &0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void ANDSI(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc &= ade8->memory[ade8->memory[ade8->memory[ade8->pc] + ade8->sp]];

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc &0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}

void OR(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc |= ade8->memory[ade8->memory[ade8->pc]];

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc &0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void ORV(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc |= ade8->memory[ade8->pc];

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc &0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void ORI(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc |= ade8->memory[ade8->memory[ade8->memory[ade8->pc]]];

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc &0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void ORS(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc |= ade8->memory[ade8->memory[ade8->pc] + ade8->sp];

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc &0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void ORSI(ade8_t *ade8) {
    ade8->pc++;
    ade8->acc |= ade8->memory[ade8->memory[ade8->memory[ade8->pc] + ade8->sp]];

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc &0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}

// Arithmetic
void ADD(ade8_t *ade8) {
    ade8->pc++;
    unsigned char a = ade8->acc;
    unsigned char b = ade8->memory[ade8->memory[ade8->pc]];
    ade8->acc = a + b;
    int result = a + b;

    if(result > 0xFF) {
        ade8->flag |= FLAG_CARRY;
    }

    char sa = (char)a;
    char sb = (char)b;
    char sresult = sa + sb;

    if((sa > 0 && sb > 0 && sresult < 0) ||
        (sa < 0 && sb < 0 && sresult > 0)) {
        ade8->flag |= FLAG_OVERFLOW;        
    }

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc & 0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void ADDV(ade8_t *ade8) {
    ade8->pc++;
    unsigned char a = ade8->acc;
    unsigned char b = ade8->memory[ade8->pc];
    ade8->acc = a + b;
    int result = a + b;

    if(result > 0xFF) {
        ade8->flag |= FLAG_CARRY;
    }

    char sa = (char)a;
    char sb = (char)b;
    char sresult = sa + sb;

    if((sa > 0 && sb > 0 && sresult < 0) ||
        (sa < 0 && sb < 0 && sresult > 0)) {
        ade8->flag |= FLAG_OVERFLOW;        
    }

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc &0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void ADDI(ade8_t *ade8) {
    ade8->pc++;
    unsigned char a = ade8->acc;
    unsigned char b = ade8->memory[ade8->memory[ade8->memory[ade8->pc]]];
    ade8->acc = a + b;
    int result = a + b;

    if(result > 0xFF) {
        ade8->flag |= FLAG_CARRY;
    }

    char sa = (char)a;
    char sb = (char)b;
    char sresult = sa + sb;

    if((sa > 0 && sb > 0 && sresult < 0) ||
        (sa < 0 && sb < 0 && sresult > 0)) {
        ade8->flag |= FLAG_OVERFLOW;        
    }

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc & 0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void ADDS(ade8_t *ade8) {
    ade8->pc++;
    unsigned char a = ade8->acc;
    unsigned char b = ade8->memory[ade8->memory[ade8->pc] + ade8->sp];
    ade8->acc = a + b;
    int result = a + b;

    if(result > 0xFF) {
        ade8->flag |= FLAG_CARRY;
    }

    char sa = (char)a;
    char sb = (char)b;
    char sresult = sa + sb;

    if((sa > 0 && sb > 0 && sresult < 0) ||
        (sa < 0 && sb < 0 && sresult > 0)) {
        ade8->flag |= FLAG_OVERFLOW;        
    }

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc & 0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void ADDSI(ade8_t *ade8) {
    ade8->pc++;
    unsigned char a = ade8->acc;
    unsigned char b = ade8->memory[ade8->memory[ade8->memory[ade8->pc] + ade8->sp]];
    ade8->acc = a + b;
    int result = a + b;

    if(result > 0xFF) {
        ade8->flag |= FLAG_CARRY;
    }

    char sa = (char)a;
    char sb = (char)b;
    char sresult = sa + sb;

    if((sa > 0 && sb > 0 && sresult < 0) ||
        (sa < 0 && sb < 0 && sresult > 0)) {
        ade8->flag |= FLAG_OVERFLOW;        
    }

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc & 0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}

void SUB(ade8_t *ade8) {
    ade8->pc++;
    unsigned char a = ade8->acc;
    unsigned char b = ade8->memory[ade8->memory[ade8->pc]];
    ade8->acc = a - b;
    int result = a - b;

    if(result > 0xFF) {
        ade8->flag |= FLAG_CARRY;
    }

    char sa = (char)a;
    char sb = (char)b;
    char sresult = sa - sb;

    if((sa > 0 && sb > 0 && sresult < 0) ||
        (sa < 0 && sb < 0 && sresult > 0)) {
        ade8->flag |= FLAG_OVERFLOW;        
    }

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc & 0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void SUBV(ade8_t *ade8) {
    ade8->pc++;
    unsigned char a = ade8->acc;
    unsigned char b = ade8->memory[ade8->pc];
    ade8->acc = a-b;
    int result = a - b;

    if(result > 0xFF) {
        ade8->flag |= FLAG_CARRY;
    }

    char sa = (char)a;
    char sb = (char)b;
    char sresult = sa - sb;

    if((sa > 0 && sb > 0 && sresult < 0) ||
        (sa < 0 && sb < 0 && sresult > 0)) {
        ade8->flag |= FLAG_OVERFLOW;        
    }

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc & 0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void SUBI(ade8_t *ade8) {
    ade8->pc++;
    unsigned char a = ade8->acc;
    unsigned char b = ade8->memory[ade8->memory[ade8->memory[ade8->pc]]];
    ade8->acc = a - b;
    int result = a - b;

    if(result > 0xFF) {
        ade8->flag |= FLAG_CARRY;
    }

    char sa = (char)a;
    char sb = (char)b;
    char sresult = sa - sb;

    if((sa > 0 && sb > 0 && sresult < 0) ||
        (sa < 0 && sb < 0 && sresult > 0)) {
        ade8->flag |= FLAG_OVERFLOW;        
    }

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc & 0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void SUBS(ade8_t *ade8) {
    ade8->pc++;
    unsigned char a = ade8->acc;
    unsigned char b = ade8->memory[ade8->memory[ade8->pc] + ade8->sp];
    ade8->acc = a - b;
    int result = a - b;

    if(result > 0xFF) {
        ade8->flag |= FLAG_CARRY;
    }

    char sa = (char)a;
    char sb = (char)b;
    char sresult = sa - sb;

    if((sa > 0 && sb > 0 && sresult < 0) ||
        (sa < 0 && sb < 0 && sresult > 0)) {
        ade8->flag |= FLAG_OVERFLOW;        
    }

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc & 0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void SUBSI(ade8_t *ade8) {
    ade8->pc++;
    unsigned char a = ade8->acc;
    unsigned char b = ade8->memory[ade8->memory[ade8->memory[ade8->pc] + ade8->sp]];
    ade8->acc = a - b;
    int result = a - b;

    if(result > 0xFF) {
        ade8->flag |= FLAG_CARRY;
    }

    char sa = (char)a;
    char sb = (char)b;
    char sresult = sa - sb;

    if((sa > 0 && sb > 0 && sresult < 0) ||
        (sa < 0 && sb < 0 && sresult > 0)) {
        ade8->flag |= FLAG_OVERFLOW;        
    }

    if(ade8->acc == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if((ade8->acc & 0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}

// Comparison
void CMP(ade8_t *ade8) {
    ade8->pc++;
    unsigned char a = ade8->acc;
    unsigned char b = ade8->memory[ade8->memory[ade8->pc]];
    int result = a - b;

    if(result > 0xFF) {
        ade8->flag |= FLAG_CARRY;
    }

    char sa = (char)a;
    char sb = (char)b;
    char sresult = sa - sb;

    if((sa > 0 && sb > 0 && sresult < 0) ||
        (sa < 0 && sb < 0 && sresult > 0)) {
        ade8->flag |= FLAG_OVERFLOW;        
    }

    if((a-b) == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if(((a-b) & 0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void CMPV(ade8_t *ade8) {
    ade8->pc++;
    unsigned char a = ade8->acc;
    unsigned char b = ade8->memory[ade8->pc];
    int result = a - b;

    if(result > 0xFF) {
        ade8->flag |= FLAG_CARRY;
    }

    char sa = (char)a;
    char sb = (char)b;
    char sresult = sa - sb;

    if((sa > 0 && sb > 0 && sresult < 0) ||
        (sa < 0 && sb < 0 && sresult > 0)) {
        ade8->flag |= FLAG_OVERFLOW;        
    }

    if((a-b) == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if(((a-b) & 0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void CMPI(ade8_t *ade8) {
    ade8->pc++;
    unsigned char a = ade8->acc;
    unsigned char b = ade8->memory[ade8->memory[ade8->memory[ade8->pc]]];
    int result = a - b;

    if(result > 0xFF) {
        ade8->flag |= FLAG_CARRY;
    }

    char sa = (char)a;
    char sb = (char)b;
    char sresult = sa - sb;

    if((sa > 0 && sb > 0 && sresult < 0) ||
        (sa < 0 && sb < 0 && sresult > 0)) {
        ade8->flag |= FLAG_OVERFLOW;        
    }

    if((a-b) == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if(((a-b) & 0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void CMPS(ade8_t *ade8) {
    ade8->pc++;
    unsigned char a = ade8->acc;
    unsigned char b = ade8->memory[ade8->memory[ade8->pc] + ade8->sp];
    int result = a - b;

    if(result > 0xFF) {
        ade8->flag |= FLAG_CARRY;
    }

    char sa = (char)a;
    char sb = (char)b;
    char sresult = sa - sb;

    if((sa > 0 && sb > 0 && sresult < 0) ||
        (sa < 0 && sb < 0 && sresult > 0)) {
        ade8->flag |= FLAG_OVERFLOW;        
    }

    if((a-b) == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if(((a-b) & 0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}
void CMPSI(ade8_t *ade8) {
    ade8->pc++;
    unsigned char a = ade8->acc;
    unsigned char b = ade8->memory[ade8->memory[ade8->memory[ade8->pc] + ade8->sp]];
    int result = a - b;

    if(result > 0xFF) {
        ade8->flag |= FLAG_CARRY;
    }

    char sa = (char)a;
    char sb = (char)b;
    char sresult = sa - sb;

    if((sa > 0 && sb > 0 && sresult < 0) ||
        (sa < 0 && sb < 0 && sresult > 0)) {
        ade8->flag |= FLAG_OVERFLOW;        
    }

    if((a-b) == 0) {
        ade8->flag |= FLAG_ZERO;               
    }

    if(((a-b) & 0b10000000) == 0b10000000) {
        ade8->flag |= FLAG_NEGATIVE;
    }
}

// Stack operations
void PUSH(ade8_t *ade8) {
    ade8->memory[ade8->sp] = ade8->acc;
    ade8->sp = ade8->sp - 1;
}
void POP(ade8_t *ade8) {
    if(ade8->sp != 255) {
        ade8->acc = ade8->memory[ade8->sp];
        ade8->sp = ade8->sp + 1;
    }
}
void CALL(ade8_t *ade8) {
    ade8->sp = ade8->sp - 1;
    ade8->memory[ade8->sp] = ade8->pc;
    ade8->pc = ade8->memory[ade8->pc + 1];
    ade8->pc++;
}
void RET(ade8_t *ade8) {
    ade8->pc = ade8->memory[ade8->sp];
    ade8->sp = ade8->sp + 1;
}
void RES(ade8_t *ade8) {
    ade8->sp = ade8->sp - ade8->memory[ade8->pc + 1];
    ade8->pc = ade8->pc + 1;
}
void REL(ade8_t *ade8) {
    ade8->sp = ade8->sp + ade8->memory[ade8->pc + 1];
    ade8->pc = ade8->pc + 1;
}
void LAS(ade8_t *ade8) {
    ade8->acc = ade8->memory[ade8->pc + 1] + ade8->sp;
    ade8->pc = ade8->pc + 1;
}
void LA(ade8_t *ade8) {
    ade8->acc = ade8->memory[ade8->pc + 1];
    ade8->pc = ade8->pc + 1;
}

// Jumps
void JMP(ade8_t *ade8) {
    ade8->pc = ade8->memory[ade8->pc+1];
}
void JZ(ade8_t *ade8) {
    if((ade8->flag & FLAG_ZERO) == FLAG_ZERO) {
        ade8->pc = ade8->memory[ade8->pc+1];
    }else {
        ade8->pc += 2;
    }
}
void JN(ade8_t *ade8) {
    if((ade8->flag & FLAG_NEGATIVE) == FLAG_NEGATIVE) {
        ade8->pc = ade8->memory[ade8->pc+1];
    }else {
        ade8->pc += 2;
    }
}
void JO(ade8_t *ade8) {
    if((ade8->flag & FLAG_OVERFLOW) == FLAG_OVERFLOW) {
        ade8->pc = ade8->memory[ade8->pc+1];
    }else {
        ade8->pc += 2;
    }
}
void JC(ade8_t *ade8) {
    if((ade8->flag & FLAG_CARRY) == FLAG_CARRY) {
        ade8->pc = ade8->memory[ade8->pc+1];
    }else {
        ade8->pc += 2;
    }
}
void JE(ade8_t *ade8) {
    if((ade8->flag & FLAG_ZERO) == FLAG_ZERO) {
        ade8->pc = ade8->memory[ade8->pc+1];
    }else {
        ade8->pc += 2;
    }
}
void JLV(ade8_t *ade8) {
    if((ade8->flag & FLAG_OVERFLOW) == FLAG_OVERFLOW && 
        (ade8->flag & FLAG_NEGATIVE) == FLAG_NEGATIVE) {
        ade8->pc = ade8->memory[ade8->pc+1];
    } else {
        ade8->pc += 2;
    }
}
void JLU(ade8_t *ade8) {
    if((ade8->flag & FLAG_CARRY) == FLAG_CARRY) {
        ade8->pc = ade8->memory[ade8->pc+1];
    }else {
        ade8->pc += 2;
    }
}

void ade8_execute_next(ade8_t *ade8) {
    switch (ade8->memory[ade8->pc]) {

        // Unary operations
        case 0x70: NEG(ade8); break;
        case 0x71: NOT(ade8); break;

        // Load/Store
        case 0x00: LD(ade8); break;
        case 0x01: LDV(ade8); break;
        case 0x02: LDI(ade8); break;
        case 0x03: LDS(ade8); break;
        case 0x04: LDSI(ade8); break;

        case 0x10: ST(ade8); break;
        case 0x12: STI(ade8); break;
        case 0x13: STS(ade8); break;
        case 0x14: STSI(ade8); break;

        // Logical
        case 0x20: AND(ade8); break;
        case 0x21: ANDV(ade8); break;
        case 0x22: ANDI(ade8); break;
        case 0x23: ANDS(ade8); break;
        case 0x24: ANDSI(ade8); break;

        case 0x30: OR(ade8); break;
        case 0x31: ORV(ade8); break;
        case 0x32: ORI(ade8); break;
        case 0x33: ORS(ade8); break;
        case 0x34: ORSI(ade8); break;

        // Arithmetic
        case 0x40: ADD(ade8); break;
        case 0x41: ADDV(ade8); break;
        case 0x42: ADDI(ade8); break;
        case 0x43: ADDS(ade8); break;
        case 0x44: ADDSI(ade8); break;

        case 0x50: SUB(ade8); break;
        case 0x51: SUBV(ade8); break;
        case 0x52: SUBI(ade8); break;
        case 0x53: SUBS(ade8); break;
        case 0x54: SUBSI(ade8); break;

        // Comparison
        case 0x60: CMP(ade8); break;
        case 0x61: CMPV(ade8); break;
        case 0x62: CMPI(ade8); break;
        case 0x63: CMPS(ade8); break;
        case 0x64: CMPSI(ade8); break;

        // Stack operations
        case 0x80: PUSH(ade8); break;
        case 0x81: POP(ade8); break;
        case 0x82: CALL(ade8); break;
        case 0x83: RET(ade8); break;
        case 0x84: RES(ade8); break;
        case 0x85: REL(ade8); break;
        case 0x86: LAS(ade8); break;

        // Jumps
        case 0x90: JMP(ade8); break;
        case 0x91: JZ(ade8); break;
        case 0x92: JN(ade8); break;
        case 0x93: JO(ade8); break;
        case 0x94: JC(ade8); break;
        case 0x95: JLV(ade8); break;
        case 0x96: JLU(ade8); break;

        default:
            break;
    }
    ade8->pc++;
}