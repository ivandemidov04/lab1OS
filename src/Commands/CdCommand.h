#ifndef CDCOMMAND_H
#define CDCOMMAND_H

#include "Command.h"

class CdCommand : public Command {
public:
    void execute(const std::vector<std::string>& args) override;
};

#endif
