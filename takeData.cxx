#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>

#include <EthScope.h>

int main(int argc, char *argv[]) {
    if(argc != 3) {
        std::cout << "Execute as: ./takeData [dut-name] [bias]" << std::endl;
        return 1;
    }
    std::string dut = argv[1];
    int bias = boost::lexical_cast<int>(argv[2]);
    
    std::cout << "###################" << std::endl;
    std::cout << "DUT: " << dut << std::endl;
    std::cout << "Bias: " << bias << std::endl;
    std::cout << "###################" << std::endl;

    std::cout << "Connecting to Scope ... " << std::endl;
    EthScope scope("192.168.1.21");
    std::cout << " ... beep ... " << std::endl;
    scope.beep();
    std::string channel = "C2";
    std::cout << " ... reading waveform from channel: " << channel << std::endl;
    scope.readWave(channel);
    std::cout << " ... saving waveform to file ... " << std::endl;
    scope.saveLastWaveToFile("data", dut, bias);
    std::cout << " ... done!" << std::endl;

    return 0;
}

