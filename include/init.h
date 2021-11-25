#ifndef INIT_H
#define INIT_H

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <sstream>

//#include "../include/sim.h"

//Holds data from config file
struct configData {

    std::string populationFile;
    std::string regionFile;
    int infectedArea;
    int infectiousPeriod;
    int contactRate;
    int vaccines;
	
};

//Read in info and return that info in appropriate data structure
configData getConfig();
std::map<int, int> getPopulationData(configData);
std::vector<std::vector<int> > getAdjacencyList(configData);

//Print info
void printInfo(configData cfgData);
void printInfo(std::map<int, int> popData);
void printInfo(std::vector<std::vector<int> > adjacencyList);

#endif