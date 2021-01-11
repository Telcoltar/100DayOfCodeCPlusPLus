#include <vector>
#include <iostream>
#include "FibonacciGen.cpp"

int main() {
    std::vector<unsigned long> vec;
    std::cout << vec.capacity() << "\n";
    vec.reserve(10);
    FibonacciGen gen;
    for (int i{}; i < 20; i++) {
        vec.emplace_back(gen.next());
    }
    std::cout << vec.capacity() << "\n";
    for (auto el : vec) {
        std::cout << el << ", ";
    }
}