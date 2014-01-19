#ifndef ETHSCOPE_H
#define ETHSCOPE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "TcpSocket.h"

class EthScope {
    public:
        EthScope(std::string ip, int port = 1861);
        ~EthScope();

        void beep();
        void idn();
        void configure(std::fstream file);
        
        void write(std::string str);
        std::string read();

    private:
        TcpSocket socket;
        std::vector<char> header;
        
        void prepHeader(std::string str);
};

#endif
