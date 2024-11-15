#include "ClearCommand.h"
#include <iostream>
#include <unistd.h>
#include <filesystem>

namespace fs = std::filesystem;

void ClearCommand::execute(const std::vector<std::string>& args) {
    if (args.empty()) {
        if (access("/home/theguppyfish/lab1OS/.history", F_OK) == 0) {
            fs::remove("/home/theguppyfish/lab1OS/.history");
        }
        std::cerr << "history cleared" << std::endl;
        return;
    } else {
        std::cerr << "failed to clear history" << std::endl;
    }
}