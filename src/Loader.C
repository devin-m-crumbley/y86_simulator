#include <iostream>
#include <fstream>
#include <string.h>
#include <ctype.h>
#include "Debug.h" 
#include "Memory.h"
#include "String.h"
#include "Loader.h"
#include "Tools.h"

//These are the constants used by the
//printErrMsg method.
#define USAGE 0          /* missing command line argument */ 
#define BADFILE 1        /* file has wrong suffix */
#define OPENERR 2        /* file doesn't open */
#define BADDATA 3        /* bad data record */
#define BADCOM 4         /* bad comment record */
#define NUMERRS 5

//useful defines to avoid magic numbers
#define ADDRBEGIN 2      /* beginning and ending indices for address */
#define ADDREND 4
#define DATABEGIN 7      /* beginning index for data bytes */ 
#define COMMENT 28       /* location of | */
#define MAXBYTES 10      /* max data bytes in a data record */

bool error;

/* 
 * Loader
 * Initializes the private data members
 */
Loader::Loader(int argc, char * argv[], Memory * mem)
{
   //this method is COMPLETE
   this->lastAddress = -1;   //keep track of last mem byte written to for error checking
   this->mem = mem;          //memory instance
   this->inputFile = NULL;   
   if (argc > 1) inputFile = new String(argv[1]);  //input file name
   error = false;
}

/*
 * printErrMsg
 * Prints an error message and returns false (load failed)
 * If the line number is not -1, it also prints the line where error occurred
 *
 * which - indicates error number
 * lineNumber - number of line in input file on which error occurred (if applicable)
 * line - line on which error occurred (if applicable)
 */
bool Loader::printErrMsg(int32_t which, int32_t lineNumber, String * line)
{
   //this method is COMPLETE
   static char * errMsg[NUMERRS] = 
   {
      (char *) "Usage: yess <file.yo>\n",                       //USAGE
      (char *) "Input file name must end with .yo extension\n", //BADFILE
      (char *) "File open failed\n",                            //OPENERR
      (char *) "Badly formed data record\n",                    //BADDATA
      (char *) "Badly formed comment record\n",                 //BADCOM
   };   
   if (which >= NUMERRS)
   {
      std::cout << "Unknown error: " << which << "\n";
   } else
   {
      std::cout << errMsg[which]; 
      if (lineNumber != -1 && line != NULL)
      {
         std::cout << "Error on line " << std::dec << lineNumber
              << ": " << line->get_stdstr() << std::endl;
      }
   } 
   return false; //load fails
}

/*
 * openFile
 * The name of the file is in the data member openFile (could be NULL if
 * no command line argument provided)
 * Checks to see if the file name is well-formed and can be opened
 * If there is an error, it prints an error message and returns false.
 * Otherwise, the file is opened and the function returns false
 *
 * modifies inf data member (file handle) if file is opened
 */
bool Loader::openFile()
{
   //TODO

   //If the user didn't supply a command line argument (inputFile is NULL)
   //then print the USAGE error message and return false
   if (inputFile == NULL){
      printErrMsg(USAGE, -1, NULL);
      return false;
   }
   
   //If the filename is badly formed (doesn't end in a .yo)
   //then print the BADFILE error message and return false
   
   //get the length
   //length -3 to find the . != .yo 

   if (inputFile->isSubString((char *) ".yo", (inputFile->get_stdstr().length() - 3), error) == false){
      printErrMsg(BADFILE, -1, NULL);
      return false;
   }

   
   //open the file using an std::ifstream open
   //if the file can't be opened then print the OPENERR message 
   //and return false
   inf.open(inputFile->get_cstr());
   if(!inf.is_open()){
      printErrMsg(OPENERR, -1, NULL);
      return false;
   }


   return true;  //file name is good and file open succeeded
}

/*
 * load 
 * Opens the .yo file
 * Reads the lines in the file line by line and
 * loads the data bytes in data records into the Memory
 * If a line has an error in it, then NONE of the line will be
 * loaded into memory and the load function will return false
 *
 * Returns true if load succeeded (no errors in the input) 
 * and false otherwise
*/   
bool Loader::load()
{
   //if the openFile fails return false
   if (!openFile()) return false;

   std::string line;
   int lineNumber = 1;  //needed if an error is found
   while (getline(inf, line))
   {
      //create a String to contain the std::string
      //Now, all accesses to the input line MUST be via your
      //String class methods
      String inputLine(line);

      //if the line is a data record with errors
      //then print the BADDATA error message and return false
      //check to see if it is a data record first
      //if (line.isSubstr("0x", 0, error)){if bad data do things} else if (bad comm)
      if (inputLine.isSubString((char*)"0x", 0, error)){
         if(badData(line, lineNumber)){
            printErrMsg(BADDATA, lineNumber, &inputLine);
            return false;
         }
      }
      //if the line is a comment record with errors
      //then print the BADCOM error message and return false
      //
      else if(badComment(line, lineNumber)){
         return false;
      }
      //Otherwise, load any data on the line into
      //memory
      //
      loadToMemory(line);
      //Don't do all of this work in this method!
      //Break the work up into multiple single purpose methods

      //increment the line number for next iteration
      lineNumber++;
   }

   return true;  //load succeeded
}

//add helper methods here and to Loader.h


/* Helper method to check for bad data.
 *
 * 
 * @param: String l the line being checked
 * @param: int ln the line number
 */
