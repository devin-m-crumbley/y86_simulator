//class to represent the W pipeline register

//indices into the inherited fields array
#define W_STAT 0
#define W_ICODE 1
#define W_VALE 2
#define W_VALM 3
#define W_DSTE 4
#define W_DSTM 5

//size of inherited fields array
#define W_NUMFIELDS 6

class W : public PipeReg //inherits from PipeReg
{
   public:
      W();
      void bubble();
      void dump();
};
