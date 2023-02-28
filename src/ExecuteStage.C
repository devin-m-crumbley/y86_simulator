#include <iostream>
#include <cstdint>
#include <stdio.h>
#include "PipeRegField.h"
#include "PipeReg.h"
#include "Memory.h"
#include "RegisterFile.h"
#include "ConditionCodes.h"
#include "Stage.h"
#include "ExecuteStage.h"
#include "Debug.h"
#include "Instructions.h"
#include "Tools.h"
#include "Status.h"
#include "E.h"
#include "M.h"
#include "W.h"

/*
 * doClockLow
 *
 * Performs the Execute stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
bool ExecuteStage::doClockLow(PipeReg ** pregs)
{
   PipeReg * ereg = pregs[EREG];
   PipeReg * mreg = pregs[MREG];
   PipeReg * wreg = pregs[WREG];
   uint64_t stat = ereg->get(E_STAT);
   uint64_t icode = ereg->get(E_ICODE);
   uint64_t ifun = ereg->get(E_IFUN);
   uint64_t valA = ereg->get(E_VALA);
   uint64_t valB = ereg->get(E_VALB);
   uint64_t dstm = ereg->get(E_DSTM);
   e_valE = aluCombinational(icode, ereg, wreg);
   e_Cnd = cond(icode, ifun);
   e_dstE = get_e_dstE(icode, ereg);
   calculateControlSignals(wreg);

   setMInput(mreg, stat, icode, e_Cnd, e_valE, valA, e_dstE, dstm);

   return false;
}

void ExecuteStage::setMInput(PipeReg * mreg, uint64_t M_stat, uint64_t M_icode, 
                           uint64_t e_Cnd, uint64_t M_valE,  uint64_t M_valA, 
                           uint64_t M_dste, uint64_t M_dstm)
{
   mreg->set(M_STAT, M_stat);
   mreg->set(M_ICODE, M_icode);
   mreg->set(M_CND, e_Cnd);
   mreg->set(M_VALE, M_valE);
   mreg->set(M_VALA, M_valA);
   mreg->set(M_DSTE, M_dste);
   mreg->set(M_DSTM, M_dstm);
}

/* doClockHigh
 *
 * 
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
*/
void ExecuteStage::doClockHigh(PipeReg ** pregs)
{
   PipeReg * ereg = pregs[EREG];
   PipeReg * mreg = pregs[MREG];
   PipeReg * wreg = pregs[WREG];

   if(M_bubble) ((M *)mreg)->bubble();
   else mreg->normal();
}

/* aluA - used to choose the value of op1 in the ALU.
 *
 * 
 * @param: PipeReg * ereg - array of the pipeline register (for the E instance)
 * @param: uint64_t code  - the code that is passed into the the system
 */
uint64_t ExecuteStage::aluA(uint64_t icode, PipeReg * ereg){
   if (icode == IRRMOVQ || icode == IOPQ){
      return ereg->get(E_VALA);
   }
   if (icode == IIRMOVQ || icode == IRMMOVQ || icode == IMRMOVQ){
      return ereg->get(E_VALC);
   }
   if (icode == ICALL || icode == IPUSHQ){
      return -8;
   }
   if (icode == IRET || icode == IPOPQ){
      return 8;
   } 
   return 0;
}

/* aluA - used to choose the value of op2 in the ALU.
 *
 * 
 * @param: PipeReg * ereg - array of the pipeline register (for the E instance)
 * @param: uint64_t code  - the code that is passed into the the system
 */
uint64_t ExecuteStage::aluB(uint64_t icode, PipeReg * ereg){
   if (icode == IRMMOVQ || icode == IMRMOVQ || icode == IOPQ || icode == ICALL || icode == IPUSHQ || icode == IRET || icode == IPOPQ){
      return ereg->get(E_VALB);
   }
   return 0;
}

/* aluB - used to choose the value of op2 in the ALU.
 *
 * 
 * @param: PipeReg * ereg - array of the pipeline register (for the E instance)
 * @param: uint64_t code  - the code that is passed into the the system
 */
uint64_t ExecuteStage::aluFun(uint64_t icode, PipeReg * ereg){
   if(icode == IOPQ){
      return ereg->get(E_IFUN);
   }
   return ADDQ;
} 

/* set_cc - used to see if the instruction in current use is an operation if it is
 * returns True
 *
 * 
 * @param: uint64_t code  - the code that is passed into the the system
 * @param: PipeReg * wreg - array of the pipeline register (for the W instance)
 */
bool ExecuteStage::set_cc(uint64_t icode, PipeReg * wreg){
   if(icode == IOPQ && (!mStatChecker()) && (!wStatChecker(wreg))){
      return true;
   }
   return false;
}

