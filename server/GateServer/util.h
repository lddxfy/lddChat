#pragma once
#include <string>
#include <cassert>


//char תΪ16����
unsigned char ToHex(unsigned char x);
//��16����תΪʮ���Ƶ�char
unsigned char FromHex(unsigned char x);
std::string UrlEncode(const std::string& str);

std::string UrlDecode(const std::string& str);


