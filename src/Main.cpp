#include <iostream>
#include <vector>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <chrono>
#include <linux/sched.h>
#include <termios.h>
#include "Shell.h"

void setRawMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}


int clone3(struct clone_args *cl_args, size_t size) {
    return syscall(SYS_clone3, cl_args, size);
}

void execute_task(Shell& shell, const std::string& command) {
    auto start = std::chrono::high_resolution_clock::now();

    struct clone_args cl_args = {0};
    cl_args.flags = CLONE_FS | CLONE_FILES;
    cl_args.exit_signal = SIGCHLD;

    int pid = clone3(&cl_args, sizeof(cl_args));
    if (pid == 0) {
        std::vector<char*> args;
        shell.execute_command(command);
        exit(0);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;
        std::cout << "Execution time: " << elapsed_time.count() << " seconds" << std::endl;
    } else {
        std::cerr << "clone3 failed!" << std::endl;
    }
}

int main() {
    Shell shell;
    std::string command;
    setRawMode();

    while (1) {
        command = "";
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != nullptr) {
            std::cout << "shell " << cwd << "> ";
        } else {
            std::cerr << "getcwd() error" << std::endl;
        }

        char ch;
        while ((ch = getchar()) != '\n') {
            if (ch == 9) {
                command = shell.autocomplete(command);
            } else if (ch == 127) {
                if (command.size() > 0) {
                    command.pop_back();
                }
                std::cout << "\n" << "shell " << cwd << "> " << command;
            } else {
                std::cout << ch;
                command += ch;
            }
        }
        std::cout << "\n";

        if (command == "exit") break;
        execute_task(shell, command);
    }

    return 0;
}