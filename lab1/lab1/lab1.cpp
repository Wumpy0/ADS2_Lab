#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <string>
#include <chrono>

std::vector<std::string> createTempFiles(int auxiliaryFiles) {
    std::vector<std::string> tempFiles;
    for (int i = 0; i < auxiliaryFiles; i++) {
        tempFiles.push_back("temp_" + std::to_string(i) + ".txt");
    }
    return tempFiles;
}

long long countNumbersInFile(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open " + fileName);
    }

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

bool multiphaseSort(const std::string& inputFile, const std::string& outputFile, int auxiliaryFiles = 3, long long inputFileSize = -1) {
    std::cout << "Starting multiphaseSort()..." << std::endl;
    if (auxiliaryFiles < 2) {
        auxiliaryFiles = 2;
    }
    std::cout << "Number of auxiliaryFiles: " << auxiliaryFiles << std::endl;
    std::cout << "Input file: " << inputFile << std::endl;
    std::cout << "Output file: " << outputFile << std::endl;

    std::vector<std::string> tempFiles = createTempFiles(auxiliaryFiles);

    if (inputFileSize < 0) {
        long long inputFileSize = countNumbersInFile(inputFile);
    }
    if (inputFileSize == 0) {
        return true;
    }
    std::cout << "Size of input file: " << inputFileSize << std::endl;

    // Разделяем исходный файл
    int runSize = std::max(1000LL, inputFileSize / (auxiliaryFiles * 100));
    std::cout << "Run size: " << runSize << std::endl;
    std::cout << "1. Splitting the input file..." << std::endl;

    std::ifstream input(inputFile);
    if (!input.is_open()) {
        throw std::runtime_error("Cannot open input file");
    }

    std::vector<int> buffer(runSize);
    int currentFile = 0;
    int number;

    auto start = std::chrono::high_resolution_clock::now();

    // Открываем вспомогательные файлы
    std::vector<std::ofstream> outputStreams;
    for (const auto& fileName : tempFiles) {
        std::ofstream stream(fileName, std::ios::app);
        if (!stream.is_open()) {
            throw std::runtime_error("Cannot open temporary file: " + fileName);
        }
        outputStreams.push_back(std::move(stream));
    }
    // Записываем блоки в файлы
    while (input >> number) {
        buffer.clear();
        buffer.push_back(number);

        // Читаем блок размером runSize
        for (int i = 1; i < runSize && (input >> number); i++) {
            buffer.push_back(number);
        }

        // Сортируем блок
        std::sort(buffer.begin(), buffer.end());

        // Записываем отсортированный блок во временный файл
        for (size_t i = 0; i < buffer.size(); i++) {
            outputStreams[currentFile] << buffer[i];
            if (i < buffer.size() - 1) outputStreams[currentFile] << " ";
        }
        outputStreams[currentFile] << "\n"; // Разделитель между отсортированными блоками

        currentFile = (currentFile + 1) % tempFiles.size();
    }
    // Закрываем вспомогательные файлы
    for (auto& stream : outputStreams) {
        stream.close();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Time needed for splitting: " << duration << " ms\n";

    input.close();

    cleanupTempFiles(tempFiles);
    return true;
}

bool createFileWithRandomNumbers(const std::string& fileName, const int numbersCount, const int minNumberValue, const int maxNumberValue) {
    std::ofstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open " + fileName + " during the generation random numbers");
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
        throw std::runtime_error("Cannot open " + fileName + " during the sorting check");
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
    try {
        multiphaseSort("file.txt", "result.txt", 5, 1000000);
    } 
    catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}