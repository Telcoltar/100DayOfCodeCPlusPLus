#include <iostream>
#include <unordered_map>
#include <iterator>

bool comp(std::pair<const int, int> a, std::pair<int, int> b) {
    return a.second > b.second;
}

int main(int argc, char** argv) {
    std::unordered_map<int, int> count_map{};
    int current_count{};
    std::string current_string{};
    for(size_t i{1}; i < argc; i++) {
        current_string = std::string{argv[i]};
        current_count = count_map[current_string.length()];
        count_map[static_cast<int>(current_string.length())] = current_count + 1;
    }
    auto max_other = std::max_element(count_map.cbegin(),count_map.cend(), comp);
    for (int i{}; i <= max_other->first; i++) {
        std::cout << i << ": " << std::string(count_map[i], '*') << "\n";
    }
}