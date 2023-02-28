class MemoryStage: public Stage
{
   public:
      bool doClockLow(PipeReg ** pregs);
      void doClockHigh(PipeReg ** pregs);
   private:
      void setWInput(PipeReg * wreg, uint64_t W_stat, uint64_t W_icode, 
                           uint64_t W_valE, uint64_t W_valM,  uint64_t W_dste, 
                           uint64_t W_dstm);
      uint64_t Addr(uint64_t icode, PipeReg * mreg);
      uint64_t Mem_read(uint64_t icode);
      uint64_t Mem_write(uint64_t icode);
   
};