bool Loader::badData(String l, int ln){
   // if (! l.isChar(' ', 0, error)){
   //    if (!(l.isChar(':', ADDREND + 1, error)) || !(l.isChar(' ', ADDREND + 2, error)) || !(l.isChar(' ', COMMENT - 1, error)) 
   //       || !(isHex(l, DATABEGIN)) || !(l.isChar('|', COMMENT, error)) || !dataMultOf2(l) || dataStart(l) ||  nonContiguous(l)
   //       || ! lastAddressLess(l)){
   //       if (!(l.isSpaces(0, COMMENT - 1, error))){
   //          printErrMsg(BADDATA, ln, &l);
   //          return true;
   //       }
   //    }
   // }

   //fix this up to make it look not so bad

   if (! l.isChar(' ', 0, error)){
      if (!(l.isChar(':', ADDREND + 1, error))){
         if (!(l.isSpaces(0, COMMENT - 1, error))){
            return true;
         }
      }
      if (!(l.isChar(' ', ADDREND + 2, error))){
         if (!(l.isSpaces(0, COMMENT - 1, error))){
            return true;
         }
      }
      if (!(l.isChar(' ', COMMENT - 1, error))){
         if (!(l.isSpaces(0, COMMENT - 1, error))){
            return true;
         }
      }
      if (!(isHex(l, DATABEGIN))){
         if (!(l.isSpaces(0, COMMENT - 1, error))){
            return true;
         }
      }
      if (!(l.isChar('|', COMMENT, error))){
         if (!(l.isSpaces(0, COMMENT - 1, error))){
            return true;
         }
      }
      if (!dataMultOf2(l)){
         if (!(l.isSpaces(0, COMMENT - 1, error))){
            return true;
         }
      }
      if (dataStart(l)){
         if (!(l.isSpaces(0, COMMENT - 1, error))){
            return true;
         }
      }
      if (nonContiguous(l)){
         if (!(l.isSpaces(0, COMMENT - 1, error))){
            return true;
         }
      }
      if (! lastAddressLess(l)){
         if (!(l.isSpaces(0, COMMENT - 1, error))){
            return true;
         }
      }
   }      
   return false;
}

/* Returns true if str[start idx, end idx] is all valid hex.
 *
 * 
 * @param: String l the line being checked
 * @param: int32_t sIdx the starting index
 */
bool Loader::isHex(String l, int32_t sIdx){
   std::string help; 
   help = l.get_stdstr();
   for (int i = sIdx; help[i] != ' '; i++){
      if(!isxdigit(help[i])){
         return false;
      }
   }
   return true;
}

/* checks and makes sure that the data is a multiple of two making sure that it is aligned.
 *
 * 
 * @param: String l the line being checked
 */
bool Loader::dataMultOf2(String l){
   int count = 0;
   for (int i = DATABEGIN; !l.isChar(' ', i, error); i++){
      count++;
   }
   if ((count % 2) != 0){
      return false;
   }
   if (l.convert2Hex(ADDRBEGIN, ADDREND, error) + count > MEMSIZE){
      return false;
   }
   return true;
}

/* Helper method to load memory to the mem item.
 *
 * 
 * @param: String l the line being checked
 */
void Loader::loadToMemory(String line){
   int idx = DATABEGIN;  // defining a local variable to a changeable variable
   bool error = false;   // variable to hold a bool that is false to pass into all of the methods that need an error
   std::string stArray = line.get_stdstr();
   if(stArray[0] != ' '){
      this->lastAddress = line.convert2Hex(ADDRBEGIN, ADDREND, error);
   }
   for(int i = 0; i < MAXBYTES && stArray[idx] != ' '; i++){ //iterate though the erroy 
      this->mem->putByte(line.convert2Hex(idx, idx + 1, error), lastAddress, error);
      this->lastAddress += 1;
      idx += 2;
   }
}

/* Helper method to check for bad comments.
 *
 * 
 * @param: String l the line being checked
 * @param: int ln the line number
 */
bool Loader::badComment(String l, int ln){
   if(l.isSpaces(ADDRBEGIN, COMMENT, error) || !l.isSubString((char *)"0x", 0, error) || noAddress(l)){
      if (!(l.isSpaces(0, COMMENT - 1, error))){
         printErrMsg(BADCOM, ln, &l);
         return true;
      }
   }
   if (l.isSpaces(0, COMMENT, error)){
      printErrMsg(BADCOM, ln, &l);
      return true;
   }
   return false;
}

/* Helper method to check where the data starts.
 *
 * 
 * @param: String l the line being checked
 */
bool Loader::dataStart(String l){
   if(!(l.isSpaces(DATABEGIN, COMMENT - 1, error)) && (l.isChar(' ', DATABEGIN, error))){
      return true;
   }
   return false;
}

/* Helper method to check for an address.
 *
 * 
 * @param: String l the line being checked
 */
bool Loader::noAddress(String l){
   if(l.isSpaces(ADDRBEGIN, ADDREND - 1, error)){
      return true;
   }
   return false;
}

/* Helper method to check if it is contiguous.
 *
 * 
 * @param: String l the line being checked
 */
bool Loader::nonContiguous(String l){
   std::string help; 
   help = l.get_stdstr();
   int count = DATABEGIN;
   for (int i = DATABEGIN; help[i] != ' '; i++){
      count++;
   } 
   if(!(l.isSpaces(count, COMMENT - 1, error))){
      return true;
   }
   return false;
}

/* Helper method to make sure that the next address is higher than the last used address.
 *
 * 
 * @param: String l the line being checked
 */
bool Loader::lastAddressLess(String l){
   error = false;
   int address = l.convert2Hex(ADDRBEGIN, ADDREND, error);
   if (this->lastAddress > address || error){
      return false;
   }
   return true; 
}