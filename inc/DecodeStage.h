class DecodeStage: public Stage
{
   public:
      bool doClockLow(PipeReg ** pregs);
      void doClockHigh(PipeReg ** pregs);
   private:
      bool E_bubble;
      void setEInput(PipeReg * ereg, uint64_t stat, uint64_t icode, 
                     uint64_t ifun, uint64_t valc, uint64_t vala, uint64_t valb, 
                     uint64_t dste, uint64_t srca,  uint64_t dstm, 
                     uint64_t srcb);
      uint64_t dsrcA(PipeReg * dreg, uint64_t code);
      uint64_t dsrcB(PipeReg * dreg, uint64_t code);
      uint64_t d_dstE(PipeReg * dreg, uint64_t code);
      uint64_t d_dstM(PipeReg * dreg, uint64_t code);
      uint64_t d_FwdA(PipeReg * dreg, PipeReg * mreg, PipeReg * wreg, uint64_t srcA, bool error, uint64_t icode);
      uint64_t d_FwdB(PipeReg * mreg, PipeReg * wreg, uint64_t srcB, bool error);
      void calculateControlSignals (PipeReg * ereg);




};