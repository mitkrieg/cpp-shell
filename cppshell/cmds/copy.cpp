#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;




int main(int argc, char* argv[]) {

    if (argc != 3) {
        cerr << "not enough args: copy <original> <copy>" << endl;
        return 1;
    }

    char* src = argv[1];
    char* dst = argv[2];
    int original = open(src, O_RDONLY);
    if (original < 0) {
        cerr << "cannot open original: " << src << endl;
        return 1;
    }

    struct stat orig_st;
    fstat(original, &orig_st);

    int copy = open(dst, O_WRONLY | O_CREAT | O_TRUNC, orig_st.st_mode);
    if (copy < 0) {
        cerr << "couldn't create or find: " << dst << endl;
        close(original);
        return 1;
    }

    // create buffer of 4096 bytes
    char buffer[4096];
    // use n to hold the number of bytes read (aka what's left to write)
    ssize_t n;
    // to read you have to provide a writeable place and the amount that can be written there
    while ((n  = read(original, buffer, sizeof(buffer))) > 0 ) {
        ssize_t written = 0;
        // while there's still stuff left to write
        while (written < n) {
            // get a chunk to write add the size to the buffer and subtract it from what's left to write
            ssize_t w = write(copy, buffer + written, n - written);
            if (w < 0) {
                perror("write");
                close(original);
                close(copy);
                return 1;

            }
            written += w;
        }
    }

    if (n < 0) {
        perror("read");
        close(original);
        close(copy);
    }
    close(original);
    close(copy);
    return 0;



}