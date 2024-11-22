#include "EmaSearchStrBenchmark.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <cstdio>

char ExternalMemorySearcher::randomChar() {
    static std::random_device rd;
    static std::mt19937 engine(rd());
    static std::uniform_int_distribution<int> dist(32, 126); 

    return static_cast<char>(dist(engine));
}

void ExternalMemorySearcher::generateRandomFile(const std::string& filename, size_t size_in_mb) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open the file for writing: " << filename << std::endl;
        return;
    }

    size_t num_chars = size_in_mb * 1024 * 1024;
    for (size_t i = 0; i < num_chars; ++i) {
        file.put(randomChar());
    }

    std::cout << "Random file generated: " << filename << " (" << size_in_mb << " MB)" << std::endl;
}

void ExternalMemorySearcher::searchStringInFile(const std::string& filename, const std::string& target_string) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open the file for reading: " << filename << std::endl;
        return;
    }

    std::string line;
    size_t line_number = 0;
    size_t target_length = target_string.length();
    bool found = false;

    while (std::getline(file, line)) {
        line_number++;
        size_t pos = line.find(target_string);
        if (pos != std::string::npos) {
            std::cout << "String found at line " << line_number << ", position: " << pos << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "String not found in the file." << std::endl;
    }

    file.close();
}

void ExternalMemorySearcher::searchStringInChunks(const std::string& filename, const std::string& target_string, size_t chunk_size_mb) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open the file for reading: " << filename << std::endl;
        return;
    }

    size_t chunk_size_in_bytes = chunk_size_mb * 1024 * 1024;
    size_t chunk_number = 0;
    std::vector<char> buffer(chunk_size_in_bytes);
    size_t target_length = target_string.length();

    bool found = false;
    
    std::string previous_chunk_end = "";

    while (file.read(buffer.data(), chunk_size_in_bytes) || file.gcount() > 0) {
        chunk_number++;
        size_t read_size = file.gcount();
        std::string chunk_data(buffer.begin(), buffer.begin() + read_size);

        std::string search_data = previous_chunk_end + chunk_data;
        size_t pos = search_data.find(target_string);
        if (pos != std::string::npos) {
            std::cout << "String found in chunk " << chunk_number << ", position: " << pos << std::endl;
            found = true;
        }

        size_t overlap_size = target_length - 1;
        if (overlap_size > 0) {
            previous_chunk_end = chunk_data.substr(chunk_data.length() - overlap_size);
        }
    }

    if (!found) {
        std::cout << "String not found in the file." << std::endl;
    }

    file.close();
}

void ExternalMemorySearcher::externalMemorySearch(
    const std::string& input_filename,
    const std::string& target_string,
    size_t chunk_size_mb
) {
    searchStringInChunks(input_filename, target_string, chunk_size_mb);
}

void ExternalMemorySearcher::printHelp() {
    std::cout << "Available subcommands:\n"
              << "\tgenerate <output_file> <size_mb>\n\t\tGenerate a random text file\n"
              << "\tsearch <input_file> <target_string>\n\t\tSearch for a string in the file\n"
              << "\tsearch-chunks <input_file> <target_string> <chunk_size_mb>\n\t\tSearch for a string in the file in chunks (using external memory)\n"
              << "\thelp\n\t\tPrint this message (without arguments).\n";
}
