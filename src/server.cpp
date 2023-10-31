#include <boost/asio.hpp>
#include <array>
#include <iostream>
#include <mutex>
#include "command_interpreter.hpp"

constexpr size_t BUFFER_SIZE = 1024;


class Session: public std::enable_shared_from_this<Session> {
public:

    Session(boost::asio::ip::tcp::socket&& s): socket_(std::move(s)){}
    std::mutex operation_mutex;

    void read() {
        auto pointer(shared_from_this()); 
        socket_.async_read_some(boost::asio::buffer(ch_buffer, BUFFER_SIZE),
        [pointer, this] (boost::system::error_code ec, size_t length) {
            if(!ec) {
                std::string str(ch_buffer, length);
                str = str.substr(0, str.size() - 1);
                std::unique_lock lock(operation_mutex);
                std::string out = interpret(str);
                out += "\n";
                this->write(out);
            } else {
                this->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
                this->socket_.close();
            }
        }
        );
    }

    void write(std::string& s) {
        auto pointer(shared_from_this());
        std::memcpy(ch_buffer, s.c_str(), std::min(s.size(), BUFFER_SIZE));
        socket_.async_write_some(boost::asio::buffer(ch_buffer, std::min(s.size(), BUFFER_SIZE)), 
        [pointer, this, str = s] (boost::system::error_code ec, size_t length) {
            if(!ec) {
                if(str.size() > length) {
                    auto new_s = str.substr(length, str.size() - length);
                    this->write(new_s);
                }
                this->read();
            } else {
                this->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
                this->socket_.close();
            }
        }
        );
}

boost::asio::ip::tcp::socket socket_;
char ch_buffer[BUFFER_SIZE];

};

class Server {
public:
     Server(unsigned short port):
     context(),
     acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
        accept();
        context.run();
     }

private:

     void accept() {
          auto socket = std::make_shared<boost::asio::ip::tcp::socket>(context);
          acceptor.async_accept(*socket,
          [this, socket](boost::system::error_code ec) {
               if(!ec) {
                    std::make_shared<Session>((std::move(*socket)))->read();
               }
               this->accept();
          });
     }
     boost::asio::io_context context;
     boost::asio::ip::tcp::acceptor acceptor;
     size_t size;
};


int main(int argc, char** argv) {
     (void)argc;
     (void)argv;
     //if(argc != 2) {
     //     return 1;
     //}
     try {
          Server server(9002);
          
     } catch (std::exception& e) {
          std::cout << e.what();
     }
}

