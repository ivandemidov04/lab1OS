#include "FactorizeBenchmark.h"
#include "EmaSearchStrBenchmark.h"
#include <iostream>
#include <string>

void print_usage(const char* program_name) {
    std::cerr << "Usage: " << program_name
              << " --factorize-iterations <n> [--number <value>] --emasearch-command <command>" << std::endl;
    std::cerr << "  --factorize-iterations <n>     : Number of factorization iterations (required)" << std::endl;
    std::cerr << "  --number <value>               : Number to factorize (optional, default=1234567890123456789)" << std::endl;
    std::cerr << "  --emasearch-command <command>  : EmaSearch command to run (required)" << std::endl;
    std::cerr << "    Valid commands: full-benchmark" << std::endl;
}

int main(int argc, char* argv[]) {
    int factorize_iterations = 0;
    long long number = 1234567890123456789LL;
    std::string emasearch_command;

    std::cout << "Starting Combined Benchmark" << std::endl;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--factorize-iterations" && i + 1 < argc) {
            factorize_iterations = std::stoi(argv[++i]);
            if (factorize_iterations <= 0) {
                std::cerr << "Error: Factorize iterations must be a positive integer." << std::endl;
                return 1;
            }
        }
        else if (arg == "--number" && i + 1 < argc) {
            number = std::stoll(argv[++i]);
            if (number <= 1) {
                std::cerr << "Error: Number must be greater than 1 for factorization." << std::endl;
                return 1;
            }
        }
        else if (arg == "--emasearch-command" && i + 1 < argc) {
            emasearch_command = argv[++i];
            break;
        }
        else {
            std::cerr << "Unknown or incomplete argument: " << arg << std::endl;
            print_usage(argv[0]);
            return 1;
        }
    }

    if (factorize_iterations == 0 || emasearch_command.empty()) {
        std::cerr << "Error: Both --factorize-iterations and --emasearch-command are required." << std::endl;
        print_usage(argv[0]);
        return 1;
    }

    std::cout << "Running Factorization Benchmark" << std::endl;
    FactorizeBenchmark::run(factorize_iterations, number);

    std::cout << "Running EmaSearch Benchmark (" << emasearch_command << ")" << std::endl;

    // if (emasearch_command == "generate") {
    //     if (argc != 5) {
    //         std::cout << "Usage: --emasearch-command generate <output_file> <size_mb>" << std::endl;
    //         return 1;
    //     }
    //     std::string output_file = argv[4];
    //     size_t size_mb = std::stoull(argv[5]);
    //     ExternalMemorySearcher::generateRandomFile(output_file, size_mb);
    // }
    // else if (emasearch_command == "search-chunks") {
    //     if (argc != 6) {
    //         std::cout << "Usage: --emasearch-command search-chunks <input_file> <target_string> <chunk_size_mb>" << std::endl;
    //         return 1;
    //     }
    //     std::string input_file = argv[4];
    //     std::string target_string = argv[5];
    //     size_t chunk_size_mb = std::stoull(argv[6]);
    //     ExternalMemorySearcher::externalMemorySearch(input_file, target_string, chunk_size_mb);
    // }
    // else 
    if (emasearch_command == "full-benchmark") {
        if (argc != 8) {
            std::cout << "Usage: --emasearch-command full-benchmark <input_file> <target_string> <repeat-count>" << std::endl;
            return 1;
        }
        std::string input_file = argv[5];
        int target_int = std::stoi(argv[6]);
        size_t repeat_count = std::stoull(argv[7]);

        for (size_t i = 0; i < repeat_count; ++i) {
            // ExternalMemorySearcher::generateRandomFile(input_file, 256);
            ExternalMemorySearcher::externalMemorySearch(input_file, target_int, 256);
        }
    }
    else {
        std::cerr << "Unknown EmaSearch command: " << emasearch_command << std::endl;
        ExternalMemorySearcher::printHelp();
        return 1;
    }

    std::cout << "Both benchmarks have completed successfully." << std::endl;
    return 0;
}
