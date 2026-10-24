
#include <iostream>
#include <fmt/format.h>
#include <thread>
#include <cstdint>
#include <functional>
#include <vector>
#include <mutex>


void Worker(std::size_t id, const std::function<void()>& do_work) {
    std::cout << fmt::format("Worker {} started", id) << std::endl;
    do_work();
    std::cout << "Worker " << id << " finished" << std::endl;
}


void Start() {
    std::cout << "Start workers" << std::endl;
    const std::size_t thread_count = 8;
    std::mutex counter_mutex;
    std::uint64_t counter = 0;
    const std::size_t increments_per_thread = 1'000'000;

    auto do_work = [&counter, &counter_mutex]() {
        for (std::size_t i = 0; i != increments_per_thread; ++i) {
            std::lock_guard<std::mutex> lock(counter_mutex);
            ++counter;
        }
    };

    std::vector<std::thread> threads;
    threads.reserve(thread_count);
    for (std::size_t i = 0; i != thread_count; ++i) {
        threads.emplace_back(Worker, i, do_work);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    const auto expected = thread_count * increments_per_thread;
    std::cout << "Expected: " << expected << std::endl;
    std::cout << "Actual: " << counter << std::endl;
}


int main() {
    Start();
    return 0;
}
