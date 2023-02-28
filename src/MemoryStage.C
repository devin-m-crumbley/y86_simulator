#include <iostream>
#include <cstdint>
#include <stdio.h>
#include "PipeRegField.h"
#include "PipeReg.h"
#include "Memory.h"
#include "RegisterFile.h"
#include "ConditionCodes.h"
#include "Stage.h"
#include "MemoryStage.h"
#include "Debug.h"
#include "Instructions.h"
#include "Status.h"
#include "M.h"
#include "W.h"

/*
 * doClockLow
 *
 * Performs the Memory stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
bool MemoryStage::doClockLow(PipeReg ** pregs)
{
   PipeReg * mreg = pregs[MREG];
   PipeReg * wreg = pregs[WREG];
   uint64_t icode = mreg->get(M_ICODE);
   uint64_t cnd = mreg->get(M_CND);
   uint64_t valE = mreg->get(M_VALE);
   uint64_t valA = mreg->get(M_VALA);
   uint64_t dste = mreg->get(M_DSTE);
   uint64_t dstm = mreg->get(M_DSTM);
   uint64_t address = Addr(icode, mreg);
   bool mem_error = false;
   m_stat = 0;
   m_valM = 0;
   if(Mem_read(icode)){
      m_valM = mem->getLong(address, mem_error);
   }
   if(Mem_write(icode)){
      mem->putLong(valA, address, mem_error);
   }
   if(mem_error) m_stat = SADR;
   else m_stat = mreg->get(M_STAT);

   setWInput(wreg, m_stat, icode, valE, m_valM, dste, dstm);
   return false;
}

void MemoryStage::setWInput(PipeReg * wreg, uint64_t W_stat, uint64_t W_icode, 
                           uint64_t W_valE, uint64_t W_valM,  uint64_t W_dste, 
                           uint64_t W_dstm)
{
   wreg->set(W_STAT, W_stat);
   wreg->set(W_ICODE, W_icode);
   wreg->set(W_VALE, W_valE);
   wreg->set(W_VALM, W_valM);
   wreg->set(W_DSTE, W_dste);
   wreg->set(W_DSTM, W_dstm);
}

/* doClockHigh
 *
 * 
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
*/
void MemoryStage::doClockHigh(PipeReg ** pregs)
{
   PipeReg * wreg = pregs[WREG];

   wreg->normal();
}

/* Addr - Addr
 *
 * //HCL for Addr component
 * M_icode in { IRMMOVQ, IPUSHQ, ICALL, IMRMOVQ } : M_valE;
 * M_icode in { IPOPQ, IRET } : M_valA;
 * 1: 0;  
 * ];
 *
 * @param: uint64_t icode  - the icode that is passed into the the system from doClockLow
 * @param: PipeReg * mreg - array of the pipeline register (for the M instance)
 */
uint64_t MemoryStage::Addr(uint64_t icode, PipeReg * mreg){
   switch(icode){
      case IRMMOVQ:
      case IPUSHQ:
      case ICALL:
      case IMRMOVQ:
      return mreg->get(M_VALE);
      case IPOPQ:
      case IRET:
      return mreg->get(M_VALA);
      default:
      return 0;
   }
}

/* Mem_read - Mem_read
 *
 * //HCL for Mem Read component
 * bool mem_read = M_icode in { IMRMOVQ, IPOPQ, IRET }; 
 *
 * @param: uint64_t icode  - the icode that is passed into the the system from doClockLow
 */
uint64_t MemoryStage::Mem_read(uint64_t icode){
   switch(icode){
      case IMRMOVQ:
      case IPOPQ:
      case IRET:
      return 1;
      default:
      return 0;
   }
}

/* Mem_read - Mem_read
 *
 * //HCL for Mem Write component
 * bool mem_write = M_icode in { IRMMOVQ, IPUSHQ, ICALL }; 
 * 
 * @param: uint64_t icode  - the icode that is passed into the the system from doClockLow
 */
uint64_t MemoryStage::Mem_write(uint64_t icode){
   switch(icode){
      case IRMMOVQ:
      case IPUSHQ:
      case ICALL:
      return 1;
      default:
      return 0;
   }
}