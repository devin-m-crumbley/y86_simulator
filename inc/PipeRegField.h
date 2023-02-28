//class to represent a single pipeline register field

class PipeRegField
{
   private: 
      uint64_t input;   //current input to the register
      uint64_t state;   //current state (output)
   public:
      //default state is 0 if none is provided
      PipeRegField(int state = 0); 
      void setInput(uint64_t input);
      uint64_t getOutput();
      void normal();
      //default state is 0 if none is provided
      void setState(int state = 0);
};
 

  
