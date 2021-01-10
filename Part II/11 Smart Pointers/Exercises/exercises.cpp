#include <cstdio>
#include <memory>
#include <utility>
#include <iostream>

using FileGuard = std::shared_ptr<FILE>;

struct Hal {
    Hal(FileGuard file, std::string identifier) : file{std::move(file)}, identifier{std::move(identifier)} {
        fprintf(this->file.get(), "Hello (%s)\n", this->identifier.c_str());
    }
    ~Hal() {
        fprintf(file.get(), "Stop Dave (%s)\n", identifier.c_str());
    }
    void write_status() {
        fprintf(file.get(), "I'm completely operational (%s)\n", identifier.c_str());
    }
    FileGuard file;
    std::string identifier;
};

void say_hello(const FileGuard& file) {
    fprintf(file.get(), "HELLO DAVE\n");
}

int main() {
    auto file = fopen("HAL9000", "w");
    if (!file) return errno;
    FileGuard file_guard{ file, fclose };
    Hal halter{file_guard, "Halter 1"};
    Hal halter_2{file_guard, "Halter 2"};
    // File open here
    say_hello(file_guard);
    halter.write_status();
    halter_2.write_status();
    // File closed here
    say_hello(file_guard);
    return 0;
}