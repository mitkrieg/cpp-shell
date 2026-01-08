#include <iostream>
#include <unistd.h>

using namespace std;

int main () {
    char* cwd = getcwd(nullptr, 0); // nullptr in getcwd allocates mem for us

    if (cwd == nullptr) {
        perror("pwd");
        return 1;
    }

    cout << cwd << endl;
    free(cwd);
}