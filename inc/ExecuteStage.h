class ExecuteStage: public Stage
{
   public:
      bool doClockLow(PipeReg ** pregs);
      void doClockHigh(PipeReg ** pregs);
   private:
      bool M_bubble;
      void setMInput(PipeReg * mreg, uint64_t stat, uint64_t icode, 
                           uint64_t cnd, uint64_t vale,  uint64_t vala, 
                           uint64_t dste, uint64_t dstm);
      uint64_t get_e_dstE(uint64_t icode, PipeReg * ereg);
      uint64_t aluA(uint64_t icode, PipeReg * ereg);
      uint64_t aluB(uint64_t icode, PipeReg * ereg);
      uint64_t aluFun(uint64_t icode, PipeReg * ereg);
      void ccCombinational(uint64_t icode, uint32_t cNum, bool cc, PipeReg * wreg);
      bool set_cc(uint64_t icode, PipeReg * wreg);
      uint64_t aluCombinational(uint64_t icode, PipeReg * ereg, PipeReg * wreg);
      uint64_t cond(uint64_t icode, uint64_t ifun);
      bool mStatChecker();
      bool wStatChecker (PipeReg * wreg);
      void calculateControlSignals (PipeReg * wreg);

};