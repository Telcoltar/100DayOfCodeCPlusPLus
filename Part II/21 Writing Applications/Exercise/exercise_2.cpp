#include <boost/asio.hpp>
#include <iostream>
#include <istream>
#include <cstdio>
#include <ostream>
#include <string>
#include <boost/program_options.hpp>
#include <filesystem>
#include <fstream>

std::string request(const std::string& host, const std::string& target, boost::asio::io_context& io_context) {
    std::stringstream request_stream;
    request_stream << "GET " << target << " HTTP/1.1\r\n"
                      "Host: " << host << "\r\n"
                                          "Accept: text/html\r\n"
                                          "Accept-Language: en-us\r\n"
                                          "Accept-Encoding: identity\r\n"
                                          "Connection: close\r\n\r\n";
    const auto request = request_stream.str();
    boost::asio::ip::tcp::resolver resolver{ io_context };
    const auto endpoints = resolver.resolve(host, "http");
    boost::asio::ip::tcp::socket socket{ io_context };
    const auto connected_endpoint = boost::asio::connect(socket, endpoints);
    boost::asio::write(socket, boost::asio::buffer(request));
    std::string response;
    boost::system::error_code ec;
    boost::asio::read(socket, boost::asio::dynamic_buffer(response), ec);
    if (ec && ec.value() != 2) throw boost::system::system_error{ ec };
    return response;
}

int main(int argc, char** argv) {
    boost::asio::io_context io_context;
    using namespace boost::program_options;
    bool is_help{};
    options_description description{ "request [options] host target1, target2, ..."};
    description.add_options()
            ("help,h", bool_switch(&is_help), "display a help dialog")
            ("directory,d", value<std::string>(), "directory to save output")
            ("host", value<std::string>(), "host")
            ("targets", value<std::vector<std::string>>(), "targets");
    positional_options_description positional;
    positional.add("host", 1);
    positional.add("targets", -1);
    command_line_parser parser{ argc, argv };
    parser.options(description);
    parser.positional(positional);
    variables_map vm;
    try {
        auto parsed_result = parser.run();
        store(parsed_result, vm);
        notify(vm);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    }
    std::string host{};
    if (is_help) {
        std::cout << description;
        return -1;
    }
    if (vm["host"].empty()) {
        host = "www.example.com";
    } else {
        host = vm["host"].as<std::string>();
    }
    std::vector<std::string> targets;
    if (!vm["targets"].empty()) {
        targets = vm["targets"].as<std::vector<std::string>>();
    } else {
        targets = {""};
    }
    std::string directory;
    if (!vm["directory"].empty()) {
        directory = vm["directory"].as<std::string>();
    }
    for (auto& target : targets) {
        try  {
            const auto response = request(host, "/" + target, io_context);
            if (!directory.empty()) {
                std::string current_file {directory};
                current_file += "/";
                current_file += host;
                current_file += "%";
                current_file += target;
                current_file += ".txt";
                std::filesystem::path p {current_file};
                std::cout << p.filename() << "\n";
                std::cout << p.parent_path() << "\n";
                std::cout << p.root_directory() << "\n";
                std::cout << p.root_path() << "\n";
                std::cout << p.root_name() << "\n";
                std::ofstream file{ p, std::ios::out };
                file << response << std::endl;
            }
            // std::cout << response << "\n";
        } catch(boost::system::system_error& se) {
            std::cerr << "Error: " << se.what() << std::endl;
        }
    }

}