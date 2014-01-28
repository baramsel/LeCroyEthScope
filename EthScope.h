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
        
        void configure(std::fstream file);
        void readWave();
        void beep();
        void idn();

        void setOutfile(std::string arg_name);
        
        void write(std::string str);
        std::string read();

    private:
        TcpSocket socket;
        std::vector<char> header;
        std::string outfileName;
        
        void prepHeader(std::string str);
        unsigned checkHeader(char *arg_header);
};

#endif
