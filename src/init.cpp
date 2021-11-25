///Functions to initialize the simulation
#ifndef INIT_CPP
#define INIT_CPP

#include "../include/init.h"

///Get information from config file and return a struct
configData getConfig() {

    bool validFile = false;
    std::fstream configFile;
    std::string fileName;
    configData returnInfo;

    while ( !validFile ) {

        //Open user specified config file and check that its valid
        std::cout << "Enter config file: ";
        std::getline(std::cin, fileName);
        configFile.open(fileName);

        if (configFile.good() ) {

            std::cout << "Valid Config File!" << std::endl;
            validFile = true;
        }
        else { std::cout << "Invalid Config File... " << std::endl; }

    }

    //Initialize variables to be pulled from config file
    std::string populationFile = "";
    std::string regionFile = "";
    int infectedArea = 0;
    int infectiousPeriod = 0;
    int contactRate = 0;
    int vaccines = 0;

    //Read in variables from config file, use semicolon as delimiter
    configFile.ignore(256, ':');
    std::getline(configFile, populationFile);

    configFile.ignore(256, ':');
    std::getline(configFile, regionFile);

    configFile.ignore(256, ':');
    configFile >> infectedArea;

    configFile.ignore(256, ':');
    configFile >> infectiousPeriod;

    configFile.ignore(256, ':');
    configFile >> contactRate;

    configFile.ignore(256, ':');
    configFile >> vaccines;
    configFile.close();

    //Save data from config file to configData struct and return it
    returnInfo.populationFile = populationFile;
    returnInfo.regionFile = regionFile;
    returnInfo.infectedArea = infectedArea;
    returnInfo.infectiousPeriod = infectiousPeriod;
    returnInfo.contactRate = contactRate;
    returnInfo.vaccines = vaccines;

    return returnInfo;

}

///Get the list of population data from the population file and return a map
std::map<int, int> getPopulationData(configData cfgData) {

    std::fstream populationFile;
    populationFile.open(cfgData.populationFile);

    if (populationFile.good()) {std::cout << "Valid Population File!" << std::endl; }
    else {std::cout << "Invalid Population File..." << std::endl; }

    //Use map to hold ID and population values
    std::map<int, int> populationData;
    int ID;
    int population;

    //Iterate through entire file and add all population values to the map
    while ( !populationFile.eof() ) {

        populationFile >> ID;
        populationFile.ignore(256, ':');
        populationFile >> population;
        populationData[ID] = population;
    }

    populationFile.close();

    return populationData;

}

///Get adjacency information from region file and return a vector of vectors
std::vector<std::vector<int> > getAdjacencyList(configData cfgData) {

    //Open and check file
    std::fstream regionFile;
    regionFile.open(cfgData.regionFile);

    if (regionFile.good()) {std::cout << "Valid Region File!" << std::endl;}
    else {std::cout << "Invalid Region File..." << std::endl; }

    //Calculate grid size
    int amtRows = 0;
    std::string tempStr;

    //Get amount of rows
    //Each time the program can read in a row from the CSV file, increase amtRows by 1
    while ( std::getline(regionFile, tempStr) ) {

        amtRows++;

    }
    amtRows++;

    regionFile.close();
    regionFile.open(cfgData.regionFile);

    //Container that will hold the lists
    std::vector<std::vector<int> > adjacencyLists(amtRows);

    //Initialize variables and discard first line
    std::getline(regionFile, tempStr);
    int areaNumber = 1;
    std::string val;
    std::vector<int> individualList;

    //Go row by row, if theres a 1, that row(the area) has adjacency to the number of the collumn(the adjacent area)
    for (int i = 0; i < amtRows; i++) {

        std::getline(regionFile, tempStr);
        std::istringstream streamLine(tempStr);
        val = " ";
        std::getline(streamLine, val, ',');

        for (int j = 0; j < amtRows; j++) {

            val = " ";
            std::getline(streamLine, val, ',');
            std::istringstream toList(val);
            int value = 0;
            toList >> value;

            if (value == 1) {

                adjacencyLists[i+1].push_back(j+1);

            }

        }

    }

    return adjacencyLists;

}

///Print info from config file
void printInfo(configData data) {

    std::cout << "-------   CONFIG DATA ------- " << std::endl;
    std::cout << "Population File: " << data.populationFile << std::endl;
    std::cout << "Region File: " << data.regionFile << std::endl;
    std::cout << "Infected Area: " << data.infectedArea << std::endl;
    std::cout << "Infectious Period: " << data.infectiousPeriod << std::endl;
    std::cout << "Contact Rate: " << data.contactRate << std::endl;
    std::cout << "Number of Vaccines: " << data.vaccines << std::endl << std::endl;

}

///Print info from population file
void printInfo(std::map<int, int> popData) {

    std::cout << "Regional Population" << std::endl;

    for (auto i = popData.begin(); i != popData.end(); i++) {

        std::cout << i->first << "  " << i->second << std::endl;

    }

}

///Print info from adjacency list
void printInfo(std::vector<std::vector<int> > adjacencyList) {

    std::cout << std::endl << "Adjacency List" << std::endl;

    for (int i = 1; i < adjacencyList.size() - 1; i++ ) {

        std::cout << i << ": ";

        for (int j = 0; j < adjacencyList[i].size(); j++) {

            std::cout << adjacencyList[i].at(j) << " ";

        }

        std::cout << std::endl;

    }

    std::cout << std::endl;

}

#endif