#include <string>
#include <fstream>
#include <iostream>
#include <regex>

#include <Windows.h>

int main(int argc, char** argv) {
    std::string filepath = argv[1];
    std::wifstream file{filepath, std::ios::in};
    SetConsoleOutputCP( CP_UTF8 );
    std::wstring current{};
    static std::wregex regex{LR"([,.])"};
    std::wstring replace{};
    int count{};
    int char_count{};
    while (file >> current) {
        count++;
        current = std::regex_replace(current, regex, replace);
        char_count += current.length();
    }
    std::cout << "\n";
    double average{static_cast<double >(char_count) / count};
    std::cout << count << ", " << average;
}

