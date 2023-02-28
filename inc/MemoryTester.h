
class MemoryTester
{
   private:
      void doClearMemoryTests();
      void doPutGetByteTests();
      void doPutGetLongTests();
      void doByteErrorTests();
      void doLongErrorTests();
      void doPutByteGetLongTests();
      Memory * mem;
   public:
      MemoryTester();
      void doMemoryTests();
};

