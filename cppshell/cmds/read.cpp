#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cerr << "not enough args: read <filename>" << endl;
        return 1;
    }

    char* filename = argv[1];

    int file = open(filename, O_RDONLY);
    if (file < 0) {
        cerr << "cannot open file: " << filename << endl;
        return 1;
    }

    char buffer[4096];

    ssize_t n;

    while ((n = read(file, buffer, sizeof(buffer))) > 0 ) {
        cout.write(buffer, n);
    }
    
    if (n < 0) {
        perror("read");
        close(file);
        return 1;
    }
    
    cout << endl;
    close(file);
    return 0;
}