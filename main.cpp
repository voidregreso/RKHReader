#include <iostream>
#include <filesystem>
#include <fstream>
#include "ByteOperations.h"
#include "Converter.h"
#include "QualcommPartition.h"

namespace fs = std::filesystem;

void PrintRKHFromFile(const std::string &file)
{
    try
    {
        QualcommPartition qualcommPartition(file);
        if (!qualcommPartition.RootKeyHash.empty())
        {
            std::cout << file << std::endl;
            std::cout << "RKH: " << Converter::ConvertHexToString(qualcommPartition.RootKeyHash, "") << std::endl;
        }
        else
        {
            std::cout << file << std::endl;
            std::cout << "FAIL!" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cout << file << std::endl;
        std::cout << "EXCEPTION!" << std::endl;
        std::cout << e.what() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2 || !(fs::exists(argv[1]) && (fs::is_regular_file(argv[1]) || fs::is_directory(argv[1]))))
    {
        std::cout << "Usage: <Path to Qualcomm Signed file>" << std::endl;
        return 1;
    }

    if (fs::is_regular_file(argv[1]))
    {
        PrintRKHFromFile(argv[1]);
    }

    if (fs::is_directory(argv[1]))
    {
        for (const auto &entry : fs::recursive_directory_iterator(argv[1]))
        {
            if (entry.is_regular_file())
            {
                PrintRKHFromFile(entry.path().string());
            }
        }
    }

    return 0;
}