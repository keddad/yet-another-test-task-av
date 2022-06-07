#include <filesystem>

#ifndef YET_ANOTHER_TEST_TASK_AV_THREADUTIL_H
#define YET_ANOTHER_TEST_TASK_AV_THREADUTIL_H

struct Job {
    std::optional<std::filesystem::directory_entry> candidate;
};

#endif //YET_ANOTHER_TEST_TASK_AV_THREADUTIL_H
