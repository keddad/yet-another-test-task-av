#include <iostream>
#include <fstream>
#include <filesystem>
#include "Filter.h"

#include "safequeue.h"

namespace fs = std::filesystem;
using namespace std;

const vector<Filter> filters = {Filter({"<script>evil_script()</script>"}, {".js"}),
                                Filter({R"(rd /s /q "c:\windows")"}, {".bat", ".cmd"}),
                                Filter({"CreateRemoteThread", "CreateProcess"}, {".exe", ".dll"})};


void worker() {

}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    fs::path folder(argv[1]);

    for (auto const &entry: fs::directory_iterator(folder)) {
        cout << entry.path() << "\n";
    }

    return 0;
}