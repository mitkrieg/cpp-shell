#include <iostream>
#include <iomanip>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <ctime>
#include <cstring>

using namespace std;


int main () {
    DIR* dir = opendir(".");
    
    struct dirent* entry;

    cout << left << setw(10) << "Name" 
         << right << setw(12) << "Size" 
         << setw(30) << "Last Modified" 
         << setw(30) << "Last Opened"
         << setw(12) << "Permissions" << endl;
    cout << string(94, '-') << endl;
    
    int count = 0;
    int size = 0;
    while ((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0) {
            continue;
        }
        struct stat filestat;
        
        if (stat(entry->d_name, &filestat) == 0) {
            // ctime returns 26 char ending in \n\0, so take the first 24 and append \0 to null terminate the string
            char atime_str[26], mtime_str[26];
            strncpy(atime_str, ctime(&filestat.st_atime), 24);
            atime_str[24] = '\0';
            strncpy(mtime_str, ctime(&filestat.st_mtime), 24);
            mtime_str[24] = '\0';
            
            // Format permissions as octal
            mode_t mode = filestat.st_mode & 0777;
            
            cout << left << setw(10) << entry->d_name
                 << right << setw(12) << filestat.st_size
                 << setw(30) << mtime_str
                 << setw(30) << atime_str
                 << setw(12) << oct << mode << dec << endl;
                 
            count++;
            size += filestat.st_size;
        } else {
            cerr << "Couldn't find file: " << entry->d_name << endl;
        }
    }
    return 0;
}