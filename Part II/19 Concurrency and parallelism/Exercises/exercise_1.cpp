#include <mutex>
#include <future>
#include <iostream>
#include <thread>

class SpinLock {
    std::mutex m;


public:
    void lock() {
        while (!m.try_lock()) {
            std::this_thread::sleep_for(std::chrono::microseconds (10));
        }
    }

    bool try_unlock() {
        return m.try_lock();
    }

    void unlock() {
        m.unlock();
    }
};

void count() {
    int counter{};
    int iterations{1'000'000};
    SpinLock spinlock_mutex;

    auto count_up = std::async(std::launch::async,
                               [&] {
                                   for (int j{}; j < iterations; j++) {
                                       std::lock_guard<SpinLock> spinlock_guard{spinlock_mutex};
                                       counter++;
                                   }
                               });

    auto count_down = std::async(std::launch::async,
                                 [&] {
                                     for (int j{}; j < iterations; j++) {
                                         std::lock_guard<SpinLock> spinlock_guard{spinlock_mutex};
                                         counter--;
                                     }
                                 });
    std::cout << "Counter: " << counter << "\n";
    count_up.get();
    std::cout << "Counter: " << counter << "\n";
    count_down.get();
    std::cout << "Counter: " << counter << "\n";
}

int main() {
    count();
    std::cout << "Next" << "\n";
    count();
    std::cout << "Next" << "\n";
    count();
}
