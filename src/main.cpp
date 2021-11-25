///Main program functions with high level of abstraction
#ifndef MAIN_CPP
#define MAIN_CPP

#include "../include/init.h"
#include "../include/sim.h"

std::vector<int> peakInfected(4);
std::vector<int> peakDay(4);
std::vector<int> endDay(4);
std::vector<int> totalInfected(4);

int main() {

    //Get the data needed for the simulation
    configData cfgData = getConfig();
    std::map<int, int> popData = getPopulationData(cfgData);
    std::vector<std::vector<int> > adjacencyList = getAdjacencyList(cfgData);

    //Print data
    printInfo(popData);
    printInfo(adjacencyList);

    //create areaList vector
    std::vector<Area> areaList = createAreaList(popData, adjacencyList);

    //Run different simulations
    RunSim(areaList, closeness, cfgData);
    RunSim(areaList, degree, cfgData);
    RunSim(areaList, randomly, cfgData);
    RunSim(areaList, equal, cfgData);
    RunSim(areaList, proportional, cfgData);

	//Print final results
	printFinalResults();
	
    //system("pause");

    return 0;

}

#endif
