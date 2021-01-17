#include <iostream>
#include <filesystem>

int main(int argc, char** argv) {
    if (argc != 3) {
        throw std::logic_error{"Provide a path and extension."};
    }
    const std::filesystem::path path {argv[1]};
    const std::string ext {argv[2]};
    for (const auto& file : std::filesystem::recursive_directory_iterator(path)) {
        if (file.path().extension() == ext) {
            std::cout << file.path().filename().string() << "\n";
        }
    }
}
