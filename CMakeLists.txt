cmake_minimum_required(VERSION 3.22)
project(yet_another_test_task_av)

set(CMAKE_CXX_STANDARD 20)

add_executable(yet_another_test_task_av
        src/main.cpp
        src/safequeue.h src/Filter.h)
