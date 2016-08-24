// #################################
// # Author: Timon Heim
// #         Rebecca Carney
// # Email: timon.heim at cern.ch
// #        rebecca.carney at cern.ch
// # Project: LeCroy Eth Scope
// # Description: Container to save waveforms
// # Date: 28.10.14
// ################################

#include <WaveContainer.h>

WaveContainer::WaveContainer() {
    timestamp = 0;
    xscale = 1;
    yscale = 1;
}

WaveContainer::WaveContainer(const WaveContainer &w) {
    timestamp = w.getTime();
    xscale = w.getXscale();
    yscale = w.getYscale();
    xunit = w.getXunit();
    yunit = w.getYunit();

    m_x = w.getX();
    m_y = w.getY();
}

int WaveContainer::getSize(){
    return m_y.size();
}

void WaveContainer::toFile(std::string filename) {
    // Open File
    std::fstream outfile(filename.c_str(), std::ios::out);
    if(!outfile) {
        std::cout << "# ERROR # " << __PRETTY_FUNCTION__ << " -> Couldn't open file!" << std::endl;
        return;
    }
    
    // Write header
    outfile << "#Timestamp  " << timestamp << std::endl;
    outfile << "#Xscale     " << xscale << " " << xunit << std::endl;
    outfile << "#Yscale     " << yscale << " " << yunit << std::endl;
    outfile << "#Npoints    " << m_x.size() << std::endl;
    // Write data
    for(unsigned int i=0; i<m_x.size(); i++)
        outfile << m_x[i] << " " << m_y[i] << std::endl;

    outfile.close();
}

void WaveContainer::fromFile(std::string filename) {
    std::fstream infile(filename.c_str(), std::ios::in);
    if(!infile) {
        std::cout << "# ERROR # " << __PRETTY_FUNCTION__ << " -> Couldn't open " << filename<< std::endl;
        return;
    }

    std::string trash;
    int npoints = 0;
    // Read header
    infile >> trash >> timestamp;
    infile >> trash >> xscale >> xunit;
    infile >> trash >> yscale >> yunit;
    infile >> trash >> npoints;
    // Read data
    this->resetData();
    for (int i=0; i<npoints; i++) {
        double x = 0;
        double y = 0;
        infile >> x >> y;
        if (!infile) {
            std::cout << "# ERROR # " << __PRETTY_FUNCTION__ << " -> Expected more data!" << std::endl;
            break;
        }
        this->addEntry(x, y);
    }
    infile.close();
}

void WaveContainer::trim(double xlow, double xhigh) {
    for (unsigned int i=0; i<m_x.size(); i++) {
        if (m_x[i] <= xlow || m_y[i] >= xhigh) {
            m_x.erase(m_x.begin()+i);
            m_y.erase(m_y.begin()+i);
            i--;
        }
    }
}

// Getters
int WaveContainer::getTime() const {
    return timestamp;
}

double WaveContainer::getXscale() const {
    return xscale;
}

double WaveContainer::getYscale() const {
    return yscale;
}

std::string WaveContainer::getXunit() const {
    return xunit;
}

std::string WaveContainer::getYunit() const {
    return yunit;
}

vector<double> WaveContainer::getX() const {
    return m_x;
}

vector<double> WaveContainer::getY() const {
    return m_y;
}

double WaveContainer::getX(int i) const {
    return m_x[i];
}

double WaveContainer::getY(int i) const {
    return m_y[i];
}

// Setters
void WaveContainer::setData(std::vector<double> x, std::vector<double> y) {
    m_x = x;
    m_y = y;
}

void WaveContainer::replaceEntry(int i, double x, double y){
    m_x[i] = x;
    m_y[i] = y;
}

void WaveContainer::addEntry(double x, double y){
    m_x.push_back(x);
    m_y.push_back(y);
}

void WaveContainer::resetData() {
    m_x.clear();
    m_y.clear();
}
void WaveContainer::setTime(int arg_time) {
    timestamp = arg_time;
}

void WaveContainer::setXscale(double arg_xscale) {
    xscale = arg_xscale;
}

void WaveContainer::setYscale(double arg_yscale) {
    yscale = arg_yscale;
}

void WaveContainer::setXunit(std::string arg_xunit) {
    xunit = arg_xunit;
}

void WaveContainer::setYunit(std::string arg_yunit) {
    yunit = arg_yunit;
}
