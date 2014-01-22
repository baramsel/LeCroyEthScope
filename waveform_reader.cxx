#include <iostream>
#include <fstream>
#include <string>

#include <boost/lexical_cast.hpp>

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
    std::cout << "- Block Length:    " << length << std::endl;
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
    
    // Read & Check header information
    char header[8];
    file.read(header, 8);
    unsigned msg_size = checkHeader(header);

    // Read message, size given in header
    std::string msg;
    msg.resize(msg_size, ' '); // reserve space
    char* begin = &*msg.begin();
    file.read(begin, msg_size);
    std::cout << "Message content: \"" << msg << "\"" << std::endl;

    //Get length of waveform data
    unsigned desc_length = boost::lexical_cast<unsigned>(msg.substr(msg.size()-9, 9));
    std::cout << "Decriptor length: " << desc_length << std::endl;
    std::string desc;
    desc.resize(desc_length, ' ');
    begin = &*desc.begin();
    file.read(begin, desc_length);
    std::cout << "Descriptor content: \"" << desc << "\"" << std::endl;

    return 0;
}
