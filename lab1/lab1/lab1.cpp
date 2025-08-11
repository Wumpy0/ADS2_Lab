#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <string>

std::vector<std::string> createTempFiles(int auxiliaryFiles) {
    std::vector<std::string> tempFiles;
    for (int i = 0; i < auxiliaryFiles; i++) {
        tempFiles.push_back("temp_" + std::to_string(i) + ".txt");
    }
    return tempFiles;
}

long long countNumbersInFile(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) return 0;

    long long count = 0;
    int temp;
    while (file >> temp) {
        count++;
    }
    file.close();
    return count;
}

void cleanupTempFiles(const std::vector<std::string>& tempFiles) {
    for (const auto& fileName : tempFiles) {
        std::remove(fileName.c_str());
    }
}

bool multiphaseSort(const std::string& inputFile, const std::string& outputFile, int auxiliaryFiles = 3) {
    if (auxiliaryFiles < 2) {
        auxiliaryFiles = 2;
    }

    std::vector<std::string> tempFiles = createTempFiles(auxiliaryFiles);

    long long totalNumbers = countNumbersInFile(inputFile);
    if (totalNumbers == 0) {
        cleanupTempFiles(tempFiles);
        return false;
    }


    return true;
}

bool createFileWithRandomNumbers(const std::string& fileName, const int numbersCount, const int minNumberValue, const int maxNumberValue) {
    std::ofstream file(fileName);
    if (!file.is_open()) {
        return false;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(minNumberValue, maxNumberValue);

    // Генерируем числа порциями, чтобы не загружать всю память
    const int batchSize = 10000;
    int remaining = numbersCount;

    while (remaining > 0) {
        int currentBatch = std::min(batchSize, remaining);

        for (int i = 0; i < currentBatch; i++) {
            file << dis(gen) << " ";
        }

        remaining -= currentBatch;
    }

    file.close();
    return true;
}

bool isFileContainsSortedArray(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        return false;
    }

    int current, next;
    if (!(file >> current)) {
        file.close();
        return true; // Пустой файл считается отсортированным
    }

    while (file >> next) {
        if (current > next) {
            file.close();
            return false;
        }
        current = next;
    }

    file.close();
    return true;
}



int main()
{
    //std::cout << "counting...\n";
    //std::cout << countNumbersInFile("file.txt") << std::endl;
    std::vector<std::string> tempFiles = createTempFiles(5);
    for (auto tempFile : tempFiles) {
        std::ofstream file(tempFile);
        file.close();
    }
    std::cout << "...\n";
    std::cin.get();
    cleanupTempFiles(tempFiles);
}