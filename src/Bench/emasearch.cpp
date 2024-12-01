#include "EmaSearchStrBenchmark.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        ExternalMemorySearcher::printHelp();
        return 1;
    }

    std::string command = argv[1];

    // if (command == "generate") {
    //     if (argc != 4) {
    //         std::cout << "Usage: ./emasearch generate <output_file> <size_mb>" << std::endl;
    //         return 1;
    //     }
    //     std::string output_file = argv[2];
    //     size_t size_mb = std::stoull(argv[3]);
    //     ExternalMemorySearcher::generateRandomFile(output_file, size_mb);
    // }
    // else if (command == "search-chunks") {
    //     if (argc != 5) {
    //         std::cout << "Usage: ./emasearch search-chunks <input_file> <target_string> <chunk_size_mb>" << std::endl;
    //         return 1;
    //     }
    //     std::string input_file = argv[2];
    //     std::string target_string = argv[3];
    //     size_t chunk_size_mb = std::stoull(argv[4]);
    //     ExternalMemorySearcher::externalMemorySearch(input_file, target_string, chunk_size_mb);
    // }
    // else 
    if (command == "help") {
        ExternalMemorySearcher::printHelp();
    }
    else if (command == "full-benchmark") {
        if (argc != 5) {
            std::cout << "Usage: ./emasearch full-benchmark <input_file> <target_string> <repeat-count>" << std::endl;
            return 1;
        }
        std::string input_file = argv[2];
        int target_int = std::stoi(argv[3]);
        size_t repeat_count = std::stoull(argv[4]);

        for (size_t i = 0; i < repeat_count; ++i) {
            // ExternalMemorySearcher::generateRandomFile(input_file, 256);
            ExternalMemorySearcher::externalMemorySearch(input_file, target_int, 256);
        }
    }
    else {
        std::cout << "Unknown subcommand: " << command << std::endl;
        ExternalMemorySearcher::printHelp();
        return 1;
    }

    return 0;
}
