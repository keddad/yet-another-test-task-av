#include <map>

#ifndef YET_ANOTHER_TEST_TASK_AV_STATS_H
#define YET_ANOTHER_TEST_TASK_AV_STATS_H

struct Stats {
    int mathes = 0;
    int empties = 0;
    int errors = 0;
    std::map<std::string, int> filter_to_data;
};

#endif //YET_ANOTHER_TEST_TASK_AV_STATS_H
