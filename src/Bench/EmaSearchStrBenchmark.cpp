#include "EmaSearchStrBenchmark.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

// char ExternalMemorySearcher::randomChar() {
//     static std::random_device rd;
//     static std::mt19937 engine(rd());
//     static std::uniform_int_distribution<int> dist(32, 126); // generint(4 bytes) // можно сгенерить файл с помощью /dev/random

//     return static_cast<char>(dist(engine));
// }

// void ExternalMemorySearcher::generateRandomFile(const std::string& filename, size_t size_in_mb) {
//     std::ofstream file(filename);
//     if (!file) {
//         std::cerr << "Failed to open the file for writing: " << filename << std::endl;
//         return;
//     }

//     size_t num_chars = size_in_mb * 1024 * 1024;
//     for (size_t i = 0; i < num_chars; ++i) {
//         file.put(randomChar());
//     }

//     std::cout << "Random file generated: " << filename << " (" << size_in_mb << " MB)" << std::endl;
// }

void ExternalMemorySearcher::searchStringInChunks(const std::string& filename, const int& target_int, size_t chunk_size_mb) {
    int fd = open(filename.c_str(), O_RDONLY | O_DIRECT);
    if (fd == -1) {
        std::cerr << "Failed to open the file for reading: " << filename << std::endl;
        return;
    }

    const size_t max_chunk_size_in_bytes = 32 * 1024 * 1024;
    size_t chunk_size_in_bytes = std::min(chunk_size_mb, 32ul) * 1024 * 1024; // Читаем не больше 32 MB за раз

    std::vector<char> buffer(chunk_size_in_bytes);
    size_t chunk_number = 0;
    bool found = false;

    while (true) {
        chunk_number++;
        ssize_t bytes_read = read(fd, buffer.data(), chunk_size_in_bytes);

        if (bytes_read <= 0) {
            break;
        }

        for (size_t i = 0; i <= bytes_read - 4; ++i) {
            int* chunk_int_ptr = reinterpret_cast<int*>(&buffer[i]);
            if (*chunk_int_ptr == target_int) {
                std::cout << "Integer found in chunk " << chunk_number << ", position: " << i << std::endl;
                found = true;
            }
        }
    }

    if (!found) {
        std::cout << "Integer not found in the file." << std::endl;
    }

    close(fd);
}

void ExternalMemorySearcher::externalMemorySearch(
    const std::string& input_filename,
    const int& target_int,
    size_t chunk_size_mb
) {
    searchStringInChunks(input_filename, target_int, chunk_size_mb);
}

void ExternalMemorySearcher::printHelp() {
    std::cout << "Available subcommands:\n"
              << "\tfull-benchmark <input_file> <target_string> <chunk_size_mb>\n\t\tSearch for a string in the file in chunks (using external memory)\n"
              << "\thelp\n\t\tPrint this message (without arguments).\n";
}
