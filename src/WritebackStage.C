#include <iostream>
#include <cstdint>
#include <stdio.h>
#include "PipeRegField.h"
#include "PipeReg.h"
#include "Memory.h"
#include "RegisterFile.h"
#include "ConditionCodes.h"
#include "Stage.h"
#include "WritebackStage.h"
#include "Debug.h"
#include "Status.h"
#include "W.h"
#include "Instructions.h"


/*
 * doClockLow
 *
 * Performs the Writeback stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
bool WritebackStage::doClockLow(PipeReg ** pregs)
{
   PipeReg * wreg = pregs[WREG];
   uint64_t W_icode = wreg->get(W_ICODE);
   uint64_t W_stat = wreg->get(W_STAT);
   return W_stat != SAOK;
}

/* doClockHigh
 *
 * 
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
*/
void WritebackStage::doClockHigh(PipeReg ** pregs)
{
   bool error = false;
   PipeReg * wreg = pregs[WREG];
   RegisterFile * name = RegisterFile::getInstance();
   name->writeRegister(wreg->get(W_VALE), wreg->get(W_DSTE), error);
   name->writeRegister(wreg->get(W_VALM), wreg->get(W_DSTM), error);

}