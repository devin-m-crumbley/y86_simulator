//class to represent the F pipeline register

//index into the inherited fields array
#define F_PREDPC 0

//size of inherited fields array
#define F_NUMFIELDS 1

class F : public PipeReg //inherits from PipeReg
{
   public:
      F();
      void normal();
      void dump();
};
