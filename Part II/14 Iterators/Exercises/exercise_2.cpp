#include <array>
#include <iostream>


template <typename Iterator>
int sum(Iterator it) {
    int result{};
    for (auto el : it) {
        result += el;
    }
    return result;
}

int main() {
    std::array<int, 5> arr{1,5,7,9,5};
    std::cout << sum(arr);
}