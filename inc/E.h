//class to represent the E pipeline register

//indices into the inherited fields array
#define E_STAT 0
#define E_ICODE 1
#define E_IFUN 2
#define E_VALC 3
#define E_VALA 4
#define E_VALB 5
#define E_DSTE 6
#define E_DSTM 7
#define E_SRCA 8
#define E_SRCB 9

//size of inherited fields array
#define E_NUMFIELDS 10

class E : public PipeReg //inherits from PipeReg
{
   public:
      E();
      void bubble();
      void dump();
};
