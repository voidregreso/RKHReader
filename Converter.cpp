#include "Converter.h"

std::string Converter::ConvertHexToString(const std::vector<uint8_t> &Bytes, const std::string &Separator)
{
    std::ostringstream s;
    for (size_t i = 0; i < Bytes.size(); ++i)
    {
        if (i != 0)
        {
            s << Separator;
        }
        s << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(Bytes[i]);
    }
    return s.str();
}

std::vector<uint8_t> Converter::ConvertStringToHex(const std::string &HexString)
{
    if (HexString.length() % 2 != 0)
    {
        throw std::invalid_argument("The binary key cannot have an odd number of digits");
    }

    std::vector<uint8_t> arr(HexString.length() / 2);
    for (size_t i = 0; i < arr.size(); ++i)
    {
        arr[i] = (GetHexVal(HexString[i * 2]) << 4) + GetHexVal(HexString[i * 2 + 1]);
    }
    return arr;
}

int Converter::GetHexVal(char hex)
{
    int val = static_cast<int>(hex);
    return val - (val < 58 ? 48 : (val < 97 ? 55 : 87));
}
