#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <string>
#include <chrono>

std::ifstream openInputFile(const std::string& fileName, std::ios::openmode mode = std::ios::in) {
    std::ifstream file(fileName, mode);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open " + fileName);
    }
    return file;
}

std::ofstream openOutputFile(const std::string& fileName, std::ios::openmode mode = std::ios::out) {
    std::ofstream file(fileName, mode);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open " + fileName);
    }
    return file;
}

std::vector<std::ifstream> openInputFiles(const std::vector<std::string>& fileNames, std::ios::openmode mode = std::ios::in) {
    std::vector<std::ifstream> streams;
    streams.reserve(fileNames.size());

    for (const auto& name : fileNames) {
        streams.emplace_back(name, mode);
        if (!streams.back().is_open()) {
            throw std::runtime_error("Cannot open " + name);
        }
    }

    return streams;
}

std::vector<std::ofstream> openOutputFiles(const std::vector<std::string>& fileNames, std::ios::openmode mode = std::ios::out) {
    std::vector<std::ofstream> streams;
    streams.reserve(fileNames.size());

    for (const auto& name : fileNames) {
        streams.emplace_back(name, mode);
        if (!streams.back().is_open()) {
            throw std::runtime_error("Cannot open " + name);
        }
    }

    return streams;
}

void closeAllFiles(std::vector<std::ofstream>& fileNames) {
    for (auto& stream : fileNames) {
        stream.close();
    }
}

void closeAllFiles(std::vector<std::ifstream>& fileNames) {
    for (auto& stream : fileNames) {
        stream.close();
    }
}

std::vector<std::string> createTempFiles(int auxiliaryFiles, std::string pattern = "temp_") {
    std::vector<std::string> tempFiles;
    for (int i = 0; i < auxiliaryFiles; i++) {
        tempFiles.push_back(pattern + std::to_string(i) + ".txt");
    }
    return tempFiles;
}

long long countNumbersInFile(const std::string& fileName) {
    std::ifstream file = openInputFile(fileName);
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
        inputFileSize = countNumbersInFile(inputFile);
    }
    if (inputFileSize == 0) {
        return true;
    }
    std::cout << "Size of input file: " << inputFileSize << std::endl;

    // Разделяем исходный файл
    int runSize = std::max(1000LL, inputFileSize / (auxiliaryFiles * 100));
    std::cout << "Run size: " << runSize << std::endl;
    std::cout << "1. Splitting the input file..." << std::endl;

    std::ifstream input = openInputFile(inputFile);

    std::vector<int> buffer(runSize);
    int currentFile = 0;
    int numberFromFile;

    // Открываем вспомогательные файлы
    std::vector<std::ofstream> outputTempFilesStreams = openOutputFiles(tempFiles, std::ios::app);

    // Записываем блоки в файлы
    while (input >> numberFromFile) {
        buffer.clear();
        buffer.push_back(numberFromFile);

        // Читаем блок размером runSize
        for (int i = 1; i < runSize && (input >> numberFromFile); i++) {
            buffer.push_back(numberFromFile);
        }

        // Сортируем блок
        std::sort(buffer.begin(), buffer.end());

        // Записываем отсортированный блок во временный файл
        for (size_t i = 0; i < buffer.size(); i++) {
            outputTempFilesStreams[currentFile] << buffer[i];
            if (i < buffer.size() - 1) outputTempFilesStreams[currentFile] << " ";
        }
        outputTempFilesStreams[currentFile] << "\n"; // Разделитель между отсортированными блоками

        currentFile = (currentFile + 1) % tempFiles.size();
    }
    // Закрываем файлы
    closeAllFiles(outputTempFilesStreams);
    input.close();

    // Фаза слияния
    std::cout << "2. Merging phase..." << std::endl;
    int mergingPhaseNumber = 1;
    bool hasMorePhase = true;
    while (hasMorePhase) {
        std::cout << "  Merging phase №" + std::to_string(mergingPhaseNumber) << std::endl;
        // Проверяем, какие файлы содержат данные
        std::vector<int> activeFiles;
        for (int i = 0; i < auxiliaryFiles; i++) {
            std::ifstream file(tempFiles[i]);
            if (file.is_open()) {
                std::string line;
                if (std::getline(file, line) && !line.empty()) {
                    activeFiles.push_back(i);
                }
                file.close();
            }
        }

        if (activeFiles.size() <= 1) {
            hasMorePhase = false; // Сортировка завершена или нечего сливать
        }

        // Создаём новые вспомогательные файлы для записи в них
        std::vector<std::string> newTempFiles = createTempFiles(auxiliaryFiles, "new_temp_");

        // Открываем вспомогательные файлы для чтения
        std::vector<std::ifstream> inputTempFilesStreams = openInputFiles(tempFiles);

        // Открываем новые вспомогательные файлы для записи
        std::vector<std::ofstream> outputNewTempFilesStreams = openOutputFiles(newTempFiles, std::ios::app);
        
        // Сливаем блоки
        int outputFileIndex = 0;
        bool hasMoreRuns = true;

        while (hasMoreRuns) {
            
        }

        // Закрываем все вспомогательные файлы
        closeAllFiles(inputTempFilesStreams);
        closeAllFiles(outputNewTempFilesStreams);

        // Заменяем старые временные файлы новыми
        for (int i = 0; i < auxiliaryFiles; i++) {
            std::remove(tempFiles[i].c_str());
            std::rename(newTempFiles[i].c_str(), tempFiles[i].c_str());
        }

        mergingPhaseNumber++;
    }



    cleanupTempFiles(tempFiles);
    return true;
}

bool createFileWithRandomNumbers(const std::string& fileName, const int numbersCount, const int minNumberValue, const int maxNumberValue) {
    std::ofstream file = openOutputFile(fileName);

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
    std::ifstream file = openInputFile(fileName);

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
        std::cout << (isFileContainsSortedArray("result.txt")) ? "File is sorted!" : "After multiphaseSort() file still unsorted";
    } 
    catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}