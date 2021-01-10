

#include <stdexcept>
#include <iostream>

template <typename Fn, typename In>
constexpr bool all(Fn function, In* input, size_t length) {
    for (size_t i {}; i < length; i++) {
        if (!function(input[i])) {
            return false;
        }
    }
    return true;
}

int main() {
    int data[]{ 100, 200, 300, 400, 500 };
    size_t data_len = 5;
    auto all_gt50 = all([](auto x) { return x > 50; }, data, data_len);
    std::cout << "All elements are greater than 50: " << all_gt50;
    try {
        throw std::runtime_error{"Hello"};
    }
    catch (...) {
        std::cerr << "catch" << std::endl;
    }
}
