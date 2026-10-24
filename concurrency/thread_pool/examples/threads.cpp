
#include <iostream>
#include <chrono>
#include <fmt/format.h>
#include <thread>


void Worker(std::size_t id, std::chrono::milliseconds duration) {
    std::cout << fmt::format(
        "Worker {} started for duration: {} ms",
        id,
        duration.count()
    ) << std::endl;
    std::this_thread::sleep_for(duration);
    std::cout << "Worker " << id << " finished" << std::endl;
}


void Start() {
    std::cout << "Start workers" << std::endl;
    std::thread t1(Worker, 1, std::chrono::milliseconds(300));
    std::thread t2(Worker, 2, std::chrono::milliseconds(100));
    std::thread t3(Worker, 3, std::chrono::milliseconds(200));


    t1.join();
    t2.join();
    t3.join();
    std::cout << "All workers finished" << std::endl;
}


int main() {
    Start();
    return 0;
}
