#include "HistoryCommand.h"
#include <iostream>
#include <unistd.h>
#include <filesystem>

namespace fs = std::filesystem;

void HistoryCommand::execute(const std::vector<std::string>& args) {
    if (args.empty()) {
        if (access(".history", F_OK) == 0) {
            FILE *file;
            char ch;

            file = fopen(".history", "r");
            if (file == NULL) {
                perror("Error opening file .history");
            }

            while ((ch = fgetc(file)) != EOF) {
                putchar(ch);
            }

            fclose(file);
        }
        return;
    } else {
        std::cerr << "failed to print history" << std::endl;
    }
}