#include <iostream>
#include <cstdint>
#include <stdio.h>
#include "PipeRegField.h"
#include "PipeReg.h"
#include "Memory.h"
#include "RegisterFile.h"
#include "ConditionCodes.h"
#include "Stage.h"
#include "DecodeStage.h"
#include "Debug.h"
#include "D.h"
#include "E.h"
#include "M.h"
#include "W.h"
#include "Instructions.h"

/*
 * doClockLow
 *
 * Performs the Decode stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
bool DecodeStage::doClockLow(PipeReg ** pregs)
{
   bool error = false;
   PipeReg * dreg = pregs[DREG];
   PipeReg * ereg = pregs[EREG];
   PipeReg * mreg = pregs[MREG];
   PipeReg * wreg = pregs[WREG];
   uint64_t stat = dreg->get(D_STAT);
   uint64_t icode = dreg->get(D_ICODE);
   uint64_t ifun = dreg->get(D_IFUN);
   uint64_t valc = dreg->get(D_VALC);
   uint64_t E_dste = d_dstE(dreg, icode);
   uint64_t E_dstm = d_dstM(dreg, icode);
   d_srcA = dsrcA(dreg, icode);
   d_srcB = dsrcB(dreg, icode);   
   uint64_t E_valA = d_FwdA(dreg, mreg, wreg, d_srcA, error, icode);
   uint64_t E_valB = d_FwdB(mreg, wreg, d_srcB, error);

   calculateControlSignals(ereg);


   setEInput(ereg, stat, icode, ifun, valc, E_valA, E_valB, E_dste, E_dstm, d_srcA, d_srcB);

   return false;
}

void DecodeStage::setEInput(PipeReg * ereg, uint64_t E_stat, uint64_t E_icode, 
                     uint64_t E_ifun, uint64_t E_valC, uint64_t E_valA, uint64_t E_valB, 
                     uint64_t E_dste, uint64_t E_dstm,  uint64_t E_srcA, 
                     uint64_t E_srcB)
{
   ereg->set(E_STAT, E_stat);
   ereg->set(E_ICODE, E_icode);
   ereg->set(E_IFUN, E_ifun);
   ereg->set(E_VALC, E_valC);
   ereg->set(E_VALA, E_valA);
   ereg->set(E_VALB, E_valB);
   ereg->set(E_DSTE, E_dste);
   ereg->set(E_DSTM, E_dstm);
   ereg->set(E_SRCA, E_srcA);
   ereg->set(E_SRCB, E_srcB);
}

/* doClockHigh
 *
 * 
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
*/
void DecodeStage::doClockHigh(PipeReg ** pregs)
{
   PipeReg * ereg = pregs[EREG];
   if(E_bubble) ((E *)ereg)->bubble();
   else ereg->normal();
}

/* d_srcB
 *
 * 
 * @param: dregs - array of the pipeline register (for the D instance)
 * @param: uint64_t code  - the code that is passed into the the system
*/
uint64_t DecodeStage::dsrcA(PipeReg * dreg, uint64_t code){
   if(code == IRRMOVQ || code == IRMMOVQ || code == IOPQ || code  == IPUSHQ){
      return dreg->get(D_RA); 
   }
   else if (code == IPOPQ || code == IRET){
      return RSP;
   }
   return RNONE;
}

/* d_srcB
 *
 * 
 * @param: PipeReg * dreg - array of the pipeline register (for the D instance)
 * @param: uint64_t code  - the code that is passed into the the system
*/
uint64_t DecodeStage::dsrcB(PipeReg * dreg, uint64_t code){
   if(code == IOPQ || code == IRMMOVQ || code == IMRMOVQ){
      return dreg->get(D_RB);
   }
   else if(code == IPUSHQ || code == IPOPQ || code == ICALL || code == IRET){
      return RSP;
   }
   return RNONE;
}

