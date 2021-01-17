#include <mutex>
#include <queue>
#include <iostream>
#include <future>
#include <thread>

template <typename T>
class SafeQueue {
    std::queue<T> q;
    std::mutex m;
    std::condition_variable cv;

public:
    T front() {
        std::scoped_lock lock {m};
        return q.front();
    }

    void push(T arg) {
        std::scoped_lock lock {m};
        q.push(arg);
        cv.notify_one();
    }

    bool empty() {
        std::scoped_lock lock {m};
        return q.empty();
    }

    void pop() {
        std::scoped_lock lock {m};
        q.pop();
    }

    size_t size() {
        std::scoped_lock lock {m};
        return q.size();
    }

    T wait_and_pop() {
        std::unique_lock lock {m};
        if (q.empty()) {
            cv.wait((lock), [&] {return !q.empty();});
            std::cout << "wait" << "\n";
        }
        T tmp = q.front();
        q.pop();
        return tmp;
    }
};

int main() {
    SafeQueue<int> sq;
    int iterations{100};
    auto add_ones = std::async(std::launch::async, [&] {
        for (int j{}; j < iterations; j++) {
            std::this_thread::sleep_for(std::chrono::microseconds (10));
            sq.push(1);
        }
    });
    auto remove = std::async(std::launch::async, [&] {
        for (int j{}; j < iterations; j++) {
            std::cout << sq.wait_and_pop() << ", ";
        }
    });
    add_ones.get();
    remove.get();
}