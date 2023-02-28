//class to represent the Fetch stage

class FetchStage: public Stage
{
   private:
      bool F_stall;
      bool D_stall;
      bool D_bubble;
      //TODO: provide declarations for new methods
      uint64_t selectPC(PipeReg * mreg, PipeReg * wreg, PipeReg * freg);
      uint64_t predictPC(u_int64_t f_icode, u_int64_t f_valC, u_int64_t f_valP);
      bool needValC(uint64_t f_icode);
      bool needRegIds(uint64_t f_icode);
      uint64_t pcIncrement(uint64_t f_PC, bool needReg, bool needVal);
      uint64_t instr_valid(uint64_t icode);
      uint64_t f_stat(uint64_t icode, bool error);
      //set the input value for each fieldof the D pipeline register
      void setDInput(PipeReg * dreg, uint64_t stat, uint64_t icode, 
                     uint64_t ifun, uint64_t rA, uint64_t rB,
                     uint64_t valC, uint64_t valP);
      void getRegIds(uint64_t * ra, uint64_t * rb, uint64_t f_PC, bool error);
      uint64_t buildValC(uint64_t f_PC, bool error, bool needReg);
   public:
      //These are the only methods that are called outside of the class
      bool doClockLow(PipeReg ** pregs);  
      void doClockHigh(PipeReg ** pregs);
      void checkFstall(PipeReg * dreg, PipeReg * ereg, PipeReg * mreg);
      void checkDstall(PipeReg * ereg);
      void calculateControlSignals (PipeReg * dreg, PipeReg * ereg, PipeReg * mreg);
      void checkDbubble(PipeReg * dreg, PipeReg * ereg, PipeReg * mreg);
      bool checkCodes(PipeReg * dreg, PipeReg * ereg, PipeReg * mreg);
      bool checkIJXX(PipeReg * ereg);



};
