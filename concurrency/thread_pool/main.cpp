#include <iostream>
#include <mutex>
#include "thread_pool.hpp"


int main() {
    std::vector<int> counts(1000, 0);
    std::mutex mutex;

    {
        ThreadPool pool(3);
        for (std::size_t i = 0; i < counts.size(); ++i) {
            pool.Submit([i, &counts, &mutex] {
                std::lock_guard<std::mutex> lock(mutex);
                counts[i]++;
            });
        }
    }

    // Check
    for (std::size_t i = 0; i < counts.size(); ++i) {
        if (counts[i] != 1) {
            std::cout << "Error: counts[" << i << "] = " << counts[i] << std::endl;
            return 1;
        }
    }
    std::cout << "Success!" << std::endl;

    return 0;
}
