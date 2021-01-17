#include <iostream>
#include <filesystem>
#include <sstream>

using namespace std;
using namespace std::filesystem;

struct Attributes {
    Attributes& operator+=(const Attributes& other) {
        this->size_bytes += other.size_bytes;
        this->n_directories += other.n_directories;
        this->n_files += other.n_files;
        return *this;
    }
    size_t size_bytes;
    size_t n_directories;
    size_t n_files;
};

//https://en.cppreference.com/w/cpp/filesystem/perms
std::string demo_perms(std::filesystem::perms p)
{
    std::stringstream ss;
    ss << ((p & std::filesystem::perms::owner_read) != std::filesystem::perms::none ? "r" : "-")
       << ((p & std::filesystem::perms::owner_write) != std::filesystem::perms::none ? "w" : "-")
       << ((p & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ? "x" : "-")
       << ((p & std::filesystem::perms::group_read) != std::filesystem::perms::none ? "r" : "-")
       << ((p & std::filesystem::perms::group_write) != std::filesystem::perms::none ? "w" : "-")
       << ((p & std::filesystem::perms::group_exec) != std::filesystem::perms::none ? "x" : "-")
       << ((p & std::filesystem::perms::others_read) != std::filesystem::perms::none ? "r" : "-")
       << ((p & std::filesystem::perms::others_write) != std::filesystem::perms::none ? "w" : "-")
       << ((p & std::filesystem::perms::others_exec) != std::filesystem::perms::none ? "x" : "-")
       << " ";
    return ss.str();
}

void print_line_with_perm(const Attributes& attributes, string_view path) {
    cout << setw(8) << demo_perms(status(path).permissions())
         << setw(14) << attributes.size_bytes
         << setw(7) << attributes.n_files
         << setw(7) << attributes.n_directories
         << " " << path << "\n";
}

void print_line(const Attributes& attributes, string_view path) {
    cout << setw(14) << attributes.size_bytes
         << setw(7) << attributes.n_files
         << setw(7) << attributes.n_directories
         << " " << path << "\n";
}

Attributes explore(const directory_entry& directory) {
    Attributes attributes{};
    for(const auto& entry : recursive_directory_iterator{ directory.path() }) {
        if (entry.is_directory()) {
            attributes.n_directories++;
        } else {
            attributes.n_files++;
            attributes.size_bytes += entry.file_size();
        }
    }
    return attributes;
}

class Parameters {
public:
    bool recursive = false;
    bool permissions = false;
};

Parameters parse_arguments(const std::string& parameters){
    Parameters para{};
    for (auto& ch : parameters) {
        if (ch == 'R') {
            para.recursive = true;
        } else if (ch == 'P') {
            para.permissions = true;
        }
    }
    return para;
}

int main(int argc, const char** argv) {
    if (argc == 1) {
        throw std::logic_error {"Not enough arguments"};
    }
    const std::string first_arg {argv[1]};
    Parameters parameters{};
    path sys_path;
    if (first_arg.starts_with('-')) {
        if (argc != 3) {
            throw std::logic_error {"Wrong number of arguments"};
        }
        parameters = parse_arguments(first_arg);
        sys_path = path{ argv[2] };
    } else {
        if (argc != 2) {
            throw std::logic_error {"Wrong number of arguments"};
        }
        sys_path = path{ argv[1] };
    }
    if (parameters.permissions) {
        cout << "Perm      Size           Files  Dirs   Name\n";
        cout << "--------- -------------- ------ ------ ------------\n";
    } else {
        cout << "Size           Files  Dirs   Name\n";
        cout << "-------------- ------ ------ ------------\n";
    }
    Attributes root_attributes{};
    for (const auto& entry : directory_iterator{ sys_path }) {
        try {
            if (entry.is_directory()) {
                if (parameters.recursive) {
                    const auto attributes = explore(entry);
                    root_attributes += attributes;
                    if (parameters.permissions) {
                        print_line_with_perm(root_attributes, sys_path.filename().string());
                    } else {
                        print_line(root_attributes, sys_path.filename().string());
                    }
                }
                root_attributes.n_directories++;
            } else {
                root_attributes.n_files++;
                error_code ec;
                root_attributes.size_bytes += entry.file_size(ec);
                if (ec) cerr << "Error reading file size: "
                             << entry.path().string() << endl;
            }
        } catch(const exception&) {
        }
    }
    if (parameters.permissions) {
        print_line_with_perm(root_attributes, sys_path.filename().string());
    } else {
        print_line(root_attributes, sys_path.filename().string());
    }
}