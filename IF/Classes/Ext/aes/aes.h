#ifndef aes_h
#define aes_h

#include <string>
#include <vector>
#include <stdlib.h>
class AES
{
public:
   static std::string encrypt(const std::string& word,double time);
   static std::string decrypt(const std::string& word,int lenth,double time);
};
#endif