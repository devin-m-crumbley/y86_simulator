#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstddef>
#include <assert.h>
#include "RegisterFile.h"
#include "Instructions.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "W.h"
#include "Status.h"

/*
 * W constructor
 *
 * initialize the W pipeline register 
*/
W::W()
{
   int32_t i;
   fields = new PipeRegField * [W_NUMFIELDS];
   numFields = W_NUMFIELDS;
   for (i = 0; i < numFields; i++)
   {
       fields[i] = new PipeRegField();
   }
   bubble();
}

/*
 * bubble
 *
 * Set all elements of the PipeRegField array within so that 
 * the W PipeReg holds a NOP
 */
void W::bubble()
{
   int32_t i;
   for (i = 0; i < numFields; i++)
   {
       fields[i]->setState();
   }
   fields[W_STAT]->setState(SAOK);
   fields[W_ICODE]->setState(INOP);
   fields[W_DSTE]->setState(RNONE);
   fields[W_DSTM]->setState(RNONE);
}

/* 
 * dump
 *
 * outputs the current values of the W pipeline register
*/
void W:: dump()
{
   uint64_t stat = fields[W_STAT]->getOutput();
   uint64_t icode = fields[W_ICODE]->getOutput();
   uint64_t valE = fields[W_VALE]->getOutput();
   uint64_t valM = fields[W_VALM]->getOutput();
   uint64_t dstE = fields[W_DSTE]->getOutput();
   uint64_t dstM = fields[W_DSTM]->getOutput();
   dumpField("W: stat: ", 1, stat, false);
   dumpField(" icode: ", 1, icode, false);
   dumpField(" valE: ", 16, valE, false);
   dumpField(" valM: ", 16, valM, false);
   dumpField(" dstE: ", 1, dstE, false);
   dumpField(" dstM: ", 1, dstM, true);
}

