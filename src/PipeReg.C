#include <iostream>
#include <iomanip>
#include <string>
#include <cstdint>
#include <assert.h>
#include "PipeRegField.h"
#include "PipeReg.h"

/*
 * get
 * 
 * returns the value of a PipeRegField
 *
 * @param - which is an index into the PipeRegFieldArray
 */
uint64_t PipeReg::get(int32_t which)
{
   assert(which >= 0 && which < numFields);
   return fields[which]->getOutput();
}

/*
 * set
 * 
 * sets the value of a PipeRegField
 *
 * @param - which is an index into the PipeRegField array
 * @param - value is the value the field is to be set to
 */
void PipeReg::set(int32_t which, uint64_t value)
{
   assert(which >= 0 && which < numFields);
   fields[which]->setInput(value);
}

/*
 * normal
 * 
 * calls the normal method on each element of the PipeRegField array
 */
void PipeReg::normal()
{
   int32_t i;
   for (i = 0; i < numFields; i++)
      fields[i]->normal();
}

/* dumpField
 * Outputs a string and a uint64_t using the indicated width and padding with 0s.
 * If newline is true, a newline is output afterward.
 *
 * @param: fieldname - string to output; width used is the size of the string
 * @param: width - width in which to output the uint64_t
 * @param: fieldvalue - uint64_t that is output in width columns and padded with 0s
 * @param: newline - if true a newline is output after the fieldname and field value
 */
void PipeReg::dumpField(std::string fieldname, int width, uint64_t fieldvalue, bool newline)
{
   std::cout << fieldname << std::hex << std::setw(width) 
             << std::setfill('0') << fieldvalue;
   if (newline) std::cout << std::endl;
}   
