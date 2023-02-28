#include <cstdint>
#include <string> //for std::string functions
#include <string.h> //for c-string functions (strlen, strcpy, ..)
#include "Debug.h"
#include "String.h"

/*
 * String
 *
 * Construct a String object out of a std::string 
 * This constructor would also be called on a c-str
 *
 * Modifies: str and length data members
 */
String::String(std::string str)
{
   //TODO
   //Dynamically allocate an array of chars just large enough to 
   //hold the chars of the std::string.
   //Don't store the NULL or allocate space for it.
   //Copy the characters in the std::string (excluding a NULL)
   //into your str array.
   //Set length to the size of the array.
   length = str.length();
   this->str = new char[length];
   for(int i = 0; i < length; i++){
      this->str[i] = str[i];
   } 
   return;
   
}

/*
 * get_cstr
 *
 * Build a return a c-str from the array of characters.
 */
char * String::get_cstr()
{
   //TODO
   //You need to dynamically allocate space (array of char) for the
   //c-string that you are building
   //That space needs to include space for a NULL
   //Don't forget to add the NULL.
   char * cstr = new char[length + 1];
   for(int i = 0; i < length; i++){
      cstr[i] = str[i];
   } 
   cstr[length] = '\0';
   return (char *)cstr;  
}

/*
 * get_stdstr
 *
 * Build a return a std::string from the array of characters.
 */
std::string String::get_stdstr()
{
   //TODO
   //You need to declare std::string and append the characters
   //from your str array to it
   std::string result; //here's the declaration
   for(int i = 0; i < length; i++){
      result += str[i];
   }
   return result; 
}

/*
 * get_length
 *
 * return the value of the length data member
 */
int32_t String::get_length()
{
   //TODO
   
   return length;  
}

/*
 * badIndex
 *
 * Returns true if this index into the str array is
 * invalid (negative or greater than array size)
 */
bool String::badIndex(int32_t idx)
{
   //TODO
   if(idx < 0 || idx >= length){
      return true;
   }
   return false;
}

/*
 * isSpaces
 *
 * Returns true if the characters in the array from
 * startIdx to endIdx are all spaces.
 *
 * Three cases are possible:
 * 1) error set to true and returns false if indices are invalid
 * 2) error set to false and returns false if the characters in
 *    the array at the specified indices are not all spaces
 * 3) error set to false and returns true if the characters in
 *    the array at the specified indices are all spaces
 */

bool String::isSpaces(int32_t startIdx, int32_t endIdx, bool & error)
{
   //TODO
   if(badIndex(startIdx) || badIndex(endIdx) || startIdx > endIdx){
      error = true;
      return false;
   }
   for(int i = startIdx; i <= endIdx; i++){
      if(str[i] != ' '){
         error = false;
         return false;
      }
   }
   error = false;
   return true;
}

/*
 * convert2Hex
 *
 * Builds and returns a number from the values in the array from startIdx
 * to endIdx treating those characters as if they are hex digits.
 * Returns 0 and sets error to true if the indices are invalid or
 * if the characters are not hex.
 *
 * Valid characters for conversion are:
 *  '0', '1' through '9'
 *  'a', 'b' through 'f'
 *  'A', 'B' through 'F'
 *
 *  Three cases are possible:
 *  1) sets error to false and returns converted number if the characters
 *     in the specified indices are valid hex characters
 *  2) sets error to true and returns 0 if the indices are invalid
 *  3) sets error to true and returns 0 if the indices are valid but the
 *     characters are not hex
 */
uint32_t String::convert2Hex(int32_t startIdx, int32_t endIdx, bool & error)
{
   //TODO         
   uint32_t ans = 0;
   if(badIndex(startIdx) || badIndex(endIdx) || startIdx > endIdx){
      error = true;
      return 0;
   }
   for(int i = startIdx; i <= endIdx; i++){
      ans <<= 4;
      if(str[i] >= '0' && str[i] <='9'){
         ans |= (str[i] - '0');
      }
      else if(str[i] >= 'a' && str[i] <='f'){
         ans |= ((str[i] - 'a') + 10);
      }
      else if(str[i] >= 'A' && str[i] <='F'){
         ans |= ((str[i] - 'A') + 10);
      }
      else {
         error = true;
         return 0;
      }
      
   }
   error = false;
   return ans;
}

/* 
 * isChar
 * Returns true if str[idx] is equal to what 
 *
 * Three cases are possible:
 * 1) set error to true and return false if idx is invalid
 * 2) set error to false and return true if str[idx] is what
 * 3) set error to false and return false if str[idx] is not what
 */
bool String::isChar(char what, int32_t idx, bool & error)
{
   //TODO
   if(badIndex(idx)){
      error = true;
      return false;
   }
   if(str[idx] == what){
      error = false;
      return true;
   }
   error = false;  
   return false;
}      

/*
 * isSubString
 *
 * Returns true if the c-str subStr is in the str array starting
 * at index idx 
 *
 * Three cases are possible:
 * 1) one or both indices are invalid: set error to true and return false
 * 2) indices are valid and subStr is in str array: set error to false 
 *    and return true
 * 3) indices are valid and subStr is not in str array: set error to 
 *    false and return false
 */
bool String::isSubString(char * subStr, int32_t idx, bool & error)
{
   //TODO
   //Length represents the strlen and the idx added together
   int lenth = strlen(subStr) + idx;
   if(badIndex(idx) || badIndex(idx + (int) strlen(subStr) - 1)){
      error = true;
      return false;
   }
   for (int i = idx; i <= (int) lenth - 1; i++){
      if(subStr[i - idx] != str[i]){
         error = false;
         return false;
      }
   }
   error = false;
   return true;
}


/*
 * isSubString
 *
 * Returns true if the std::string subStr is in the str array starting
 * at index idx 
 *
 * Three cases are possible:
 * 1) one or both indices are invalid: set error to true and return false
 * 2) indices are valid and subStr is in str array: set error to false 
 *    and return true
 * 3) indices are valid and subStr is not in str array: set error to 
 *    false and return false
 */
bool String::isSubString(std::string subStr, int32_t idx, bool & error)
{  
   //TODO
   if(badIndex(idx) || badIndex(idx + (int) subStr.length() - 1)){
      error = true;
      return false;
   }
   for (int i = idx; i <= (int) subStr.length() + idx - 1; i++){
      if(subStr[i - idx] != str[i]){
         error = false;
         return false;
      }
   }
   error = false;
   return true;
}

 
