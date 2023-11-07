#include <boost/asio.hpp>
#include <array>
#include <iostream>
#include "async.h"

constexpr size_t BUFFER_SIZE = 1024;


class Session: public std::enable_shared_from_this<Session> {
public:

Session(size_t size, boost::asio::ip::tcp::socket&& s): socket_(std::move(s)), handle(async::connect(size)), buffer(ch_buffer, BUFFER_SIZE){}


void read() {
     auto pointer(shared_from_this()); 
     socket_.async_read_some(buffer,
     [pointer, this] (boost::system::error_code ec, size_t length) {
          if(!ec) {
               async::receive(handle, ch_buffer, length);
               this->read();
          } else {
               this->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
               this->socket_.close();
               async::disconnect(handle);
          }
     }
     );
}

boost::asio::ip::tcp::socket socket_;
async::handle_t handle;
char ch_buffer[BUFFER_SIZE];
boost::asio::mutable_buffer buffer;

};

class Server {
public:
     Server(unsigned short port, size_t s):
     context(),
     acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
     size(s) {
          accept();
          context.run();
     }

private:

     void accept() {
          auto socket = std::make_shared<boost::asio::ip::tcp::socket>(context);
          acceptor.async_accept(*socket,
          [this, socket](boost::system::error_code ec) {
               if(!ec) {
                    std::make_shared<Session>(size, (std::move(*socket)))->read();
               }
               this->accept();
          });
     }
     boost::asio::io_context context;
     boost::asio::ip::tcp::acceptor acceptor;
     size_t size;
};


int main(int argc, char** argv) {
     (void)argv;
     if(argc != 3) {
          std::cout << "Requires port and static block size" << std::endl;
          return 1;
     }
     try {
          Server server(std::atoi(argv[1]), std::atoi(argv[2]));
          
     } catch (std::exception& e) {
          std::cout << e.what();
     }
}