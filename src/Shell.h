#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "Commands/Command.h"

class Shell {
private:
    std::map<std::string, Command*> commands;
    std::vector<std::string> split_input(const std::string& input); 

public:
    Shell();
    ~Shell();

    void register_builtin_commands();
    std::vector<std::string> listDirectory(const std::string &path);
    std::string autocomplete(const std::string& input);
    void execute_command(const std::string& input);
};

#endif
