//TODO add more #includes as you need them
#include <iostream>
#include <cstdint>
#include <stdio.h>
#include "Memory.h"
#include "ConditionCodes.h"
#include "Instructions.h"
#include "RegisterFile.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "F.h"
#include "D.h"
#include "E.h"
#include "M.h"
#include "W.h"
#include "Stage.h"
#include "FetchStage.h"
#include "Status.h"
#include "Debug.h"
#include "Tools.h"

/*
 * doClockLow
 *
 * Performs the Fetch stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
bool FetchStage::doClockLow(PipeReg ** pregs)
{
   PipeReg * freg = pregs[FREG];  //pointer to object representing F pipeline register
   PipeReg * dreg = pregs[DREG];  //pointer to object representing D pipeline register
   PipeReg * ereg = pregs[EREG];  //pointer to object representing E pipeline register
   PipeReg * mreg = pregs[MREG];  //pointer to object representing M pipeline register
   PipeReg * wreg = pregs[WREG];  //pointer to object representing W pipeline register
   bool mem_error = false;
   uint64_t icode = INOP, ifun = FNONE, rA = RNONE, rB = RNONE;
   uint64_t valC = 0, valP = 0, stat = 0, predPC = 0;
   bool needvalC = false;
   bool needregId = false;
   //TODO 
   //select PC value and read byte from memory
   //set icode and ifun using byte read from memory
   //uint64_t f_pc =  .... call your select pc function
   uint64_t f_pc = selectPC(mreg, wreg, freg);     
   uint64_t first = mem->getByte(f_pc, mem_error);
   if (mem_error){
      icode = INOP;
      ifun = FNONE;
   }
   ifun = Tools::getBits(first, 0, 3);
   icode = Tools::getBits(first, 4, 7);

   //status of this instruction is SAOK (this will change in a later lab)
   stat = f_stat(icode, mem_error);

   //TODO
   //In order to calculate the address of the next instruction,
   //you'll need to know whether this current instruction has an
   //immediate field and a register byte. (Look at the instruction encodings.)
   //needvalC =  .... call your need valC function
   //needregId = .... call your need regId function
   needvalC = needValC(icode);
   needregId = needRegIds(icode);
   //TODO
   //determine the address of the next sequential function
   //valP = ..... call your PC increment function 
   valP = pcIncrement(f_pc, needregId, needvalC);

   //gets the valC value for the input to the d register
   if (needvalC){
      valC = buildValC(f_pc, mem_error, needregId);
   }
   //TODO
   //calculate the predicted PC value
   //predPC = .... call your function that predicts the next PC   
   predPC = predictPC(icode, valC, valP);
   //set the input for the PREDPC pipe register field in the F register
   freg->set(F_PREDPC, predPC);
 
   //gets the regIds for the input to the d register 
   if (needregId){
      getRegIds(&rA, &rB, f_pc, mem_error);
   }


   calculateControlSignals(dreg, ereg, mreg);

   //set the inputs for the D register
   setDInput(dreg, stat, icode, ifun, rA, rB, valC, valP);
   return false;
}

/* doClockHigh
 *
 * applies the appropriate control signal to the F
 * and D register intances
 * 
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
*/
void FetchStage::doClockHigh(PipeReg ** pregs)
{
   PipeReg * freg = pregs[FREG];  //pointer to 
   PipeReg * dreg = pregs[DREG];
   if (!F_stall) freg->normal();
   if (D_bubble) ((D *)dreg)->bubble();
   else if (!D_stall) dreg->normal();
}

/* setDInput
 * provides the input to potentially be stored in the D register
 * during doClockHigh
 *
 * @param: dreg - pointer to the D register instance
 * @param: stat - value to be stored in the stat pipeline register within D
 * @param: icode - value to be stored in the icode pipeline register within D
 * @param: ifun - value to be stored in the ifun pipeline register within D
 * @param: rA - value to be stored in the rA pipeline register within D
 * @param: rB - value to be stored in the rB pipeline register within D
 * @param: valC - value to be stored in the valC pipeline register within D
 * @param: valP - value to be stored in the valP pipeline register within D
*/
void FetchStage::setDInput(PipeReg * dreg, uint64_t stat, uint64_t icode, 
                           uint64_t ifun, uint64_t rA, uint64_t rB,
                           uint64_t valC, uint64_t valP)
{
   dreg->set(D_STAT, stat);
   dreg->set(D_ICODE, icode);
   dreg->set(D_IFUN, ifun);
   dreg->set(D_RA, rA);
   dreg->set(D_RB, rB);
   dreg->set(D_VALC, valC);
   dreg->set(D_VALP, valP);
}

//TODO
//Write your selectPC, needRegIds, needValC, PC increment, and predictPC methods
//Remember to add declarations for these to FetchStage.h


