#include <random>
#include <iostream>

std::string gen_function(std::uniform_int_distribution<int> dist, int len) {
    std::random_device engine;
    std::string pass{};
    for (int i{}; i < len; i++) {
        pass += char{static_cast<char>(dist(engine) + '!')};
    }
    return pass;
}

int main() {
    std::uniform_int_distribution<int> uni{ 0, 93 };
    for (int i{}; i < 10; i++) {
        std::cout << gen_function(uni, 12) << "\n";
    }
}
