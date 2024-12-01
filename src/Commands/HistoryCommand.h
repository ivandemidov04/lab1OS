#ifndef HISTORYCOMMAND_H
#define HISTORYCOMMAND_H

#include "Command.h"

class HistoryCommand : public Command {
public:
    void execute(const std::vector<std::string>& args) override;
};

#endif
