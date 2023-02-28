#include <cstdint>
#include <string>
#include "PipeRegField.h"
#include "PipeReg.h"
#include "Memory.h"
#include "RegisterFile.h"
#include "ConditionCodes.h"
#include "Stage.h"

// Initialize the static data members that are accessible 
// to all descendant classes 
Memory * Stage::mem = NULL;
RegisterFile * Stage::rf = NULL;
ConditionCodes * Stage::cc = NULL;
uint64_t Stage::d_srcA = RNONE; 
uint64_t Stage::d_srcB = RNONE; 
uint64_t Stage::e_Cnd = 0;
uint64_t Stage::e_valE = 0;
uint64_t Stage::e_dstE = RNONE; 
uint64_t Stage::m_stat = 0;
uint64_t Stage::m_valM = 0;

Stage::Stage()
{
   //Initialize the pointers to singletons
   mem = Memory::getInstance();
   cc = ConditionCodes::getInstance();
   rf = RegisterFile::getInstance();
}
