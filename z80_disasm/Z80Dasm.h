/*** Z80Em: Portable Z80 emulator *******************************************/
/***                                                                      ***/
/***                               Z80Dasm.h                              ***/
/***                                                                      ***/
/*** This file contains a routine to disassemble a buffer containing Z80  ***/
/*** opcodes. It is included from Z80Debug.c and z80dasm.c                ***/
/***                                                                      ***/
/*** Copyright (C) Marcel de Kogel 1996,1997                              ***/
/***     You are not allowed to distribute this software commercially     ***/
/***     Please, notify me, if you make any changes to this file          ***/
/****************************************************************************/
#ifndef Z80_DASM_H
#define Z80_DASM_H

#include <stdio.h>
#include <string.h>
#include "../cpu/Z80.h"

class Z80Dasm {
public:
    
    Z80Dasm (Z80Environment &ev);
    char Sign (unsigned char a);
    int  Abs (unsigned char a);
    int  disasm(unsigned PC);

private:
    Z80Environment &env_;
   
}; // Z80Dasm class ends.
#endif
