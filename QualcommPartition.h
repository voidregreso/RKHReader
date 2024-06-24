#ifndef QUALCOMM_PARTITION_H
#define QUALCOMM_PARTITION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <openssl/sha.h> // For SHA256
#include "ByteOperations.h"
#include "Converter.h"

enum class QualcommPartitionHeaderType
{
    Long,
    Short
};

class QualcommPartition
{
public:
    std::vector<uint8_t> Binary;
    uint32_t HeaderOffset;
    QualcommPartitionHeaderType HeaderType;
    uint32_t ImageOffset;
    uint32_t ImageAddress;
    uint32_t ImageSize;
    uint32_t CodeSize;
    uint32_t SignatureAddress;
    uint32_t SignatureSize;
    uint32_t SignatureOffset;
    uint32_t CertificatesAddress;
    uint32_t CertificatesSize;
    uint32_t CertificatesOffset;
    std::vector<uint8_t> RootKeyHash;

    QualcommPartition(const std::string &Path);
    QualcommPartition(const std::vector<uint8_t> &binary, uint32_t offset = 0);

private:
    static std::vector<uint8_t> ReadFileToBytes(const std::string &path);
    static std::vector<uint8_t> ComputeSHA256(const std::vector<uint8_t> &data, size_t offset, size_t length);
};

#endif // QUALCOMM_PARTITION_H
