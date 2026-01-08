#include <iostream>
#include <string>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>


using namespace std;

void help() {
    cout << "Usage:\n"
         << "  new help\n"
         << "  new file <filename>\n"
         << "  new dir  <dirname> [mode_octal]\n\n"
         << "Notes:\n"
         << "  - mode is interpreted as octal (e.g., 755 or 0755). Default is 0755.\n";
}

int new_file(const char* filename) {
    
    // O_CREAT Create if missing, O_EXCL fail if already exists, O_WRONLY open for writing only
    int file = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0644);

    if (file < 0) {
        if (errno == EEXIST) {
            cerr << "File already exists: " << filename << endl;
        } else {
            cerr << "Failed to create file: " << filename << " (" << strerror(errno) << ")" << endl;
        }
        return 1;
    }

    close(file);

    return 0;
}

int new_dir(const char* dirname, int mode) {

    if (mkdir(dirname, mode) != 0) {
        if (errno == EEXIST) {
            cerr << "Directory already exists: " << dirname << endl;
        } else {
            cerr << "Filed to create directory: " << dirname << " (" << strerror(errno) << ")" << endl;
        }
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    
    if (argc < 2){
        cerr << "No arguments provided";
        help();
        return 1;
    }

    string subcommand = argv[1];
    if (subcommand == "help") {
        help();
        return 0;
    }
    else if (subcommand == "file") {
        if (argc < 3) {
            cerr << "new file: missing <filename>" << endl;
            help();
            return 1;
        }
        return new_file(argv[2]);
    }
    else if (subcommand == "dir") {
        if (argc < 3) {
            cerr << "new dir: missing <dirname>" << endl;
            help();
            return 1;
        }
        long mode = 0755;
        if (argv[3] != nullptr) {
            char* end = nullptr;
            long parsed = strtol(argv[3], &end, 8);
        
            if (end == argv[3] || *end != '\0' || parsed < 0 || parsed > 077) {
                cerr << "new dir: invalid mode" << endl;
                return 1;
            }
            mode = static_cast<mode_t>(parsed);
        }
        return new_dir(argv[2], mode);
    }

    cerr << "Unknown subcommand: " << subcommand << endl;
    help();
    return 1;
}