/* get_e_dstE - gets the value of the e_dstE
 *
 * 
 * @param: uint64_t icode  - the code that is passed into the the system
 * @param: PipeReg * ereg - array of the pipeline register (for the E instance)
 */
uint64_t ExecuteStage::get_e_dstE(uint64_t icode, PipeReg * ereg){
   if(icode == IRRMOVQ && !(e_Cnd)){
      return RNONE;
   }
   return ereg->get(E_DSTE);
} 

/* ccCombinational - ccCombinational checks if the icode is an IOPQ then
 * moves on to set the ConditionCode using setConditionCode from the 
 * conditionCode.C 
 * 
 * @param: uint64_t icode  - the icode that is passed into the the system
 * @param: uint32_t cNum   - the cNum that is passed into setConditionCodes
 * @param: bool status     - the status is either a 1 or a 0 depending on if the flag is set
 */
void ExecuteStage::ccCombinational(uint64_t icode, uint32_t cNum, bool status, PipeReg * wreg){
   if(set_cc(icode, wreg)){
      bool error = false;
      cc->setConditionCode(status, cNum, error);
   }
}

/* aluCombinational - aluCombinational is used to do the arthmetic math and 
 * sets the flags to 0 or 1 based on if their conditions are true of false
 *
 *
 * @param: uint64_t icode  - the icode that is passed into the the system
 * @param: PipeReg * ereg - array of the pipeline register (for the E instance)
 */
uint64_t ExecuteStage::aluCombinational(uint64_t icode, PipeReg * ereg, PipeReg * wreg){
   uint64_t aluFunction = aluFun(icode, ereg);
   uint64_t op1 = aluA(icode, ereg); 
   uint64_t op2 = aluB(icode, ereg);
   uint64_t result;
   uint64_t oF = 0;
   uint64_t sF = 0;
   switch(aluFunction){
      case SUBQ:
         result = op2 - op1;
         oF = Tools::subOverflow(op1, op2);
         sF = Tools::sign(result);
         break;
      case ANDQ:
         result = op1 & op2;
         sF = Tools::sign(result);
         break;
      case XORQ:
         result = op1 ^ op2;
         sF = Tools::sign(result);
         break;
      default:
         result = op2 + op1;
         oF = Tools::addOverflow(op1, op2);
         sF = Tools::sign(result);
   }
   ccCombinational(icode, SF, sF, wreg);
   ccCombinational(icode, ZF, !result, wreg);
   ccCombinational(icode, OF, oF, wreg);
   return result;
}

/* cond - aluCombinational is used to do the arthmetic math and 
 * sets the flags to 0 or 1 based on if their conditions are true of false
 *
 *
 * @param: uint64_t icode  - the icode that is passed into the the system from doClockLow
 * @param: uint64_t ifun  - the ifun that is passed into the the system from doClockLow
 */
uint64_t ExecuteStage::cond(uint64_t icode, uint64_t ifun){
   if(icode != IJXX && icode != ICMOVXX){
      return 0;
   }
   bool error = false;
   bool oF = cc->getConditionCode(OF, error);
   bool sF = cc->getConditionCode(SF, error);
   bool zF = cc->getConditionCode(ZF, error);

   switch(ifun){
      case 0:
         return 1;
      case 1:
         return (sF ^ oF) | zF;
      case 2:
         return (sF ^ oF);
      case 3:
         return zF;
      case 4:
         return !zF;
      case 5:
         return !(sF ^ oF);
      case 6:
         return !(sF ^ oF) & !zF;
   }
   
   return 0;
}

/* calculateControlSignals - checks if there is a bubble in the pipeline
 *
 *
 * @param: PipeReg * wreg - array of the pipeline register (for the W instance)
 */
void ExecuteStage::calculateControlSignals (PipeReg * wreg){
   if (mStatChecker() || wStatChecker(wreg)) M_bubble = true;
   else M_bubble = false;
}

/* mStatChecker - checks if m stat is SADR, SINS, or SHLT 
 *
 *
 * @param: PipeReg * wreg - array of the pipeline register (for the W instance)
 */
bool ExecuteStage::mStatChecker(){
   if (m_stat == SADR || m_stat == SINS || m_stat == SHLT) return true;
   return false;
}

/* wStatChecker - checks if w stat is SADR, SINS, or SHLT 
 *
 *
 * @param: PipeReg * wreg - array of the pipeline register (for the W instance)
 */
bool ExecuteStage::wStatChecker (PipeReg * wreg){
   uint64_t w_stat = wreg->get(W_STAT);
   if(w_stat == SADR || w_stat == SINS || w_stat == SHLT) return true;
   return false;
}

