#ifndef EMA_SEARCH_STR_H
#define EMA_SEARCH_STR_H

#include <cstdint>
#include <string>

class ExternalMemorySearcher {
private:
    // static char randomChar();
    static void searchStringInChunks(const std::string& filename, const int& target_int, size_t chunk_size_mb);

public:
    // static void generateRandomFile(const std::string& filename, size_t size_mb);
    static void externalMemorySearch(
        const std::string& input_filename,
        const int& target_int,
        size_t chunk_size_mb
    );
    static void printHelp();
};

#endif