#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <iostream>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

using boost::asio::ip::tcp;

class TcpSocket {
    public:
        TcpSocket(std::string ip, int port);
        ~TcpSocket();

        void write(std::vector<char> msg);
        std::vector<char> read(size_t size=1024);

    private:
        boost::asio::io_service io_service;
        tcp::resolver resolver;
        tcp::resolver::query query;
        tcp::resolver::iterator endpoint_iterator;
        tcp::resolver::iterator end;
        tcp::socket socket;
};

#endif
