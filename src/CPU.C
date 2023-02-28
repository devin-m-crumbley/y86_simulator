/* 
 * CPU class
 *
 * The CPU class contains objects to represent the FetchStage, DecodeStage,
 * ExecuteStage, MemoryStage, and Writeback Stages. These classes contain the
 * methods to simulate the combinational logic performed by the PIPE machine.
 * In addition, the CPU class contains F, D, E, M, and W objects
 * to represent the pipelined registers that provide the input and
 * accept the output of these stages.
*/
 
#include <iomanip>
#include <iostream>
#include "Memory.h"
#include "RegisterFile.h"
#include "ConditionCodes.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "F.h"
#include "D.h"
#include "E.h"
#include "M.h"
#include "W.h"
#include "Stage.h"
#include "MemoryStage.h"
#include "ExecuteStage.h"
#include "DecodeStage.h"
#include "FetchStage.h"
#include "WritebackStage.h"
#include "CPU.h"

/*
 * CPU constructor
 *
 * creates instances of each data member
*/
CPU::CPU()
{
   // pipeline stages 
   stages = new Stage * [NUMSTAGES];
   stages[FSTAGE] = new FetchStage();
   stages[DSTAGE] = new DecodeStage();
   stages[ESTAGE] = new ExecuteStage();
   stages[MSTAGE] = new MemoryStage();
   stages[WSTAGE] = new WritebackStage();

   // pipelined registers 
   pregs = new PipeReg * [NUMPIPEREGS];
   pregs[FREG] = new F();
   pregs[DREG] = new D();
   pregs[EREG] = new E();
   pregs[MREG] = new M();
   pregs[WREG] = new W();

   //initialize pointers to singleton objects
   mem = Memory::getInstance();
   cc = ConditionCodes::getInstance();
   rf = RegisterFile::getInstance();
}

/* 
 * run
 * 
 * CPU the stages of the PIPE machine until a halt is executed.
*/
void CPU::run()
{
   int cycle = 0;
   bool stop = false;

   while (!stop)
   {
      stop = doClockLow();
      doClockHigh();

      // dump the values of the pipeline registers, Condition Codes,
      // Register File, and Memory 
      std::cout << "\nAt end of cycle " << std::dec 
                << cycle << ":" << std::endl;
      dumpPipeRegs();
      cc->dump();
      rf->dump();
      mem->dump();
      cycle++;
   }
}

/*
 * doClockLow
 *
 * When the clock is low, the stages use their inputs
 * to calculate the outputs.
*/
bool CPU::doClockLow()
{
   bool stop;

   //going through the stages in reverse order helps to
   //simulate the parallel behavior of the hardware
   stop = stages[WSTAGE]->doClockLow(pregs);
   stages[MSTAGE]->doClockLow(pregs);
   stages[ESTAGE]->doClockLow(pregs);
   stages[DSTAGE]->doClockLow(pregs);
   stages[FSTAGE]->doClockLow(pregs);
   return stop;
}

/*
 * doClockHigh
 *
 * When the clock rises, the pipelined registers are updated.
 * The register file is also updated.
 * The code to update the pipelined registers is in the
 * pipe stage class.
*/
void CPU::doClockHigh()
{
   //get the WritebackStage to update the register file
   stages[WSTAGE]->doClockHigh(pregs);

   //get the MemoryStage to update the W register
   stages[MSTAGE]->doClockHigh(pregs);

   //get the ExecuteStage to update the M register
   stages[ESTAGE]->doClockHigh(pregs);

   //get the DecodeStage to update the E register
   stages[DSTAGE]->doClockHigh(pregs);

   //get the FetchStage to update the F and D registers
   stages[FSTAGE]->doClockHigh(pregs);
}

/*
 * dumpPipeRegs
 *
 * Display the contents of the pipelined registers
*/
void CPU::dumpPipeRegs()
{
   pregs[FREG]->dump();
   pregs[DREG]->dump();
   pregs[EREG]->dump();
   pregs[MREG]->dump();
   pregs[WREG]->dump();
}