// Here is the HCL describing the behavior for some of these methods. 
/*

//selectPC method: input is F, M, and W registers
word f_pc = [
    M_icode == IJXX && !M_Cnd : M_valA;
    W_icode == IRET : W_valM;
    1: F_predPC;
];

//needRegIds  method: input is f_icode
bool need_regids = f_icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, IIRMOVQ, IRMMOVQ, IMRMOVQ };

//needValC method: input is f_icode
bool need_valC = f_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL };

//predictPC method: inputs are f_icode, f_valC, f_valP
word f_predPC = [
    f_icode in { IJXX, ICALL } : f_valC;
    1: f_valP;
];

*/

/* selectPC - selectPC selects where the pc counter is.
 * 
 * @param: mreg - pointer to the M register instance
 * @param: wreg - pointer to the W register instance
 * @param: freg - pointer to the F register instance
 */
uint64_t FetchStage::selectPC(PipeReg * mreg, PipeReg * wreg, PipeReg * freg){
   if (mreg->get(M_ICODE) == IJXX && !mreg->get(M_CND)){
      return mreg->get(M_VALA);
   }
   else if(wreg->get(W_ICODE) == IRET){
      return wreg->get(W_VALM);
   }
   return freg->get(F_PREDPC);
}

/* selectPC - selectPC selects where the pc counter is.
 * 
 * @param: f_icode - icode that is passed into this method via the doclocklow
 * @param: f_valC  - valC that is passed into this method via the doclock low method
 * @param: f_valP  - valP that is passed into this method via the doclock low method holding the predicted PC-counter
 */
uint64_t FetchStage::predictPC(uint64_t f_icode, uint64_t f_valC, uint64_t f_valP){
      if (f_icode == IJXX || f_icode == ICALL){
         return f_valC;
      }
      return f_valP;
}

/* needRegIds - needRegIds reads in if the needRegIds returns true if the code requires registers
 * 
 * @param: f_icode - icode that is passed into this method via the doclocklow
 */
bool FetchStage::needRegIds(uint64_t f_icode){
   switch(f_icode){
      case IRRMOVQ:
      case IOPQ:
      case IPUSHQ:
      case IPOPQ:
      case IIRMOVQ:
      case IRMMOVQ:
      case IMRMOVQ:
         return true;
      default:
         return false;
   }
}

/* needRegIds - reads in if the icode requires the valC
 * 
 * @param: f_icode - icode that is passed into this method via the doclocklow
 */
bool FetchStage::needValC(uint64_t f_icode){
   switch(f_icode){
      case IIRMOVQ:
      case IRMMOVQ:
      case IMRMOVQ:
      case IJXX:
      case ICALL:
         return true;
      default:
         return false; 
   }
}

/* pcIncrement - pcIncrement will increment the pc counter based on if the instruction
 * needs registers or if it needs valC
 * 
 * @param: f_PC    - the current pc counter passed in, in the doClockLow method
 * @param: needReg - variable from the needRegIds method to tell if we need Registers
 * @param: needVal - variable from the needValC method to tell if we need Registers
 */
uint64_t FetchStage::pcIncrement(uint64_t f_PC, bool needReg, bool needVal) {
   //if the code needs both Registers and ValC then increment the f_pc by 10
   if ((needReg && needVal)) {
      return f_PC + 10;
   }
   //if the code needs just ValC then increment the f_pc by 9
   else if (needVal){
      return f_PC + 9;
   }
   //if the code needs just Registers then increment the f_pc by 2
   else if (needReg){
      return f_PC + 2;
   }
   //if the code needs just nothing just increment the f_pc by 1
   return f_PC + 1;
}

/* getRegIds - getRegIds use pointers to rA and rB to set them to the correct method. 
 * 
 * @param: * ra        - pointer to the ra object in doClockLow
 * @param: * rb        - pointer to the rb object in doClockLow
 * @param: f_PC        - the current pc counter passed in, in the doClockLow method
 * @param: error       - error passed from the doClockLow method
 */
void FetchStage::getRegIds(uint64_t * ra, uint64_t * rb, uint64_t f_PC, bool error) {
   uint64_t regByte = mem->getByte(f_PC + 1, error);
   *ra = Tools::getBits(regByte, 4, 7);
   *rb = Tools::getBits(regByte, 0, 3);
}

/* buildValC - buildValC build the ValC using the getByte from them Memory class. 
 * 
 * @param: f_PC       - the current pc counter passed in, in the doClockLow method
 * @param: error      - error passed from the doClockLow method
 * @param: needRegIds - variable from the needRegIds method to tell if we need Registers
 */
uint64_t FetchStage::buildValC(uint64_t f_PC, bool error, bool needRegIds){
   uint8_t valCBytes [LONGSIZE];
   //if needRegIds == true then use f_PC incremented by 2
   if(needRegIds){
      f_PC += 2;
   }
   //if needRegIds != true then use f_PC incremented by 1
   else{
      f_PC += 1;
   }
   //get each byte of the valC and build it into the full long
   for (int i = 0; i <= 7; i++){
      valCBytes[i] = mem->getByte(f_PC + i, error);
   }
   return Tools::buildLong(valCBytes);
}

