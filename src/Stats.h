#include <map>
#include <mutex>

#ifndef YET_ANOTHER_TEST_TASK_AV_STATS_H
#define YET_ANOTHER_TEST_TASK_AV_STATS_H

struct Stats {
    Stats() : mathes(0), total(0), errors(0) {};

    int mathes = 0;
    int total = 0;
    int errors = 0;
    std::map<std::string, int> filter_to_data;
    std::mutex m;
};

#endif //YET_ANOTHER_TEST_TASK_AV_STATS_H
