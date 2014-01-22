#include <iostream>
#include <fstream>
#include <string>

#define HEX(x) std::hex << "0x" << static_cast<int>(x) << std::dec

unsigned checkHeader(char *header) {
    unsigned length = 0;
    length += header[7] << 0;
    length += header[6] << 8;
    length += header[5] << 16;
    length += header[4] << 24;
    std::cout << "Header data:" << std::endl;
    std::cout << "- Operation:       " << HEX(0xFF & header[0]) << std::endl;
    std::cout << "- Header Version:  " << HEX(0xFF & header[1]) << std::endl;
    std::cout << "- Sequence number: " << HEX(0xFF & header[2]) << std::endl;
    std::cout << "- Block Length:    " << HEX(length) << std::endl;
    return length;
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Specify file" << std::endl;
        return 0;
    }
    
    // Open file
    std::string filename = argv[1];
    std::cout << "Opening file: " << filename << std::endl;

    // Check file
    std::fstream file(filename.c_str(), std::ios::in | std::ios::binary);
    if (!file) {
        std::cout << "Error opening file ... aborting!" << std::endl;
        return 0;
    }
    
    // Check header information
    char header[8];
    file.read(header, 8);
    unsigned msg_size = checkHeader(header);

    std::string msg;
    msg.resize(msg_size, ' '); // reserve space
    char* begin = &*msg.begin();
    file.read(begin, msg_size);
    std::cout << "Message content: \"" << msg << "\"" << std::endl;

    return 0;
}