/* instr_valid - Checks if the insturction for the given icode is a valid instuction
 * 
 * HCL for instr_valid
 * 
 * bool instr_valid = f_icode in { INOP, IHALT, IRRMOVQ, IIRMOVQ, IRMMOVQ, IMRMOVQ, 
 *                                 IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ };
 * 
 * @param: f_icode - icode that is passed into this method via the doclocklow
 */
uint64_t FetchStage::instr_valid(uint64_t icode){
   switch (icode)
   {
   case INOP:
   case IHALT:
   case IRRMOVQ:
   case IIRMOVQ:
   case IRMMOVQ:
   case IMRMOVQ:
   case IOPQ:
   case IJXX:
   case ICALL:
   case IRET:
   case IPUSHQ:
   case IPOPQ:
      return true;
   default:
      return false;
   }
}

/* f_stat - Changes the stat to correct value
 * 
 * HCL for f_stat
 * 
 *  word f_stat = [
 *     mem_error : SADR;
 *     !instr_valid : SINS;
 *     f_icode == IHALT : SHLT;
 *     1 : SAOK;
 *  ];
 * 
 * @param: f_icode - icode that is passed into this method via the doclocklow
 * @param: error - holds the value of mem_error
 */
uint64_t FetchStage::f_stat(uint64_t icode, bool error){
   if (error){
      return SADR;
   }
   if (!instr_valid(icode)){
      return SINS;
   }
   if (icode == IHALT){
      return SHLT;
   }
   return SAOK;
}


/* F_stall - Returns true if Fstall should be invoked else false
 * 
 * HCL for F_stall
 * 
 *    bool F_stall = E_icode in { IMRMOVQ, IPOPQ } &&  E_dstM in { d_srcA, d_srcB } || 
 *              IRET in { D_icode, E_icode, M_icode }; 
 * 
 * @param: dreg - pointer to the D register instance
 * @param: ereg - pointer to the E register instance
 * @param: mreg - pointer to the M register instance
 */
void FetchStage::checkFstall(PipeReg * dreg, PipeReg * ereg, PipeReg * mreg){
   uint64_t destM = ereg->get(E_DSTM);
   uint64_t icode = ereg->get(E_ICODE);
   if ((icode == IMRMOVQ || icode == IPOPQ) && (destM == d_srcA || destM == d_srcB)) F_stall = true;
   else if(checkCodes(dreg, ereg, mreg)) F_stall = true;
   else F_stall = false;
}

/* D_stall - Returns true if Fstall should be invoked else false
 * 
 * HCL for D_stall
 * 
 * bool D_stall = E_icode in { IMRMOVQ, IPOPQ } &&  E_dstM in { d_srcA, d_srcB };
 * 
 * @param: ereg - pointer to the E register instance
 */
void FetchStage::checkDstall(PipeReg * ereg){
   uint64_t icode = ereg->get(E_ICODE);
   uint64_t destM = ereg->get(E_DSTM);
   if ((icode == IMRMOVQ || icode == IPOPQ) && (destM == d_srcA || destM == d_srcB)) D_stall = true;
   else D_stall = false;
}

/* calculateControlSignals - sets f_stall and d_stall repectivley for control signals
 * 
 * 
 * @param: ereg - pointer to the E register instance
 */
void FetchStage::calculateControlSignals (PipeReg * dreg, PipeReg * ereg, PipeReg * mreg){
   checkFstall(dreg, ereg, mreg);
   checkDstall(ereg);
   checkDbubble(dreg, ereg, mreg);
}

/* calculateControlSignals - sets f_stall and d_stall repectivley for control signals
 * 
 * bool D_bubble = ( E_icode == IJXX && !e_Cnd ) ||
 *               # Bubble while return passes through, but not if load/use hazard 
 *               !( E_icode in { IMRMOVQ, IPOPQ } && E_dstM in { d_srcA, d_srcB }) && 
 *               IRET in { D_icode, E_icode, M_icode };  
 * 
 * @param: dreg - pointer to the D register instance
 * @param: ereg - pointer to the E register instance
 * @param: mreg - pointer to the M register instance
 */
void FetchStage::checkDbubble(PipeReg * dreg, PipeReg * ereg, PipeReg * mreg){
   uint64_t icode = ereg->get(E_ICODE);
   uint64_t dstm = ereg->get(E_DSTM);
   if (icode == IJXX && !e_Cnd) D_bubble = true;
   else if (!((icode == IMRMOVQ || icode == IPOPQ) && (dstm == d_srcA || dstm == d_srcB)) && checkCodes(dreg, ereg, mreg)) D_bubble = true;
   else D_bubble = false;
}

/* checkCodes - checks if any of the Icodes for D,E,M registers are IRET
 * 
 * 
 * @param: dreg - pointer to the D register instance
 * @param: ereg - pointer to the E register instance
 * @param: mreg - pointer to the M register instance
 */
bool FetchStage::checkCodes(PipeReg * dreg, PipeReg * ereg, PipeReg * mreg){
   if(dreg->get(D_ICODE) == IRET || ereg->get(E_ICODE) == IRET || mreg->get(M_ICODE) == IRET ) return true;
   return false;
}

