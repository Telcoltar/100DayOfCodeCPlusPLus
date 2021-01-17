#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>

std::string request(const std::string& host, boost::asio::io_context& io_context) {

    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::beast::tcp_stream stream(io_context);

    auto const results = resolver.resolve(host, "80");

    stream.connect(results);

    beast::http::request<http::string_body> req{http::verb::get, "/", 11};
    req.set(http::field::host, host);

    beast::http::write(stream, req);

    beast::flat_buffer buffer;

    beast::http::response<http::string_body> res;

    beast::http::read(stream, buffer, res);

    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);

    if(ec && ec != beast::errc::not_connected)
        throw beast::system_error{ec};

    for (auto& el : res) {
        std::cout << el.name_string() << ": " << el.value() << "\n";
    }

/*    std::cout << res["Server"] << "\n";*/

    return std::string{res.reason()};

}

int main() {
    boost::asio::io_context io_context;
    try  {
        const auto response = request("www.google.de", io_context);
        std::cout << response << "\n";
    } catch(boost::system::system_error& se) {
        std::cerr << "Error: " << se.what() << std::endl;
    }
}
