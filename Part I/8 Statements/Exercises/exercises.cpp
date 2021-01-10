#include "fibonacci.h"
#include <cstdio>
#include <iostream>
#include "primegenerator.h"

int main() {
    for (const auto i : FibonacciRange{ 5000 }) {
        printf("%d ", i);
    }
    std::cout << "\n";
    for (const auto i : PrimeRange{ 1000 }) {
        std::cout <<  i << " ";
    }
}