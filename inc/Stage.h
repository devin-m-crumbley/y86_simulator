//parent class of concrete Stage objects

//these #defines can be used as indices in an array of Stage objects
#define FSTAGE 0
#define DSTAGE 1
#define ESTAGE 2
#define MSTAGE 3
#define WSTAGE 4

//five stages: FetchStage, DecodeStage, ExecuteStage,
//             MemoryStage, WritebackStage
#define NUMSTAGES 5

class Stage
{
   protected:
      //make these static so there is just one instance
      //shared among all descendant classes
      static Memory * mem;
      static RegisterFile * rf;
      static ConditionCodes * cc;

      //values calculated by the DecodeStage that are needed by the FetchStage
      static uint64_t d_srcA;
      static uint64_t d_srcB;

      //values calculated by the ExecuteStage that needed by the DecodeStage
      static uint64_t e_dstE;
      static uint64_t e_valE; 
      static uint64_t e_Cnd;

      //values calculated by the MemoryStage 
      static uint64_t m_stat;  //needed by the ExecuteStage
      static uint64_t m_valM;  //needed by the DecodeStage
   public:
      Stage();
      //abstract methods implemented in the descendant classes
      //virtual makes these methods polymorphic       
      virtual bool doClockLow(PipeReg ** pregs) = 0;
      virtual void doClockHigh(PipeReg ** pregs) = 0;
};


