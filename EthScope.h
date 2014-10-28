#ifndef ETHSCOPE_H
#define ETHSCOPE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>

#include <boost/lexical_cast.hpp>

#include "TcpSocket.h"
#include "WaveContainer.h"

class EthScope {
    public:
        EthScope(std::string ip, int port = 1861);
        ~EthScope();
        
        void beep();
        std::string idn();
        
        void configure(std::fstream file);
        void readWave(std::string channel);

        void saveLastWaveToFile(std::string filename);
        void setOutfile(std::string arg_name);
        void setWriteRawFile(bool arg);
        
        void write(std::string str);
        std::string read();

    private:
        TcpSocket socket;
        std::vector<char> header;
        bool writeRawFile;
        std::string outfileName;

        void convertWaveform(std::vector<char> desc, std::vector<char> data);
        double timeScale;
        double voltageScale;

        WaveContainer waveData;

        void prepHeader(std::string str);
        unsigned checkHeader(char *arg_header);
        template <typename T> T swap_endian(T u);
};

#endif
