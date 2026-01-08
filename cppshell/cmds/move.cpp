#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;

void help() {
    cout << "Usage:\n"
         << "  move help\n"
         << "  move file <old_path> <new_path>\n";

}

int main (int argc, char* argv[]) {

    if (argc < 2){
        cerr << "No arguments provided" << endl;
        help();
        return 1;
    }

    string subcommand = argv[1];
    if (subcommand == "help") {
        help();
        return 0;
    }
    if (argc < 3){
        cerr << "move: Expected 2 args"<< endl;
        help();
        return 1;
    }

    return rename(argv[1], argv[2]);
}