// #################################
// # Author: Timon Heim
// #         Rebecca Carney
// # Email: timon.heim at cern.ch
// #        rebecca.carney at cern.ch
// # Project: LeCroy Eth Scope
// # Description: Container to save waveforms
// # Date: 28.10.14
// ################################

#ifndef WAVECONTAINER_H
#define WAVECONTAINER_H

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <sstream>

using namespace std;

class WaveContainer{
	public:
		//Constructors, destructors
		WaveContainer();
        WaveContainer(const WaveContainer &w);
		~WaveContainer(){};

		//Methods
		void toFile(string dir);
		void fromFile(string filename);
        void trim(double xlow, double xhigh);
		int getSize();

		//Getters
        int getTime() const;
        double getXscale() const;
        double getYscale() const;
        std::string getXunit() const;
        std::string getYunit() const;
		double getX(int i) const;
		double getY(int i) const;
		vector<double> getX() const;
		vector<double> getY() const;

		//Setters
		void setData(std::vector<double> x, std::vector<double> y);
		void replaceEntry(int i, double x, double t);
		void addEntry(double x, double y);
		void resetData();

		void setTime(int arg_time);
		void setXscale(double arg_xscale);
		void setYscale(double arg_yscale);
		void setXunit(std::string arg_xunit);
		void setYunit(std::string arg_yunit);

	private:
		vector<double> m_x;
		vector<double> m_y;

		// Header
		int timestamp;
		double xscale;
		double yscale;
		std::string xunit;
		std::string yunit;
};


#endif


