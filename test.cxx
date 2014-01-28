#include <iostream>
#include "EthScope.h"

int main(int argc, char* argv[]) {
    std::cout << "Hello World" << std::endl;
    EthScope scope("172.25.1.26");
    scope.beep();
    scope.write("*idn?");
    std::string msg = scope.read();
    std::cout << msg;
    scope.readWave();
    return 0;
}
