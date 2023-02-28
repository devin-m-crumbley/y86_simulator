//Driver class for the yess simulator
class CPU
{
   private:
      PipeReg ** pregs;    //array of pipe registers: F, D, E, M, W
      Stage ** stages;     //array of stages: FetchStage, DecodeStage, ExecuteStage,
                           //                 MemoryStage, WritebackStage
      Memory * mem;        //pointer to Memory singleton
      RegisterFile * rf;   //pointer to RegisterFile singleton
      ConditionCodes * cc; //pointer to ConditionCodes singleton

      //methods called by the public run method
      bool doClockLow();
      void doClockHigh();
      void dumpPipeRegs();
   public:
      CPU();
      void run();
};
