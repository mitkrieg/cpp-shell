#include <iostream>
#include <sys/stat.h>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

using namespace std;

void help() {
    cout << "Usage:\n"
    << "  info <filename>\n" << endl;
}

string perms_to_string(mode_t mode) {
    std::string p = "---------";

    if (mode & S_IRUSR) p[0] = 'r';
    if (mode & S_IWUSR) p[1] = 'w';
    if (mode & S_IXUSR) p[2] = 'x';

    if (mode & S_IRGRP) p[3] = 'r';
    if (mode & S_IWGRP) p[4] = 'w';
    if (mode & S_IXGRP) p[5] = 'x';

    if (mode & S_IROTH) p[6] = 'r';
    if (mode & S_IWOTH) p[7] = 'w';
    if (mode & S_IXOTH) p[8] = 'x';

    return p;
}

void print_row(const string& key, const string& value) {
    constexpr int KEY_W = 18;
    constexpr int VALUE_W = 30;

    cout << right << setw(KEY_W) << key << " "
         << left  << setw(VALUE_W) << value << '\n';
}


int main(int argc, char* argv[]) {

    if (argc < 2) {
        help();
        return 0;
    }

    struct stat st;
    if (stat(argv[1], &st) == 0) {

        char atime_str[26], mtime_str[26];
        strncpy(atime_str, ctime(&st.st_atime), 24);
        atime_str[24] = '\0';
        strncpy(mtime_str, ctime(&st.st_mtime), 24);
        mtime_str[24] = '\0';
        char btime_str[26];
        strncpy(btime_str, ctime(&st.st_birthtimespec.tv_sec), 24);
        btime_str[24] = '\0';

        string type;
        if      (S_ISREG(st.st_mode))  type = "file";
        else if (S_ISDIR(st.st_mode))  type = "directory";
        else if (S_ISLNK(st.st_mode))  type = "symlink";
        else if (S_ISCHR(st.st_mode))  type = "char device";
        else if (S_ISBLK(st.st_mode))  type = "block device";
        else if (S_ISFIFO(st.st_mode)) type = "fifo";
        else if (S_ISSOCK(st.st_mode)) type = "socket";

        passwd* pw = getpwuid(st.st_uid);
        string owner = (pw ? pw->pw_name : "unknown");
        group* gr = getgrgid(st.st_gid);
        string group = (gr ? gr->gr_name: "unknown");

        float size = static_cast<double>(st.st_size);
        string size_str;
        char str[20];

        if (size > 1024.0 * 1024.0 * 1024.0) {
            size /= 1024.0 * 1024.0 * 1024.0;
            snprintf(str, sizeof(str), "%.2f", size);
            size_str = string(str) + " (GB)";
        } else if (size > 1024.0 * 1024.0) {
            size /= 1024.0 * 1024.0;
            snprintf(str, sizeof(str), "%.2f", size);
            size_str = string(str) + " (MB)";
        } else if (size > 1024.0) {
            size /= 1024.0;
            snprintf(str, sizeof(str), "%.2f", size);
            size_str = string(str) + " (KB)";
        } else {
            snprintf(str, sizeof(str), "%.2f", size);
            size_str = string(str) + " (B)";
        }

        

        string filename = getcwd(nullptr, 0) + *argv[1];
        

        cout << "***** File Information *****" << endl;
        print_row("Filename:", filename);
        print_row("File type:", type);
        print_row("Owner:", owner);
        print_row("Group:", group);
        print_row("Size:",size_str);
        print_row("Blocks: ", to_string(st.st_blocks));
        print_row("Device:", to_string(st.st_dev));
        print_row("Permissions:" , perms_to_string(st.st_mode));
        print_row("Created at:" , btime_str);
        print_row("Last Accessed:", atime_str);
        print_row("Last Modified:", mtime_str);
    }
    return 0;
}