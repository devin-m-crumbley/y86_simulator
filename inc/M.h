//class to represent the M pipeline register

//indices into the inherited fields array
#define M_STAT 0
#define M_ICODE 1
#define M_CND 2
#define M_VALE 3
#define M_VALA 4
#define M_DSTE 5
#define M_DSTM 6

//size of inherited fields array
#define M_NUMFIELDS 7

class M : public PipeReg //inherits from PipeReg
{
   public:
      M();
      void bubble();
      void dump();
};
