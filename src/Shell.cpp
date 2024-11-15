#include "Shell.h"
#include "Commands/Commands.h"
#include <unistd.h>
#include <fstream> 
#include <algorithm>
#include <filesystem>
#include <dirent.h>

namespace fs = std::filesystem;

Shell::Shell() {
    register_builtin_commands();
}

Shell::~Shell() {
    for (auto& command : commands) {
        delete command.second;
    }
}

void Shell::register_builtin_commands() {
    commands["cd"] = new CdCommand();
    commands["history"] = new HistoryCommand();
    commands["clear"] = new ClearCommand();
}

std::vector<std::string> Shell::listDirectory(const std::string &path) {
    std::vector<std::string> files;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            files.push_back(ent->d_name);
        }
        closedir(dir);
    } else {
        perror("Could not open directory");
    }
    return files;
}

std::string Shell::autocomplete(const std::string &input) {
    std::string ret = input;
    std::vector<std::string> files = listDirectory(".");

    std::string lastword = "";
    for (auto i = 0; i < input.size(); i++) {
        if (input[i] == ' ') {
            lastword = "";
        } else {
            lastword += input[i];
        }
    }

    std::vector<std::string> matches;
    for (const auto &file : files) {
        if (file.find(lastword) == 0) {
            matches.push_back(file);
        }
    }

    if (matches.size() == 1) {
        for (auto i = lastword.size(); i < matches[0].size(); i++) {
            std::cout << matches[0][i];
            ret += matches[0][i];
        }
    } else {
        if (matches.empty()) {
            std::cout << "\nNo matches found." << std::endl;
        } else if (matches.size() > 1) {
            std::cout << "\nMatches:";
            for (const auto &match : matches) {
                std::cout << "\n" << match;
            }
            std::cout << std::endl;
        }

        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != nullptr) {
            std::cout << "shell " << cwd << "> " << input;
        } else {
            std::cerr << "getcwd() error" << std::endl;
        }
    }
    return ret;
}

void Shell::execute_command(const std::string& input) {
    std::vector<std::string> args = split_input(input);
    if (args.empty()) return;

    if (access("/home/theguppyfish/lab1OS/.history", F_OK) != 0) {
        std::ofstream file("/home/theguppyfish/lab1OS/.history");
    }

    FILE *file;
    file = fopen("/home/theguppyfish/lab1OS/.history", "a");
    if (file == NULL) {
        perror("Error opening file .history");
    }

    if (fputs(input.c_str(), file) == EOF) {
        perror("Error writing to file .history");
        fclose(file);
    }
    if (fputs("\n", file) == EOF) {
        perror("Error writing to file .history");
        fclose(file);
    }
    fclose(file);

    std::string cmd_name = args[0];
    args.erase(args.begin());

    if (commands.find(cmd_name) != commands.end()) {
        commands[cmd_name]->execute(args);
    } else {
        std::vector<char*> exec_args;
        exec_args.push_back(const_cast<char*>(cmd_name.c_str()));
        for (auto& arg : args) {
            exec_args.push_back(const_cast<char*>(arg.c_str()));
        }
        exec_args.push_back(nullptr);

        if (execvp(cmd_name.c_str(), exec_args.data()) == -1) {
            std::cerr << "Unknown command or failed to execute: " << cmd_name << std::endl;
        }
    }
}

std::vector<std::string> Shell::split_input(const std::string& input) {
    std::vector<std::string> tokens;
    std::string token;
    for (char ch : input) {
        if (isspace(ch)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token.push_back(ch);
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}