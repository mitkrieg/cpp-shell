#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>

using namespace std;

void help() {
    cout << "Usage:\n"
         << "  delete help\n"
         << "  delete file <filename>\n"
         << "  delete dir  <dirname>\n"
         << "  ** delete dir behaves recursively and will" << endl
         << "     delete all files contained within" 
         << endl;
}

int delete_file(char* filename) {

    if (unlink(filename) != 0) {
        if (errno == ENOENT) {
            cerr << "No file: " << filename << endl;
        } else {
            cerr << "Failed to delete file: " << filename << " (" << strerror(errno) << ")" << endl;
        }
        return 1;
    }

    return 0;
}

int delete_dir(char* dirname) {
    DIR* dir = opendir(dirname);

    if (!dir) {
        if (errno == ENOENT) {
            cerr << "No such dir: " << dirname << endl;
        } else {
            cerr << "Failed to check file: " << dirname << " (" << strerror(errno) << ")" << endl;
        }
        return 1;
    }
    struct dirent* entry;
    while((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        string path = string(dirname) + "/" + entry->d_name;
        struct stat st;
        if (stat(path.c_str(), &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                if (delete_dir(const_cast<char *>(path.c_str())) != 0) {
                    closedir(dir);
                    return 1;
                }
            } else {
                if (unlink(path.c_str()) != 0) {
                    cerr << "Failed to delete file: " << path << " (" << strerror(errno) << ")" << endl;
                    closedir(dir);
                    return 1;
                }
            }
        }
    }
    closedir(dir);
    if (rmdir(dirname) != 0) {
        cerr << "Failed to delete directory: " << dirname << " (" << strerror(errno) << ")" << endl;
        return 1;
    }
    return 0;
}

int main (int argc, char* argv[]) {

    if (argc < 2) {
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
            cerr << "delete file: missing <filename>" << endl;
            help();
            return 1;
        }

        return delete_file(argv[2]);
    }
    else if (subcommand == "dir") {
        if (argc < 3) {
            cerr << "delete dir: missing <dirname>" << endl;
            help();
            return 1;
        }

        return delete_dir(argv[2]);
    }
    cerr << "Unknown subcommand: " << subcommand << endl;
    help();
    return 1;
}