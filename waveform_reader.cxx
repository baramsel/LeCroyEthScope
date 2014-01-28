#include <iostream>
#include <fstream>
#include <string>

#include <boost/lexical_cast.hpp>

#define HEX(x) std::hex << "0x" << static_cast<int>(x) << std::dec

unsigned checkHeader(char *header) {
    unsigned length = 0;
    length += (0xFF & header[7]) << 0;
    length += (0xFF & header[6]) << 8;
    length += (0xFF & header[5]) << 16;
    length += (0xFF & header[4]) << 24;
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
    file.seekg(0, std::ios::end);
    unsigned file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    unsigned cur = file.tellg();
    
    std::cout << "Fiel size = " << file_size << " byte" << std::endl;
    std::cout << std::endl << "################################" << std::endl;
    std::cout << "Messag Part #1:" << std::endl;
    std::cout << "################################" << std::endl;
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

    std::cout << std::endl << "################################" << std::endl;
    std::cout << "Messag Part #2:" << std::endl;
    std::cout << "################################" << std::endl;
    //Get length of waveform data
    file.read(header, 8);
    unsigned desc_length = checkHeader(header);
    //unsigned desc_length = boost::lexical_cast<unsigned>(msg.substr(msg.size()-9, 9));
    std::cout << "Decriptor length: " << desc_length << std::endl;
    std::string desc;
    desc.resize(desc_length, ' ');
    begin = &*desc.begin();
    file.read(begin, desc_length);
    for (unsigned i=0; i<desc_length; i++) {
        if (i % 16  == 0) {
            std::cout << std::endl << i << "\t";
        }
        std::cout << HEX(0xFF & desc[i]) << " ";
    }
    std::cout << std::endl;
    //std::cout << "Descriptor content: \"" << desc << "\"" << std::endl;
    if (file.eof()) std::cout << "End  file reached" << std::endl;

    // Get further parameters
    int wave_desc = 0;
    wave_desc += (0xFF & desc[39]) << 0;
    wave_desc += (0xFF & desc[38]) << 8;
    wave_desc += (0xFF & desc[37]) << 16;
    wave_desc += (0xFF & desc[36]) << 24;

    int wave_array1 = 0;
    wave_array1 += (0xFF & desc[63]) << 0;
    wave_array1 += (0xFF & desc[62]) << 8;
    wave_array1 += (0xFF & desc[61]) << 16;
    wave_array1 += (0xFF & desc[60]) << 24;

    int wave_array_count = 0;
    wave_array_count += (0xFF & desc[119]) << 0;
    wave_array_count += (0xFF & desc[118]) << 8;
    wave_array_count += (0xFF & desc[117]) << 16;
    wave_array_count += (0xFF & desc[116]) << 24;

    std::cout << "Wave descriptor length: " << wave_desc << std::endl;
    std::cout << "Wave array length: " << wave_array1 << std::endl;
    std::cout << "Wave number of datapoints: " << wave_array_count << std::endl;
    
    std::cout << std::endl << "################################" << std::endl;
    std::cout << "Messag Part #3:" << std::endl;
    std::cout << "################################" << std::endl;
    cur = file.tellg();
    std::cout << file_size - cur << " bytes left" << std::endl;
    file.read(header, 8);
    if (file.eof()) std::cout << "End  file reached" << std::endl;
    cur = file.tellg();
    std::cout << file_size - cur << " bytes left" << std::endl;
    unsigned wave_length = checkHeader(header);
    std::cout << "Decriptor length: " << wave_length << std::endl;
    std::string wave;
    wave.resize(wave_length, 0x0);
    begin = &*wave.begin();
    file.read(begin, wave_length);
    for (unsigned i=0; i<wave_length; i++) {
        if (i % 16  == 0) {
            std::cout << std::endl << i << "\t";
        }
        std::cout << HEX(0xFF & wave[i]) << " ";
    }
    std::cout << std::endl;
    if (file.eof()) std::cout << "End  file reached" << std::endl;
    
    std::cout << std::endl << "################################" << std::endl;
    std::cout << "Messag Part #4:" << std::endl;
    std::cout << "################################" << std::endl;
    cur = file.tellg();
    std::cout << file_size - cur << " bytes left" << std::endl;
    file.read(header, 8);
    if (file.eof()) std::cout << "End  file reached" << std::endl;
    cur = file.tellg();
    std::cout << file_size - cur << " bytes left" << std::endl;
    unsigned wave_length2 = checkHeader(header);
    int i=0;
    while(file) {
        if (i % 16  == 0) {
            std::cout << std::endl << i << "\t";
        }
        char tmp;
        file >> tmp;
        std::cout << HEX(0xFF & tmp) << " ";
        i++;
    }
    return 0;
}
