#define CATCH_CONFIG_MAIN
#include "../../Catch2/catch.hpp"
#include "../task_3/CompressOutputStream.h"
#include "../task_3/DecompressInputStream.h"
#include "../task_3/EncryptOutputStream.h"
#include "../task_3/DecryptInputStream.h"
#include "MemoryStream.h"
#include <memory>

// дополнить тесты,  проверить на больше 256
TEST_CASE("CompressOutputStream compresses repeated sequences", "[compression]") {
    auto memoryStream = std::make_unique<MemoryOutputStream>();
    auto rawMemoryStream = memoryStream.get(); 
    CompressOutputStream compressStream(std::move(memoryStream));

    SECTION("Compress 4 repeated bytes") {
        uint8_t data[] = { 1, 1, 1, 1 };
        compressStream.WriteBlock(data, sizeof(data));
        compressStream.Close();

        const std::vector<uint8_t>& compressedData = rawMemoryStream->GetData();
        REQUIRE(compressedData.size() == 3);
        REQUIRE(compressedData[0] == 0xFF);
        REQUIRE(compressedData[1] == 0x01);
        REQUIRE(compressedData[2] == 0x04);
    }
}

TEST_CASE("DecompressInputStream decompresses data correctly", "[decompression]") {
    SECTION("Decompress simple compressed data") {
        std::vector<uint8_t> compressedData = { 0xFF, 0x01, 0x04 };
        auto memoryStream = std::make_unique<MemoryInputStream>(compressedData);
        DecompressInputStream decompressStream(std::move(memoryStream));

        uint8_t decompressedData[4];
        decompressStream.ReadBlock(decompressedData, sizeof(decompressedData));

        uint8_t expectedData[] = { 1, 1, 1, 1 };
        REQUIRE(memcmp(decompressedData, expectedData, sizeof(decompressedData)) == 0);
    }

    SECTION("Decompress mixed data") {
        std::vector<uint8_t> compressedData = { 0xFF, 0x02, 0x02, 0xFF, 0x03, 0x03, 0xFF, 0x04, 0x04 };
        auto memoryStream = std::make_unique<MemoryInputStream>(compressedData);
        DecompressInputStream decompressStream(std::move(memoryStream));

        uint8_t decompressedData[9];
        decompressStream.ReadBlock(decompressedData, sizeof(decompressedData));

        uint8_t expectedData[] = { 2, 2, 3, 3, 3, 4, 4, 4, 4 };
        REQUIRE(memcmp(decompressedData, expectedData, sizeof(decompressedData)) == 0);
    }
}

TEST_CASE("CompressOutputStream handles non-repeated data correctly", "[compression]") {
    auto memoryStream = std::make_unique<MemoryOutputStream>();
    auto rawMemoryStream = memoryStream.get(); 
    CompressOutputStream compressStream(std::move(memoryStream));

    uint8_t data[] = { 5, 6, 7, 8 };
    compressStream.WriteBlock(data, sizeof(data));
    compressStream.Close();

    const std::vector<uint8_t>& compressedData = rawMemoryStream->GetData();
    REQUIRE(compressedData.size() == sizeof(data));
    REQUIRE(memcmp(compressedData.data(), data, sizeof(data)) == 0);
}

TEST_CASE("EncryptOutputStream encrypts data correctly", "[encryption]") {
    auto memoryStream = std::make_unique<MemoryOutputStream>();
    auto rawMemoryStream = memoryStream.get();
    uint32_t encryptionKey = 12345;
    EncryptOutputStream encryptStream(std::move(memoryStream), encryptionKey);

    SECTION("Encrypt block of data") {
        uint8_t data[] = { 1, 2, 3, 4, 5 };
        encryptStream.WriteBlock(data, sizeof(data));
        encryptStream.Close();

        const std::vector<uint8_t>& encryptedData = rawMemoryStream->GetData();

        REQUIRE(encryptedData.size() == sizeof(data));
        REQUIRE(memcmp(encryptedData.data(), data, sizeof(data)) != 0);
    }
}

TEST_CASE("DecryptInputStream decrypts data correctly", "[decryption]") {
    uint32_t encryptionKey = 12345;

    SECTION("Decrypt previously encrypted data") {
        auto memoryStream = std::make_unique<MemoryOutputStream>();
        auto rawMemoryStream = memoryStream.get();
        EncryptOutputStream encryptStream(std::move(memoryStream), encryptionKey);

        uint8_t originalData[] = { 1, 2, 3, 4, 5 };
        encryptStream.WriteBlock(originalData, sizeof(originalData));
        encryptStream.Close();

        auto encryptedMemoryStream = std::make_unique<MemoryInputStream>(rawMemoryStream->GetData());
        DecryptInputStream decryptStream(std::move(encryptedMemoryStream), encryptionKey);

        uint8_t decryptedData[sizeof(originalData)];
        decryptStream.ReadBlock(decryptedData, sizeof(decryptedData));

        REQUIRE(memcmp(decryptedData, originalData, sizeof(originalData)) == 0);
    }
}

TEST_CASE("Encrypt and Decrypt streams work together", "[encryption][decryption]") {
    uint32_t encryptionKey = 54321;

    SECTION("Encrypt and decrypt block of data") {
        auto memoryStream = std::make_unique<MemoryOutputStream>();
        auto rawMemoryStream = memoryStream.get();
        EncryptOutputStream encryptStream(std::move(memoryStream), encryptionKey);

        uint8_t originalData[] = { 10, 20, 30, 40, 50 };
        encryptStream.WriteBlock(originalData, sizeof(originalData));
        encryptStream.Close();

        auto encryptedMemoryStream = std::make_unique<MemoryInputStream>(rawMemoryStream->GetData());
        DecryptInputStream decryptStream(std::move(encryptedMemoryStream), encryptionKey);

        uint8_t decryptedData[sizeof(originalData)];
        decryptStream.ReadBlock(decryptedData, sizeof(decryptedData));

        REQUIRE(memcmp(decryptedData, originalData, sizeof(originalData)) == 0);
    }
}
