#include "EthScope.h"

#define DEBUG

#define LECROY_EOI_FLAG				0x01
#define LECROY_SRQ_FLAG				0x08
#define LECROY_CLEAR_FLAG			0x10
#define LECROY_LOCKOUT_FLAG			0x20
#define LECROY_REMOTE_FLAG			0x40
#define LECROY_DATA_FLAG			0x80

EthScope::EthScope(std::string ip, int port) : socket(ip, port){
    header.push_back(LECROY_DATA_FLAG | LECROY_REMOTE_FLAG | LECROY_EOI_FLAG); // Operation
    header.push_back(0x1); // Header Version
    header.push_back(0x0); // Sequence number 1-255
    header.push_back(0x0); // Spare
    header.push_back(0x0); // Block length LSB
    header.push_back(0x0); // Block length
    header.push_back(0x0); // Block length
    header.push_back(0x0); // Block length MSB
}

EthScope::~EthScope() {

}

void EthScope::prepHeader(std::string str) {
    int size = str.size();
    header[2]++; // Increment running number
    if (header[2] == 0) header[2]++; // Skip 0
    // Size of string in bytes
    header[7] = (size & 0xFF) >> 0;
    header[6] = (size & 0xFF00) >> 8;
    header[5] = (size & 0xFF0000) >> 16;
    header[4] = (size & 0xFF000000) >> 24;
}

void EthScope::write(std::string str) {
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << " : Writing \"" << str << "\"" << std::endl;
#endif
    str += "\n";
    this->prepHeader(str);
    std::vector<char> msg(header.begin(), header.end());
    msg.insert(msg.end(), str.begin(), str.end());
    socket.write(msg);
}

std::string EthScope::read() {
    std::vector<char> msg = socket.read();
    if (msg.size() == 0) return "";
    std::string str(msg.begin(), msg.end());
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << " : Read \"" << str << "\"" << std::endl;
#endif
    return str;
}

void EthScope::beep() {
    std::string str = "buzz beep";
    this->write(str);
}

void EthScope::idn() {
    std::string str = "*idn?";
    this->write(str);
}

void EthScope::configure(std::fstream file) {
    std::string tmp;
    while(file) {
        std::getline(file, tmp);
        this->write(tmp);
    }
}
