#include "EthScope.h"

#define DEBUG

#define LECROY_EOI_FLAG				0x01
#define LECROY_SRQ_FLAG				0x08
#define LECROY_CLEAR_FLAG			0x10
#define LECROY_LOCKOUT_FLAG			0x20
#define LECROY_REMOTE_FLAG			0x40
#define LECROY_DATA_FLAG			0x80

#define HEX(x) std::hex << "0x" << static_cast<int>(x) << std::dec

EthScope::EthScope(std::string ip, int port) : socket(ip, port){
    header.push_back(LECROY_DATA_FLAG | LECROY_REMOTE_FLAG | LECROY_EOI_FLAG); // Operation
    header.push_back(0x1); // Header Version
    header.push_back(0x0); // Sequence number 1-255
    header.push_back(0x0); // Spare
    header.push_back(0x0); // Block length LSB
    header.push_back(0x0); // Block length
    header.push_back(0x0); // Block length
    header.push_back(0x0); // Block length MSB

    outfileName = "waveform_out.dat";
}

EthScope::~EthScope() {

}

void EthScope::setOutfile(std::string arg_name) {
    outfileName = arg_name;
}

// Prepares the Eth Header for the GPIB msg
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

// Checks the Eth Header and returns the length of the msg
unsigned EthScope::checkHeader(char *arg_header) {
    unsigned length = 0;
    length += (0xFF & arg_header[7]) << 0;
    length += (0xFF & arg_header[6]) << 8;
    length += (0xFF & arg_header[5]) << 16;
    length += (0xFF & arg_header[4]) << 24;
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << "Header data:" << std::endl;
    std::cout << "- Operation:       " << HEX(0xFF & arg_header[0]) << std::endl;
    std::cout << "- Header Version:  " << HEX(0xFF & arg_header[1]) << std::endl;
    std::cout << "- Sequence number: " << HEX(0xFF & arg_header[2]) << std::endl;
    std::cout << "- Block Length:    " << length << std::endl;
#endif
    return length;
}

// Writes a GPIB msg to the Scope
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

// Read one msg from the Scope
std::string EthScope::read() {
    std::vector<char> header = socket.read(8);
    unsigned length = this->checkHeader(&header[0]);
    std::vector<char> msg = socket.read(length);
    if (msg.size() == 0) return "";
    std::string str(msg.begin(), msg.end());
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << " : Read \"" << str << "\"" << std::endl;
#endif
    return str;
}

void EthScope::configure(std::fstream file) {
    std::string tmp;
    while(file) {
        std::getline(file, tmp);
        if (tmp[0] != '#')
            this->write(tmp);
    }
}

void EthScope::readWave() {
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << " : Starting readout" << std::endl;
#endif
    std::fstream outfile(outfileName.c_str(), std::ios::out | std::ios::binary);
    // Open output file to store output
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ << " : Opening file: " << outfileName << std::endl;
#endif
    if (!outfile) {
    std::cout << __PRETTY_FUNCTION__ << " : Error opening file: " << outfileName << std::endl;
        return;
    }
    
    // Request waveform from Scope
    // TODO Needs modification for other channels
    std::string req = "c1:waveform?";
    this->write(req);

    // Need to read feedback now
    // The "Header" first
    std::vector<char> header_header = socket.read(8);
    unsigned length_header = this->checkHeader(&header_header[0]);
    std::vector<char> msg_header = socket.read(length_header);
#ifdef DEBUG
    std::string msg_str(msg_header.begin(), msg_header.end());
    std::cout << __PRETTY_FUNCTION__ << " : Header: \"" << msg_str << "\"" <<std::endl;
#endif
    // Descriptor next
    std::vector<char> header_desc = socket.read(8);
    unsigned length_desc = this->checkHeader(&header_desc[0]);
    std::vector<char> msg_desc = socket.read(length_desc);
    // Data next
    std::vector<char> header_data = socket.read(8);
    unsigned length_data = this->checkHeader(&header_data[0]);
    std::vector<char> msg_data = socket.read(length_data);
    // End of transmission (should only be a newline)
    std::vector<char> header_end = socket.read(8);
    unsigned length_end = this->checkHeader(&header_end[0]);
    std::vector<char> msg_end = socket.read(length_end);

    // Write data to file
    outfile.write(&header_header[0], 8);
    outfile.write(&msg_header[0], length_header);
    outfile.write(&header_desc[0], 8);
    outfile.write(&msg_desc[0], length_desc);
    outfile.write(&header_data[0], 8);
    outfile.write(&msg_data[0], length_data);
    outfile.write(&header_end[0], 8);
    outfile.write(&msg_end[0], length_end);

    outfile.close();

}

void EthScope::beep() {
    std::string str = "buzz beep";
    this->write(str);
}

void EthScope::idn() {
    std::string str = "*idn?";
    this->write(str);
    std::cout << "Ident: " << this->read() << std::endl;
}

