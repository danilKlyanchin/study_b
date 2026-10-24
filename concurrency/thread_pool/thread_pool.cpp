#include "thread_pool.hpp"

ThreadPool::ThreadPool(std::size_t num_threads) {
    if (num_threads == 0) {
        throw std::invalid_argument("ThreadPool: num_threads must be greater than 0");
    }

    try {
        threads_.reserve(num_threads);
        for (std::size_t i = 0; i < num_threads; ++i) {
            threads_.emplace_back(&ThreadPool::Worker, this);
        }
    } catch (...) {
        StopAndNotifyAll();
        JoinAllThreads();
        throw;
    }
}

void ThreadPool::Submit(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.push(task);
    }
    cv_.notify_one();
}


ThreadPool::Action ThreadPool::WaitForAction() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [&] {
        return !tasks_.empty() || stopping_;
    });

    if (stopping_ && tasks_.empty()) {
        return {.stop = true};
    }

    auto task = tasks_.front();
    tasks_.pop();
    return {.task = std::move(task)};
}

void ThreadPool::Worker() {
    while (true) {
        auto action = WaitForAction();
        if (action.stop) {
            break;
        }
        action.task();
    }
}

void ThreadPool::StopAndNotifyAll() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stopping_ = true;
    }
    cv_.notify_all();
}

void ThreadPool::JoinAllThreads() {
    for (auto& thread : threads_) {
        thread.join();
    }
}

ThreadPool::~ThreadPool() {
    StopAndNotifyAll();
    JoinAllThreads();
}
