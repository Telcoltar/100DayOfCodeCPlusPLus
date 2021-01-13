#include <string>
#include <iostream>
#include <locale>

std::string capatalize(std::string&& input) {
    for (auto& el : input) {
        el = std::toupper(el, std::locale());
    }
    return input;
}

int main() {
    std::string input;
    std::cin >> input;
    std::cout << capatalize(std::move(input));
}