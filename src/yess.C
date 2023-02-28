/* 
 * Driver for the yess simulator
 * Usage: yess <file>.yo [-D]
 *
 * <file>.yo contains assembled y86-64 code.
 * If the -D option is provided then debug is set to 1.
 * The -D option can be used to turn on and turn off debugging print
 * statements.
*/

#include <iostream>
#include <fstream>
#include <string.h>
#include "Debug.h"
#include "Memory.h"
#include "String.h"
#include "Loader.h"
#include "RegisterFile.h"
#include "ConditionCodes.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "Stage.h"
#include "CPU.h"

int debug = 0;

int main(int argc, char * argv[])
{
   //check to see if the -D option was provided 
   if (argc >= 3 && (strcmp(argv[2], "-D") == 0)) debug = 1;

   Memory * mem = Memory::getInstance();
   Loader loadObj(argc, argv, mem);
   if (!loadObj.load())
   {
      if (mem != NULL) mem->dump();
      return 0;
   }
  
   //Code was loaded into memory
   //Now, create the CPU object and run the code
   CPU cpu;
   cpu.run(); 
   
   return 0;
}

