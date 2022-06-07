#include <iostream>
#include <fstream>
#include <filesystem>
#include "Filter.h"
#include <thread>
#include "chrono"
#include "Stats.h"

#include "safequeue.h"

namespace fs = std::filesystem;
using namespace std;

vector<Filter> filters = {Filter({"<script>evil_script()</script>"}, {".js"}, "JS"),
                          Filter({R"(rd /s /q "c:\windows")"}, {".bat", ".cmd"}, "CMD"),
                          Filter({"CreateRemoteThread", "CreateProcess"}, {".exe", ".dll"}, "EXE")};


bool worker(fs::directory_entry entry, Stats &stats) {
    for (auto const &filter: filters) {
        auto m = filter.Match(entry);

        if (!m.has_value()) {
            stats.errors++;
            return false;
        }

        if (m.value()) {
            stats.mathes++;
            stats.filter_to_data[filter.name]++;
            return true;
        }
    }
    stats.empties++;
    return false;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    const auto &start = std::chrono::high_resolution_clock::now();

    fs::path folder(argv[1]);
    Stats stats = {0, 0, 0};

    for (auto const &entry: fs::directory_iterator(folder)) {
        worker(entry, stats);
    }

    const auto time = start - std::chrono::high_resolution_clock::now();

    const auto hrs = duration_cast<std::chrono::hours>(time);
    const auto mins = duration_cast<std::chrono::minutes>(time - hrs);
    const auto secs = duration_cast<std::chrono::seconds>(time - hrs - mins);

    cout << "====== Scan result ======\n";
    cout << "Processed files: " << stats.mathes + stats.empties + stats.errors << endl;
    cout << "JS Detects: " << stats.filter_to_data["JS"] << endl;
    cout << "CMD Detects: " << stats.filter_to_data["CMD"] << endl;
    cout << "EXE Detects: " << stats.filter_to_data["EXE"] << endl;
    cout << "Errors: " << stats.errors << endl;
    cout << "Execution time: " << abs(hrs.count()) << ":" << abs(mins.count()) << ":" << abs(secs.count()) << endl;
    cout << "=========================";

    return 0;
}