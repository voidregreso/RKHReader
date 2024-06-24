#ifndef CONVERTER_H
#define CONVERTER_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <string>

class Converter
{
public:
    static std::string ConvertHexToString(const std::vector<uint8_t> &Bytes, const std::string &Separator);
    static std::vector<uint8_t> ConvertStringToHex(const std::string &HexString);

private:
    static int GetHexVal(char hex);
};

#endif // CONVERTER_H
