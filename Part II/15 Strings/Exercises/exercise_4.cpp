#include <stdexcept>
#include <string>
#include <regex>
#include <iostream>

std::function<int(int, int)> get_op(const std::string& op) {
    if (op == "+") return [](int x, int y){return x + y;};
    if (op == "-") return [](int x, int y){return x - y;};
    return [](int x, int y){return x + y;};
}

int simple_computation(std::string& inp) {
    static std::regex one_op(R"((\d+)([+]|-)(\d+))");
    std::smatch match;
    std::regex_search(inp, match, one_op);
    while (!match.empty()) {
        inp.replace(match.position(0), match.length(0),
                    std::to_string(get_op(match[2])(std::stoi(match[1]), std::stoi(match[3]))));
        std::regex_search(inp, match, one_op);
    }
    return std::stoi(inp);
}

int complex_computation(std::string& inp) {
    static std::regex get_brackets(R"(\(([\d+]+)\))");
    std::smatch match;
    std::regex_search(inp, match, get_brackets);
    std::string match_str{};
    while (!match.empty()) {
        match_str = match.str(1);
        inp.replace(match.position(1) - 1, match.length(1) + 2,
                    std::to_string(complex_computation(match_str)));
        std::regex_search(inp, match, get_brackets);
    }
    std::cout << inp << "\n";
    return simple_computation(inp);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        throw std::logic_error("Only one argument is allowed.");
    }
    std::string input{argv[1]};
    auto result{complex_computation(input)};
    std::cout << result;

}
