#ifndef BYTE_OPERATIONS_H
#define BYTE_OPERATIONS_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdint>
#include <vector>
#include <array>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <codecvt>
#include <locale>
#include <algorithm>

class ByteOperations
{
public:
    static std::string ReadAsciiString(const std::vector<uint8_t> &ByteArray, uint32_t Offset, uint32_t Length);
    static std::wstring ReadUnicodeString(const std::vector<uint8_t> &ByteArray, uint32_t Offset, uint32_t Length);
    static void WriteAsciiString(std::vector<uint8_t> &ByteArray, uint32_t Offset, const std::string &Text, uint32_t MaxBufferLength = 0);
    static void WriteUnicodeString(std::vector<uint8_t> &ByteArray, uint32_t Offset, const std::wstring &Text, uint32_t MaxBufferLength = 0);
    static uint32_t ReadUInt32(const std::vector<uint8_t> &ByteArray, uint32_t Offset);
    static void WriteUInt32(std::vector<uint8_t> &ByteArray, uint32_t Offset, uint32_t Value);
    static int32_t ReadInt32(const std::vector<uint8_t> &ByteArray, uint32_t Offset);
    static void WriteInt32(std::vector<uint8_t> &ByteArray, uint32_t Offset, int32_t Value);
    static uint16_t ReadUInt16(const std::vector<uint8_t> &ByteArray, uint32_t Offset);
    static void WriteUInt16(std::vector<uint8_t> &ByteArray, uint32_t Offset, uint16_t Value);
    static int16_t ReadInt16(const std::vector<uint8_t> &ByteArray, uint32_t Offset);
    static void WriteInt16(std::vector<uint8_t> &ByteArray, uint32_t Offset, int16_t Value);
    static uint8_t ReadUInt8(const std::vector<uint8_t> &ByteArray, uint32_t Offset);
    static void WriteUInt8(std::vector<uint8_t> &ByteArray, uint32_t Offset, uint8_t Value);
    static uint32_t ReadUInt24(const std::vector<uint8_t> &ByteArray, uint32_t Offset);
    static void WriteUInt24(std::vector<uint8_t> &ByteArray, uint32_t Offset, uint32_t Value);
    static uint64_t ReadUInt64(const std::vector<uint8_t> &ByteArray, uint32_t Offset);
    static void WriteUInt64(std::vector<uint8_t> &ByteArray, uint32_t Offset, uint64_t Value);
    static std::array<uint8_t, 16> ReadGuid(const std::vector<uint8_t> &ByteArray, uint32_t Offset);
    static void WriteGuid(std::vector<uint8_t> &ByteArray, uint32_t Offset, const std::array<uint8_t, 16> &Value);
    static uint32_t Align(uint32_t Base, uint32_t Offset, uint32_t Alignment);
    static uint32_t FindPatternInFile(const std::string &FileName, const std::vector<uint8_t> &Pattern, const std::vector<uint8_t> &Mask, std::vector<uint8_t> &OutPattern);
    static uint32_t FindAscii(const std::vector<uint8_t> &SourceBuffer, const std::string &Pattern);
    static uint32_t FindUnicode(const std::vector<uint8_t> &SourceBuffer, const std::wstring &Pattern);
    static uint32_t FindUint(const std::vector<uint8_t> &SourceBuffer, uint32_t Pattern);
    static uint32_t FindPattern(const std::vector<uint8_t> &SourceBuffer, uint32_t SourceOffset, uint32_t SourceSize, const std::vector<uint8_t> &Pattern, const std::vector<uint8_t> &Mask, std::vector<uint8_t> &OutPattern);
    static bool Compare(const std::vector<uint8_t> &Array1, const std::vector<uint8_t> &Array2);
    static uint8_t CalculateChecksum8(const std::vector<uint8_t> &Buffer, uint32_t Offset, uint32_t Size);
    static uint16_t CalculateChecksum16(const std::vector<uint8_t> &Buffer, uint32_t Offset, uint32_t Size);
    static uint32_t CRC32(const std::vector<uint8_t> &Input, uint32_t Offset, uint32_t Length);

private:
    static const std::array<uint32_t, 256> CRC32Table;
};

#endif // BYTE_OPERATIONS_H
