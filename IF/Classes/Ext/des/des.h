#ifndef des_h
#define des_h

#include <string>
#include <vector>
#include <stdlib.h>
class DES
{
public:
   static std::string encrypt(const std::string& word);
   static std::string decrypt(const std::string& word);
};
#endif