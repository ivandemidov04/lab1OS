#ifndef CLEARCOMMAND_H
#define CLEARCOMMAND_H

#include "Command.h"

class ClearCommand : public Command {
public:
    void execute(const std::vector<std::string>& args) override;
};

#endif
