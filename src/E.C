#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstddef>
#include <assert.h>
#include "RegisterFile.h"
#include "Instructions.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "E.h"
#include "Status.h"

/*
 *  E constructor
 *  
 *  initialize the E pipeline register
 *    
*/
E::E()
{
   int32_t i;
   fields = new PipeRegField * [E_NUMFIELDS];
   numFields = E_NUMFIELDS;
   for (i = 0; i < numFields; i++)
   {
       fields[i] = new PipeRegField();
   }
   bubble();
}

/*
 * bubble
 * 
 *  sets the fields of the E register so that it contains a NOP
 */
void E::bubble()
{
   int32_t i;
   for (i = 0; i < E_NUMFIELDS; i++)
   {
       fields[i]->setState(); 
   }
   fields[E_STAT]->setState(SAOK);
   fields[E_ICODE]->setState(INOP);
   fields[E_IFUN]->setState(FNONE);
   fields[E_DSTE]->setState(RNONE);
   fields[E_DSTM]->setState(RNONE);
   fields[E_SRCA]->setState(RNONE);
   fields[E_SRCB]->setState(RNONE);
}

/* 
 * dump
 *
 * outputs the current values of the E pipeline register
*/
void E::dump()
{
   uint64_t stat = fields[E_STAT]->getOutput();
   uint64_t icode = fields[E_ICODE]->getOutput();
   uint64_t ifun = fields[E_IFUN]->getOutput();
   uint64_t valC = fields[E_VALC]->getOutput();
   uint64_t valA = fields[E_VALA]->getOutput();
   uint64_t valB = fields[E_VALB]->getOutput();
   uint64_t dstE = fields[E_DSTE]->getOutput();
   uint64_t dstM = fields[E_DSTM]->getOutput();
   uint64_t srcA = fields[E_SRCA]->getOutput();
   uint64_t srcB = fields[E_SRCB]->getOutput();
   dumpField("E: stat: ", 1, stat, false);
   dumpField(" icode: ", 1, icode, false);
   dumpField(" ifun: ", 1, ifun, false);
   dumpField(" valC: ", 16, valC, false);
   dumpField(" valA: ", 16, valA, true);
   dumpField("E: valB: ", 16, valB, false);
   dumpField(" dstE: ", 1, dstE, false);
   dumpField(" dstM: ", 1, dstM, false);
   dumpField(" srcA: ", 1, srcA, false);
   dumpField(" srcB: ", 1, srcB, true);
}
