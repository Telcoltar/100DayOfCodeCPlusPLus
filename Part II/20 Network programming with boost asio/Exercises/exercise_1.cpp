#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/algorithm/string/case_conv.hpp>
using namespace boost::asio;

static int ind{};

struct Session {
    explicit Session(io_context& io_context, int index) :
    socket_{io_context, ip::udp::endpoint(ip::udp::v4(), 1895) }, index{index} {
        std::cout << "Start Session " << index << "\n";
        read();
    }
    void read() {
        socket_.async_receive_from(buffer(recv_buffer_),remote_endpoint_,
                                   [this](boost::system::error_code ec, std::size_t length) {
                                       if (ec)
                                       {
                                           return;
                                       }
                                       this->message.assign(recv_buffer_.begin(),recv_buffer_.begin()+length);
                                       std::cout << this->message;
                                       if (!(this->message == "exit\n")) {
                                           boost::algorithm::to_upper(message);
                                           this->write();
                                       }
                                   }
        );
    }

    void write() {
        socket_.async_send_to(buffer(message), remote_endpoint_,
                              [this](boost::system::error_code ec, std::size_t length) {
                                  if (ec) return;
                                  this->message.clear();
                                  this->read();
                              }
        );
    }

    ~Session() {
        std::cout << "Destroy Session " << index << "\n";
    }
private:
    ip::udp::socket socket_;
    ip::udp::endpoint remote_endpoint_;
    std::string message;
    std::array<char, 1500> recv_buffer_;
    int index;
};

int main()  {
    io_context io_context;
    Session sess {io_context, 0};
    io_context.run();
}