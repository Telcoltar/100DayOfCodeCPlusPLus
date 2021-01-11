#include <iostream>
#include <vector>
#include <set>
#include <locale>

bool comp_c_style_string(const char *obj1, const char *obj2) {
    char obj1_char{};
    char obj2_char{};
    size_t current_index{};
    while (true) {
        obj1_char = std::tolower(obj1[current_index], std::locale());
        obj2_char = std::tolower(obj2[current_index], std::locale());
        if (obj1_char < obj2_char) {
            return true;
        } else if (obj1_char > obj2_char) {
            return false;
        }
        current_index++;
    }
}

int main(int argc, char **argv) {
    std::set<char*, decltype(comp_c_style_string)*> words(argv, argv + argc, comp_c_style_string);
    for (auto el : words) {
        std::cout << el << "\n";
    }
}

