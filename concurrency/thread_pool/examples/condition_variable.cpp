#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

void Worker(std::mutex& mutex, std::condition_variable& cv, bool& ready) {
    std::cout << "Worker started to wait\n";
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [&] { return ready; });
    }
    std::cout << "Worker starts work\n";
}

int main() {
    std::condition_variable cv;
    bool ready{false};
    std::mutex mutex;

    std::thread worker(Worker, std::ref(mutex), std::ref(cv), std::ref(ready));
    std::this_thread::sleep_for(std::chrono::seconds(1));

    {
        std::unique_lock<std::mutex> lock(mutex);
        ready = true;
        cv.notify_one();
    }

    worker.join();
    std::cout << "All workers finished\n";
    return 0;
}
