#include <chrono>
#include <random>
#include <forward_list>
#include <iostream>

struct Stopwatch {
    explicit Stopwatch(std::chrono::nanoseconds& result)
            : result{ result },
              start{ std::chrono::system_clock::now() } { }
    ~Stopwatch() {
        result = std::chrono::system_clock::now() - start;
    }
private:
    std::chrono::nanoseconds& result;
    const std::chrono::time_point<std::chrono::system_clock> start;
};

long random() {
    static std::mt19937_64 mt_engine{ 102787 };
    static std::uniform_int_distribution<long> int_d{ 0, 2000 };
    return int_d(mt_engine);
}

int main() {
    size_t samples{ 1'000'000 };
    size_t size_containers{10'000'000};
    size_t  step{10};
    std::chrono::nanoseconds elapsed;
    std::vector<long> vec(size_containers);
    std::forward_list<long> list(size_containers);
    {
        Stopwatch stopwatch(elapsed);
        std::generate(vec.begin(), vec.end(), random);
        std::generate(list.begin(), list.end(), random);
    }
    std::cout << elapsed.count() / 1'000'000 << "ms" << "\n";
    for (size_t j{1}; j < 11; j++) {
        step = j;
        {
            Stopwatch stopwatch(elapsed);
            auto itr = vec.begin();
            for (size_t i{}; i < size_containers / step; i++) {
                std::advance(itr, step);
            }
        }
        std::cout << elapsed.count() / 1'000'000 << "ms" << ", ";
    }
    std::cout << "\n";
    for (size_t j{1}; j < 11; j++) {
        step = j;
        {
            Stopwatch stopwatch(elapsed);
            auto itr = list.begin();
            for (size_t i{}; i < size_containers / step; i++) {
                std::advance(itr, step);
            }
        }
        std::cout << elapsed.count() / 1'000'000 << "ms" << ", ";
    }
}