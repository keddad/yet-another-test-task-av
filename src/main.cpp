#include <iostream>
#include <fstream>
#include <filesystem>
#include "Filter.h"
#include <thread>
#include "chrono"
#include "Stats.h"
#include "threadutil.h"

#include "safequeue.h"

namespace fs = std::filesystem;
using namespace std;

vector<Filter> filters = {Filter({"<script>evil_script()</script>"}, {".js"}, "JS"),
                          Filter({R"(rd /s /q "c:\windows")"}, {".bat", ".cmd"}, "CMD"),
                          Filter({"CreateRemoteThread", "CreateProcess"}, {".exe", ".dll"}, "EXE")};

const auto thread_count = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() * 2 : 2;

void worker(SafeQueue<Job> &q, Stats &stats) {
    while (true) {
        auto job = q.dequeue();
        if (!job.candidate.has_value()) {
            return;
        }

        for (auto const &filter: filters) {
            auto m = filter.Match(job.candidate.value());

            std::lock_guard<std::mutex> lock(stats.m);

            if (!m.has_value()) {
                stats.errors++;
                break;
            }

            if (m.value()) {
                stats.mathes++;
                stats.filter_to_data[filter.name]++;
                break;
            }
        }
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    const auto &start = std::chrono::high_resolution_clock::now();

    fs::path folder(argv[1]);
    Stats stats;

    SafeQueue<Job> q;
    vector<thread> threads;

    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back(worker, std::ref(q), std::ref(stats));
    }

    stats.m.lock();
    for (auto const &entry: fs::directory_iterator(folder)) {
        stats.total++;
        q.enqueue(Job{entry});
    }

    stats.m.unlock();

    for (int i = 0; i < thread_count; ++i) {
        q.enqueue(Job{std::nullopt});
    }


    for (int i = 0; i < thread_count; ++i) {
        threads[i].join();
    }

    const auto time = start - std::chrono::high_resolution_clock::now();

    const auto hrs = duration_cast<std::chrono::hours>(time);
    const auto mins = duration_cast<std::chrono::minutes>(time - hrs);
    const auto secs = duration_cast<std::chrono::seconds>(time - hrs - mins);

    cout << "====== Scan result ======\n";
    cout << "Processed files: " << stats.total << endl;
    cout << "JS Detects: " << stats.filter_to_data["JS"] << endl;
    cout << "CMD Detects: " << stats.filter_to_data["CMD"] << endl;
    cout << "EXE Detects: " << stats.filter_to_data["EXE"] << endl;
    cout << "Errors: " << stats.errors << endl;
    cout << "Execution time: " << abs(hrs.count()) << ":" << abs(mins.count()) << ":" << abs(secs.count()) << endl;
    cout << "=========================";

    return 0;
}