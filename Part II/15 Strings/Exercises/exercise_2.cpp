#include <string>
#include <iostream>

bool checkPalindrome(std::string input)  {
    for (auto& ch : input) {
        ch = std::tolower(ch, std::locale());
    }
    std::string input_copy{input};
    std::reverse(input_copy.begin(), input_copy.end());
    return input == input_copy;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        throw std::logic_error("Give exactly one argument");
    }
    if (checkPalindrome(argv[1])) {
        std::cout << "palindrome";
    } else {
        std::cout << "no palindrome";
    }
}