/* d_dstE
 *
 * 
 * @param: PipeReg * dreg - array of the pipeline register (for the D instance)
 * @param: uint64_t code  - the code that is passed into the the system
*/
uint64_t DecodeStage::d_dstE(PipeReg * dreg, uint64_t code){
   if(code == IRRMOVQ || code == IIRMOVQ || code == IOPQ){
      return dreg->get(D_RB);
   }
   else if(code == IPUSHQ || code == IPOPQ || code == ICALL || code == IRET){
      return RSP;
   }
   return RNONE;
}

/* d_dstM
 *
 * 
 * @param: PipeReg * dreg - array of the pipeline register (for the D instance)
 * @param: uint64_t code  - the code that is passed into the the system
 */
uint64_t DecodeStage::d_dstM(PipeReg * dreg, uint64_t code){
   if(code == IMRMOVQ || code == IPOPQ){
      return dreg->get(D_RA);
   }
   return RNONE;
}

/* d_FwdA
 *
 * 
 * @param: PipeReg * mreg - array of the pipeline register (for the M instance)
 * @param: PipeReg * wreg - array of the pipeline register (for the W instance)
 * @param: uint64_t srcA  - d_srcA method gives this value allowing srcA to pass through from do clock low
 * @param: uint64_t icode  - icode the code that is passed into the the system
 * @param: uint64_t error - the error that is passed into the method
 */
uint64_t DecodeStage::d_FwdA(PipeReg * dreg, PipeReg * mreg, PipeReg * wreg, uint64_t srcA, bool error, uint64_t icode){
   if(icode == ICALL || icode == IJXX) return dreg->get(D_VALP);
   if (srcA == RNONE) return 0;
   RegisterFile * name = RegisterFile::getInstance();
   if (srcA == e_dstE) return e_valE;   
   if (srcA == mreg->get(M_DSTM)) return m_valM;
   if (srcA == mreg->get(M_DSTE)) return mreg->get(M_VALE);
   if (srcA == wreg->get(W_DSTM)) return wreg->get(W_VALM);
   if (srcA == wreg->get(W_DSTE)) return wreg->get(W_VALE);
   return name->readRegister(srcA, error); //Value from register file
}

/* d_FwdB
 *
 * 
 * @param: PipeReg * mreg - array of the pipeline register (for the M instance)
 * @param: PipeReg * wreg - array of the pipeline register (for the W instance)
 * @param: uint64_t srcB  - d_srcB method gives this value allowing srcB to pass through from do clock low
 * @param: uint64_t error - the error that is passed into the method
 */
uint64_t DecodeStage::d_FwdB(PipeReg * mreg, PipeReg * wreg, uint64_t srcB, bool error){
   if (srcB == RNONE) return 0;
   RegisterFile * name = RegisterFile::getInstance();
   if (srcB == e_dstE) return e_valE;
   if (srcB == mreg->get(M_DSTM)) return m_valM;
   if (srcB == mreg->get(M_DSTE)) return mreg->get(M_VALE);
   if (srcB == wreg->get(W_DSTM)) return wreg->get(W_VALM);
   if (srcB == wreg->get(W_DSTE)) return wreg->get(W_VALE);
   return name->readRegister(srcB, error);
}

/* calculateControlSignals - checks if there is a bubble in the pipeline
 *
 * bool E_bubble = ( E_icode == IJXX && !e_Cnd ) ||  
 *                 ( E_icode in { IMRMOVQ, IPOPQ } &&  E_dstM in { d_srcA, d_srcB } );
 * 
 * @param: PipeReg * ereg - array of the pipeline register (for the E instance)
 */
void DecodeStage::calculateControlSignals (PipeReg * ereg){
   uint64_t E_icode = ereg->get(E_ICODE);
   uint64_t E_dstM = ereg->get(E_DSTM);
   if ((E_icode == IJXX && !e_Cnd) || ((E_icode == IMRMOVQ || E_icode == IPOPQ) && (E_dstM == d_srcA || E_dstM == d_srcB))) E_bubble = true;
   else E_bubble = false;
}