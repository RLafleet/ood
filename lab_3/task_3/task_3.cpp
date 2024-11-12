#include <iostream>
#include <memory>
#include <cstring>
#include <stdexcept>
#include "IInputDataStream.h"
#include "IOutputDataStream.h"
#include "FileInputStream.h"
#include "FileOutputStream.h"
#include "DecompressInputStream.h"
#include "DecryptInputStream.h"
#include "CompressOutputStream.h"
#include "EncryptOutputStream.h"

void displayHelp() {
    std::cout << "Usage: program [options]\n";
    std::cout << "--encrypt <key>       Encrypt the input file using the provided key.\n";
    std::cout << "--decrypt <key>       Decrypt the input file using the provided key.\n";
    std::cout << "--compress            Compress the output file.\n";
    std::cout << "--decompress          Decompress the output file.\n";
}
//Диаграмма классов +

//Опции, а не опция +
//compress неправильно +
//вынести отдельно декоратор

int main(int argc, char* argv[]) 
{
    try 
    {
        if (argc < 3)
        {
            displayHelp();
            return 1;
        }

        std::string inputFileName = argv[1];
        std::string outputFileName = argv[2];
        std::unique_ptr<IInputDataStream> inputStream = std::make_unique<FileInputStream>(inputFileName);
        std::unique_ptr<IOutputDataStream> outputStream = std::make_unique<FileOutputStream>(outputFileName);

        for (int i = 3; i < argc; ++i)
        {
            if (strcmp(argv[i], "--encrypt") == 0)
            {
                uint32_t key = std::stoi(argv[++i]);
                outputStream = std::make_unique<EncryptOutputStream>(std::move(outputStream), key);
            }
            else if (strcmp(argv[i], "--decrypt") == 0)
            {
                uint32_t key = std::stoi(argv[++i]);
                inputStream = std::make_unique<DecryptInputStream>(std::move(inputStream), key);
            }
            else if (strcmp(argv[i], "--compress") == 0)
            {
                outputStream = std::make_unique<CompressOutputStream>(std::move(outputStream));
            }
            else if (strcmp(argv[i], "--decompress") == 0)
            {
                inputStream = std::make_unique<DecompressInputStream>(std::move(inputStream));
            }
            else
            {
                throw std::invalid_argument("Unknown option: " + std::string(argv[i]));
            }
        }

        std::vector<uint8_t> buffer(1024);
        while (!inputStream->IsEOF()) 
        {
            std::streamsize bytesRead = inputStream->ReadBlock(buffer.data(), buffer.size());
            outputStream->WriteBlock(buffer.data(), bytesRead);
        }

        outputStream->Close();
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1; 
    }

    return 0; 
}
