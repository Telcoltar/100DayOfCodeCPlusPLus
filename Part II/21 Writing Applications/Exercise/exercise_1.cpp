#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <memory>
#include <csignal>
#include <atomic>
#include <vector>
using namespace boost::asio;

static std::atomic_bool interrupted{};
static int ind{};
static ip::tcp::acceptor* acc;

void handler(int signal) {
    std::cout << "Handler invoked with signal " << signal << ".\n";
    acc->cancel();
    acc->close();
    interrupted = true;
}

struct Session : std::enable_shared_from_this<Session> {
    explicit Session(ip::tcp::socket socket, int index) : socket{ std::move(socket) }, index {index} {
        std::cout << "Session " << index << " started" << "\n";
    }
    void read() {
        async_read_until(socket, dynamic_buffer(message), '\n',
        [self=shared_from_this()] (boost::system::error_code ec,
                                   std::size_t length) {
            if (ec || self->message == "\n") return;
            boost::algorithm::to_upper(self->message);
            self->write();
        });
    }
    void write() {
        async_write(socket, buffer(message),
        [self=shared_from_this()] (boost::system::error_code ec,
                                   std::size_t length) {
            if (ec) return;
            self->message.clear();
            if (interrupted) {
                return;
            }
            self->read();
        });
    }

    ~Session() {
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        socket.close();
        std::cout << "Session " << index << " stopped" << "\n";
    }
private:
    ip::tcp::socket socket;
    std::string message;
    int index{};
};

void serve(ip::tcp::acceptor& acceptor) {
    if (interrupted) {
        return;
    }
    acceptor.async_accept([&acceptor](boost::system::error_code ec,
    ip::tcp::socket socket) {
        serve(acceptor);
        if (ec) return;
        auto session = std::make_shared<Session>(std::move(socket), ind);
        ind++;
        session->read();
    });
}

int main()  {
    std::signal(SIGINT, handler);
    interrupted = false;
    try {
        io_context io_context;
        ip::tcp::acceptor acceptor{ io_context,
                                    ip::tcp::endpoint(ip::tcp::v4(), 1895) };
        serve(acceptor);
        acc = &acceptor;
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}