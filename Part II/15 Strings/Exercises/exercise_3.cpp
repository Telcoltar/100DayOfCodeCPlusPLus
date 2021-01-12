#include <iostream>
#include <unordered_set>


int main(int argc, char** argv) {
    std::string input{};
    std::unordered_set vowels{'a', 'e', 'i', 'o', 'u'};
    for (int j{1}; j < argc; j++ ) {
        input += argv[j];
    }
    int count{};
    for (auto ch : input) {
        count += vowels.contains(ch);
    }
    std::cout << count;
}

