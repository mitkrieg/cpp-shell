#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>

using namespace std;


int execute_command(vector<string> &args) {
    if (args.empty()) {
        return 0;
    }
    vector<char *> cmd_args;
    for (auto &s : args) {
        // .c_str() returns a const char* and we need to remove the constness to pass it to execvp
        cmd_args.push_back(const_cast<char *>(s.c_str()));
    }
    cmd_args.push_back(nullptr);

    pid_t pid = fork();
    int status;

    if (pid == 0) {
        // this will replace the current process with the child process
        execvp(cmd_args[0], cmd_args.data());
        perror("execvp");
        _exit(1);
    } else if (pid < 0) {
        // fork failed
        perror("fork");
        return 1;
    } else {
        // wait for the child process to finish
        waitpid(pid, &status, 0);
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    } 
    if (WIFSIGNALED(status)) {
        return 128 + WTERMSIG(status);
    }
    return status;
}

int change_directory(vector<string> &args) {
    // get the path from the arguments
    const char* path = nullptr;
    if (args.size() > 1) {
        path = args[1].c_str();
    }
    // if the path is not set, use the HOME environment variable
    if (path == nullptr) {
        path = getenv("HOME");
    }
    // If path still not set there is no home directory
    if (path == nullptr) {
        cerr << "HOME environment variable is not set" << endl;
        return 1;
    }
    // change the directory to the path
    if (chdir(path) != 0) {
        perror("cd");
        return 1;
    }
    return 0;
}