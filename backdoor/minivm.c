//-----------------------------------------------------------------------------
// minivm.c -- The basic definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include "minivm.h"
#include <stdlib.h>
#include <assert.h>


//---------------------------------------------------------
// FUNCTION IMPLEMENTATIONS:


// Defers decoding of register args to the called function.
// dispatch :: VMContext -> uint32_t -> Effect()
void dispatch(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t i = EXTRACT_B0(instr);
    (*ctx->funtable[i])(ctx, instr);
    // opcode ite and jump doesn't need to increase pc
    if (i == 0xa0 || i == 0xb0) return;
    // Increment to next instruction.
    ctx->pc++;
}


// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx, const uint32_t numRegs,
                   const uint32_t numFuns, Reg* registers,
                   FunPtr* funtable, uint32_t* bytecode) 
{
    ctx->numRegs    = numRegs;
    ctx->numFuns    = numFuns;
    ctx->r          = registers;
    ctx->funtable   = funtable;
    ctx->pc         = 0;
    ctx->heap       = (uint8_t*) malloc(8192);
    assert(ctx->heap != NULL);
    ctx->bytecode   = bytecode;
}


// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> uint32_t** -> Effect()
void stepVMContext(struct VMContext* ctx) {
    // Read a 32-bit bytecode instruction.
    uint32_t instr = *(ctx->bytecode + ctx->pc);

    // Dispatch to an opcode-handler.
    dispatch(ctx, instr);
}

