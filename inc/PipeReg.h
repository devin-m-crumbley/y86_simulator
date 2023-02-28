
//indices into an array of PipeReg
#define FREG 0
#define DREG 1
#define EREG 2
#define MREG 3
#define WREG 4

//number of PipeRegisters
#define NUMPIPEREGS 5

//base class for the F, D, E, M, W pipeline registers
class PipeReg
{
   public:
      //dump method is implemented in the classes that descend
      //from PipeReg
      //
      //setting dump to 0 makes it abstract
      //virtual makes it polymorphic 
      virtual void dump() = 0;
      uint64_t get(int32_t which);
      void set(int32_t which, uint64_t value);
      void normal();
   protected:
      //protected data and methods can be accessed by descendant classes
      PipeRegField ** fields;
      int32_t numFields;
      void dumpField(std::string label, int width, uint64_t value, bool nl);
};

