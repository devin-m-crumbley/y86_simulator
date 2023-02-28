class WritebackStage: public Stage
{
   public:
      bool doClockLow(PipeReg ** pregs);
      void doClockHigh(PipeReg ** pregs);
   private:
      void setDInput(PipeReg * dreg, uint64_t vala, uint64_t valb, 
                           uint64_t dste, uint64_t srca,  uint64_t dstm, 
                           uint64_t srcb);
};