#ifndef SIM_H
#define SIM_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <math.h>
#include <queue>
#include <algorithm>

#include "../include/init.h"

///Global vectors for final results
extern std::vector<int> peakInfected;
extern std::vector<int> peakDay;
extern std::vector<int> endDay;
extern std::vector<int> totalInfected;

///Main area class that acts as a container to hold simulated information for any given area
class Area {

private:
    int ID;
    int susceptible;
    int infected;
    int recovered;
    int vaccinated;
    std::vector<int> adjacencyList;
    int population;

public:
    std::vector<int> infectedList;
    std::vector<int> recoveredList;
    void SetVaccines(int numVaccines);

    void SetPopulation(int population);
    void SetID(int ID);
    void SetAdjacencyList(std::vector<int> adjacencyList);
    void SetInfected(int infected);
    void SetSusceptible(int susceptible);
    void SetRecovered(int recovered);

    int GetInfected();
	int GetRecovered();
    int GetID();
    int GetPopulation();
    int GetVaccinated();
	
    std::vector<int> GetAdjacencyList();

    void PrintInfo();
    void SetDefaults();

};

struct sortAreaComparator {

    int areaNumber;
    int distanceValue;
    int vaccineAmt = 0;

    bool operator() (sortAreaComparator a, sortAreaComparator b) {

        if (a.distanceValue != b.distanceValue) { return(a.distanceValue < b.distanceValue); }
        else {

            return a.areaNumber < b.areaNumber;

        } //else
    } //function

};

enum distribution {

    closeness = 'C',
    degree = 'D',
    randomly = 'R',
    equal = 'E',
    proportional = 'P'

};

//Main simulation logic
void RunSim(std::vector<Area> areaList, distribution type, configData cfgData);

//Check if any areas have infected members
bool isInfectious(std::vector<Area> areaList);

//Print each area's info, this is the main print statement seen in console
void printInfo(std::vector<Area> areaList);

//Checks all areas and spreads infection if appropriate
void infectAdjacent(std::vector<Area>& areaList);

//Create list of area objects with appropriate information
std::vector<Area> createAreaList(std::map<int, int>, std::vector<std::vector<int> >);

//Vaccine distribution functions
std::vector<int> GetRandomDist(std::vector<Area> areaList, configData cfgData);
std::vector<int> GetEqualDist(std::vector<Area> areaList, configData cfgData);
std::vector<int> GetProportionalDist(std::vector<Area> areaList, configData cfgData);
std::vector<int> GetWeightedDist(std::vector<Area> areaList, configData cfgData);
std::vector<int> GetClosenessDist(std::vector<Area> areaList, configData cfgData);
std::vector<int> GetDegreeDist(std::vector<Area> areaList, configData cfgData);

int shortestDistance(std::vector<int> edges[], int start, int end, int size);

//Prints the final results
void printFinalResults();

#endif
