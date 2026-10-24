#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

void WorkerA(std::mutex& first_mutex, std::mutex& second_mutex) {
    std::lock_guard<std::mutex> first_lock(first_mutex);
    std::cout << "Worker A acquired first_mutex" << std::endl;

    std::this_thread::sleep_for(100ms);

    std::cout << "Worker A is waiting for second_mutex" << std::endl;
    std::lock_guard<std::mutex> second_lock(second_mutex);
    std::cout << "Worker A acquired second_mutex" << std::endl;
}

void WorkerB(std::mutex& first_mutex, std::mutex& second_mutex) {
    // Both workers acquire the same mutex first, preventing circular waiting.
    std::lock_guard<std::mutex> first_lock(first_mutex);
    std::cout << "Worker B acquired first_mutex" << std::endl;

    std::this_thread::sleep_for(100ms);

    std::cout << "Worker B is waiting for second_mutex" << std::endl;
    std::lock_guard<std::mutex> second_lock(second_mutex);
    std::cout << "Worker B acquired second_mutex" << std::endl;
}

int main() {
    std::mutex first_mutex;
    std::mutex second_mutex;

    std::thread worker_a(WorkerA, std::ref(first_mutex), std::ref(second_mutex));
    std::thread worker_b(WorkerB, std::ref(first_mutex), std::ref(second_mutex));

    worker_a.join();
    worker_b.join();

    std::cout << "All workers finished\n";
    return 0;
}
