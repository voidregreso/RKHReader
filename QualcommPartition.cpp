#include "QualcommPartition.h"

QualcommPartition::QualcommPartition(const std::string &Path) : QualcommPartition(ReadFileToBytes(Path)) {}

QualcommPartition::QualcommPartition(const std::vector<uint8_t> &binary, uint32_t offset)
{
#if defined(DEBUG)
    std::vector<uint8_t> hash(SHA256_DIGEST_LENGTH);
    SHA256(binary.data(), binary.size(), hash.data());
    std::cerr << "Loader: " << Converter::ConvertHexToString(hash, "") << std::endl;
#endif

    this->Binary = binary;

    std::vector<uint8_t> LongHeaderPattern = {0xD1, 0xDC, 0x4B, 0x84, 0x34, 0x10, 0xD7, 0x73, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    std::vector<uint8_t> LongHeaderMask = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    std::vector<uint8_t> dummy;

    if (ByteOperations::FindPattern(binary, offset, 4, {0x7F, 0x45, 0x4C, 0x46}, {0x00, 0x00, 0x00, 0x00}, dummy) == 0)
    {
        // This is an ELF image
        HeaderType = QualcommPartitionHeaderType::Short;
        uint32_t ProgramHeaderOffset;
        uint16_t ProgramHeaderEntrySize;
        uint32_t HashTableProgramHeaderOffset;
        if (binary[offset + 0x04] == 1)
        {
            // 32-bit elf image
            ProgramHeaderOffset = offset + ByteOperations::ReadUInt32(binary, offset + 0x1c);
            ProgramHeaderEntrySize = ByteOperations::ReadUInt16(binary, offset + 0x2a);
            HashTableProgramHeaderOffset = ProgramHeaderOffset + ProgramHeaderEntrySize;
            ImageOffset = offset + ByteOperations::ReadUInt32(binary, HashTableProgramHeaderOffset + 0x04);
            HeaderOffset = ImageOffset + 8;
        }
        else if (binary[offset + 0x04] == 2)
        {
            // 64-bit elf image
            ProgramHeaderOffset = offset + ByteOperations::ReadUInt32(binary, offset + 0x20);
            ProgramHeaderEntrySize = ByteOperations::ReadUInt16(binary, offset + 0x36);
            HashTableProgramHeaderOffset = ProgramHeaderOffset + ProgramHeaderEntrySize;
            ImageOffset = offset + ByteOperations::ReadUInt64(binary, HashTableProgramHeaderOffset + 0x08);
            HeaderOffset = ImageOffset + 8;
        }
        else
        {
            throw std::runtime_error("Invalid programmer - The type of elf image could not be determined from the provided programmer.");
        }
    }
    else if (!ByteOperations::FindPattern(binary, offset, LongHeaderPattern.size(), LongHeaderPattern, LongHeaderMask, dummy) == 0)
    {
        HeaderType = QualcommPartitionHeaderType::Short;
        ImageOffset = offset;
        HeaderOffset = ImageOffset + 8;
    }
    else
    {
        HeaderType = QualcommPartitionHeaderType::Long;
        ImageOffset = offset;
        HeaderOffset = ImageOffset + LongHeaderPattern.size();
    }

    uint32_t Version = ByteOperations::ReadUInt32(binary, ImageOffset + 0x04);

    if (ByteOperations::ReadUInt32(binary, HeaderOffset + 0x00) != 0)
    {
        ImageOffset = ByteOperations::ReadUInt32(binary, HeaderOffset + 0x00);
    }
    else if (HeaderType == QualcommPartitionHeaderType::Short)
    {
        ImageOffset += 0x28;
    }
    else
    {
        ImageOffset += 0x50;
    }

    ImageAddress = ByteOperations::ReadUInt32(binary, HeaderOffset + 0x04);
    ImageSize = ByteOperations::ReadUInt32(binary, HeaderOffset + 0x08);
    CodeSize = ByteOperations::ReadUInt32(binary, HeaderOffset + 0x0C);
    SignatureAddress = ByteOperations::ReadUInt32(binary, HeaderOffset + 0x10);
    SignatureSize = ByteOperations::ReadUInt32(binary, HeaderOffset + 0x14);
    CertificatesAddress = ByteOperations::ReadUInt32(binary, HeaderOffset + 0x18);
    CertificatesSize = ByteOperations::ReadUInt32(binary, HeaderOffset + 0x1C);

    if (SignatureAddress == 0xFFFFFFFF)
    {
        SignatureAddress = ImageAddress + CodeSize;
    }

    if (CertificatesAddress == 0xFFFFFFFF)
    {
        CertificatesAddress = SignatureAddress + SignatureSize;
    }

    if (Version > 5)
    {
        ImageOffset += 0x80;
    }

    SignatureOffset = ImageOffset + CodeSize;
    CertificatesOffset = ImageOffset + CodeSize + SignatureSize;

    uint32_t CurrentCertificateOffset = CertificatesOffset;
    uint32_t CertificateSize = 0;
    while (CurrentCertificateOffset < (CertificatesOffset + CertificatesSize))
    {
        if ((binary[CurrentCertificateOffset] == 0x30) && (binary[CurrentCertificateOffset + 1] == 0x82))
        {
            CertificateSize = (binary[CurrentCertificateOffset + 2] * 0x100) + binary[CurrentCertificateOffset + 3] + 4;

            if ((CurrentCertificateOffset + CertificateSize) == (CertificatesOffset + CertificatesSize))
            {
                RootKeyHash = ComputeSHA256(binary, CurrentCertificateOffset, CertificateSize);
#if defined(DEBUG)
                std::cerr << "RKH: " << Converter::ConvertHexToString(RootKeyHash, "") << std::endl;
#endif
            }
#if defined(DEBUG)
            else
            {
                std::cerr << "Cert: " << Converter::ConvertHexToString(ComputeSHA256(binary, CurrentCertificateOffset, CertificateSize), "") << std::endl;
            }
#endif
            CurrentCertificateOffset += CertificateSize;
        }
        else
        {
            if (RootKeyHash.empty() && (CurrentCertificateOffset > CertificatesOffset))
            {
                CurrentCertificateOffset -= CertificateSize;
                RootKeyHash = ComputeSHA256(binary, CurrentCertificateOffset, CertificateSize);
#if defined(DEBUG)
                std::cerr << "RKH: " << Converter::ConvertHexToString(RootKeyHash, "") << std::endl;
#endif
            }
            break;
        }
    }
}

std::vector<uint8_t> QualcommPartition::ReadFileToBytes(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Cannot open file: " + path);
    }
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::vector<uint8_t> QualcommPartition::ComputeSHA256(const std::vector<uint8_t> &data, size_t offset, size_t length)
{
    std::vector<uint8_t> hash(SHA256_DIGEST_LENGTH);
    SHA256(data.data() + offset, length, hash.data());
    return hash;
}
