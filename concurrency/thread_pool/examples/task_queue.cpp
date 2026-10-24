#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <queue>

void Worker(
    std::mutex& mutex,
    std::condition_variable& cv,
    std::queue<int>& tasks,
    bool& stopping
)
{
    std::cout << "Worker started to wait\n";
    while (true) {
        int task;
        {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [&] {
                return !tasks.empty() || stopping;
            });

            if (stopping && tasks.empty()) {
                break;
            }

            task = tasks.front();
            tasks.pop();
        }
        std::cout << "Worker got task " << task << std::endl;
    }

    std::cout << "Worker finished\n";
}

int main() {
    std::queue<int> q;
    std::condition_variable cv;
    std::mutex mutex;
    bool stopping{false};

    std::thread worker(Worker, std::ref(mutex), std::ref(cv), std::ref(q), std::ref(stopping));

    int num_tasks = 5;
    for (int i = 1; i < num_tasks + 1; ++i) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            q.push(i);
            std::cout << "Pushed " << i << std::endl;
        }
        cv.notify_one();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    {
        std::unique_lock<std::mutex> lock(mutex);
        stopping = true;
    }
    cv.notify_all();

    worker.join();
    std::cout << "All workers finished\n";
    return 0;
}
