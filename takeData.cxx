#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>

#include "EthScope.h"

int main(int argc, char *argv[]) {
    if(argc != 3) {
        std::cout << "Execute as: ./takeData <ip> <outfile_name>" << std::endl;
        return 1;
    }
    std::string ip = argv[1];
    std::string filename = argv[2];
    std::string channel = "C2";
    
    std::cout << "###################" << std::endl;
    std::cout << "Filename: " << filename << std::endl;
    std::cout << "IP: " << ip << std::endl;
    std::cout << "###################" << std::endl;

    std::cout << "Connecting to Scope ... " << std::endl;
    EthScope scope(ip);
    std::cout << " ... beep ... " << std::endl;
    scope.beep();
    std::cout << " ... reading waveform from channel: " << channel << std::endl;
    scope.readWave(channel);
    std::cout << " ... saving waveform to file ... " << std::endl;
    scope.saveLastWaveToFile(filename);
    std::cout << " ... done!" << std::endl;

    return 0;
}

