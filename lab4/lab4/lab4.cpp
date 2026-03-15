#include <iostream>
#include <fstream>
#include "Huffman.h"

void createTestFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename, std::ios::binary);
    file.write(content.c_str(), content.size());
    file.close();
}

size_t getFileSize(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return 0;
    size_t size = file.tellg();
    file.close();
    return size;
}

int main()
{
    try {
        // Create test file with repetitive content for good compression
        std::string test_content = 
            "Huffman coding is a data compression algorithm that assigns shorter " 
            "binary codes to frequently occurring characters and longer codes to "
            "rare characters. This allows the total size of the encoded data to b"
            "e reduced while still being able to perfectly reconstruct the origin"
            "al information.";

        createTestFile("test.txt", test_content);

        std::cout << "Huffman Encoding Test" << std::endl;
        std::cout << std::endl;

        // Create Huffman encoder
        Huffman huffman;

        // Build tree
        std::cout << "1. Building Huffman tree from 'test.txt'..." << std::endl;
        huffman.build("test.txt");
        std::cout << std::endl;

        // Get original file size
        size_t original_size = getFileSize("test.txt");

        // Encode
        std::cout << "2. Encoding file..." << std::endl;
        double ratio = huffman.encode("test.txt", "test.huff");
        size_t encoded_size = getFileSize("test.huff");
        std::cout << "   Encoding successful!" << std::endl;
        std::cout << "   Original size: " << original_size << " bytes" << std::endl;
        std::cout << "   Encoded size:  " << encoded_size << " bytes" << std::endl;
        std::cout << "   Compression ratio: " << ratio << std::endl;
        std::cout << std::endl;

        // Export tree
        std::cout << "3. Exporting Huffman tree to 'tree.data'..." << std::endl;
        if (huffman.exportTree("tree.data")) {
            std::cout << "   Export successful!" << std::endl;
        }
        else {
            std::cout << "   Export failed!" << std::endl;
            return 1;
        }
        std::cout << std::endl;

        // Create new Huffman object and import tree
        Huffman huffman2;
        std::cout << "4. Importing tree from 'tree.data'..." << std::endl;
        if (huffman2.importTree("tree.data")) {
            std::cout << "   Import successful!" << std::endl;
        }
        else {
            std::cout << "   Import failed!" << std::endl;
            return 1;
        }
        std::cout << std::endl;

        // Decode
        std::cout << "5. Decoding 'test.huff' to 'test_decoded.txt'..." << std::endl;
        bool success = huffman2.decode("test.huff", "test_decoded.txt");
        if (success) {
            std::cout << "   Decoding successful!" << std::endl;

            // Verify
            std::string original, decoded;
            std::ifstream orig_file("test.txt", std::ios::binary);
            std::ifstream decoded_file("test_decoded.txt", std::ios::binary);

            original.assign((std::istreambuf_iterator<char>(orig_file)), std::istreambuf_iterator<char>());
            decoded.assign((std::istreambuf_iterator<char>(decoded_file)), std::istreambuf_iterator<char>());

            if (original == decoded) {
                std::cout << "   Verification: decoded file matches original!" << std::endl;
            }
            else {
                std::cout << "   Verification failed: files don't match!" << std::endl;
                return 1;
            }
        }
        else {
            std::cout << "   Decoding failed!" << std::endl;
            return 1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}