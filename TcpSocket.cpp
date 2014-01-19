#include "TcpSocket.h"

TcpSocket::TcpSocket(std::string ip, int port) : resolver(io_service), query(ip, boost::lexical_cast<std::string>(port)), socket(io_service) {
    endpoint_iterator = resolver.resolve(query);

    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
    if (error)
      throw boost::system::system_error(error);
}

TcpSocket::~TcpSocket() {
    socket.close();
}

void TcpSocket::write(std::vector<char> msg) {
    socket.write_some(boost::asio::buffer(msg));
}

std::vector<char> TcpSocket::read(size_t size) {
    std::vector<char> msg(size);
    size_t real_size = socket.read_some(boost::asio::buffer(msg));
    msg.resize(real_size);
    return msg;
}
