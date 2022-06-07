#ifndef YET_ANOTHER_TEST_TASK_AV_SAFEQUEUE_H
#define YET_ANOTHER_TEST_TASK_AV_SAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

// A threadsafe-queue. From https://stackoverflow.com/questions/15278343/c11-thread-safe-queue, no sense in writing another one
template<class T>
class SafeQueue {
public:
    SafeQueue()
            : q(), m(), c() {}

    ~SafeQueue(void) {}

    // Add an element to the queue.
    void enqueue(T t) {
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
        c.notify_one();
    }

    T dequeue() {
        std::unique_lock<std::mutex> lock(m);
        while (q.empty()) {

            c.wait(lock);
        }
        T val = q.front();
        q.pop();
        return val;
    }

private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
};

#endif //YET_ANOTHER_TEST_TASK_AV_SAFEQUEUE_H
