#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (i > 1) cout << ' ';
        cout << argv[i];
    }
    cout << '\n';
    
    return 0;
}