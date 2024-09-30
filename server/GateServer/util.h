#pragma once
#include <string>
#include <cassert>


//char 转为16进制
unsigned char ToHex(unsigned char x);
//从16进制转为十进制的char
unsigned char FromHex(unsigned char x);
std::string UrlEncode(const std::string& str);

std::string UrlDecode(const std::string& str);


