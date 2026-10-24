#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <queue>
#include <vector>
#include <iostream>
#include <stdexcept>


class ThreadPool {
public:
    explicit ThreadPool(std::size_t num_threads);

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    void Submit(std::function<void()> task);
    ~ThreadPool();

private:
    struct Action {
        std::function<void()> task;
        bool stop{false};
    };

    Action WaitForAction();
    void Worker();
    void StopAndNotifyAll();
    void JoinAllThreads();

    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stopping_{false};
    std::vector<std::thread> threads_;
};
