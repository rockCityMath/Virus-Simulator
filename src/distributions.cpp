#ifndef DISTRIBUTIONS_CPP
#define DISTRIBUTIONS_CPP

#include "../include/sim.h"

//Distribute the vaccines area by area in descending order
std::vector<int> GetRandomDist(std::vector<Area> areaList, configData cfgData) {

    std::vector<int> vaccineList;
    int vaccineTotal = cfgData.vaccines;

    //Loop through all areas, allocate as many vaccines as possible than move to the next area
    for(int i = 0; i < areaList.size(); i++) {
        if(vaccineTotal > areaList[i].GetPopulation()) {
            vaccineList.push_back(areaList[i].GetPopulation());
            vaccineTotal = vaccineTotal - areaList[i].GetPopulation();
        }
        else {
            vaccineList.push_back(vaccineTotal);
            vaccineTotal = 0;
        }
    }

    return vaccineList;

}

//Distribute vaccines equally among all areas
std::vector<int> GetEqualDist(std::vector<Area> areaList, configData cfgData) {

    std::vector<int> vaccineList;
    int vaccineTotal = cfgData.vaccines;

    //Calculate share and distribute it to each area
    int share = vaccineTotal / areaList.size();

    for(int i = 0; i < areaList.size(); i++)
    {
        //Initial equal distribution
        if(areaList[i].GetPopulation() > share)
        {
            vaccineList.push_back(share);
            vaccineTotal = vaccineTotal - share;
        }
        else
        {
            vaccineList.push_back( areaList[i].GetPopulation() );
            vaccineTotal = vaccineTotal - areaList[i].GetPopulation();

        }
        
    }

    //Distribute any remaining vaccines to areas one by one in descending order
    while(vaccineTotal > 0) {

        for(int i = 0; i < areaList.size(); i++) {

            if( (vaccineTotal > 0) && (vaccineList[i] < areaList[i].GetPopulation()) ) {

                vaccineList[i] = vaccineList[i] + 1;
                vaccineTotal--;
                    
            }  

        }

    }
    
    return vaccineList;
}

//Distribute vaccines based on areas population as a proportion to the entire regions population
std::vector<int> GetWeightedDist(std::vector<Area> areaList, configData cfgData) {

    std::vector<int> vaccineList;
    float vaccineTotal = cfgData.vaccines;
    int distributedVaccines = 0;

    //Get total population amount
    float totalPopulation = 0;
    for(int i = 0; i < areaList.size(); i++) {

        totalPopulation = totalPopulation + areaList[i].GetPopulation();

    }

    //Calculate proportion and use it to find vaccine allocation for each area
    float proportion = vaccineTotal / totalPopulation;
    int areaVaccineAmt = 0;

    for(int i = 0; i < areaList.size(); i++) {

        areaVaccineAmt = proportion * areaList[i].GetPopulation();
        vaccineList.push_back(areaVaccineAmt);
        distributedVaccines = distributedVaccines + areaVaccineAmt;

    }

    //Find discrepancy between total vaccines and all distributed vaccines so any missing can be distributed
    int discrepancy = vaccineTotal - distributedVaccines;

    while(discrepancy > 0) {

        for(int i = 0; i < areaList.size(); i++) {

            if(discrepancy > 0) {

                areaVaccineAmt = vaccineList[i];
                vaccineList[i] = vaccineList[i] + 1;
                discrepancy--;

            }   
        
        }
    }

    return vaccineList;

}

std::vector<int> GetClosenessDist(std::vector<Area> areaList, configData cfgData) {

    std::vector<int> vaccineList;
    std::vector<sortAreaComparator> sortedAreaList;
    int vaccineTotal = cfgData.vaccines;

    //Calculate number of edges on graph
    int edgeAmount = 0;
    for(int i = 0; i < areaList.size(); i++) {
        std::vector<int> adjList = areaList[i].GetAdjacencyList();

        for(int j = 0; j < adjList.size(); j++) {

            edgeAmount++;

        }

    }

    //Create and populate edges vector
    std::vector<int> edges[edgeAmount];

    for(int i = 0; i < areaList.size(); i++) {
        std::vector<int> adjList = areaList[i].GetAdjacencyList();

        for(int j = 0; j < adjList.size(); j++) {

            edges[i].push_back(adjList[j] - 1);

        }

    }

    std::vector<int> areaDistanceValue;
    int distance = 0;
    int minEdges = 0;
    sortAreaComparator temp;

    //Find and assign distance value for each area
    for(int i = 0; i < areaList.size(); i++) {

        distance = 0;

        for(int j = 0; j < areaList.size(); j++) {

            minEdges = shortestDistance(edges, i, j, edgeAmount);
            distance = minEdges + distance;

        }

        temp.distanceValue = distance;
        temp.areaNumber = i;
        sortedAreaList.push_back(temp);


    }

    //Sort area list based on lowest closeness value, then by area numben
    std::sort(sortedAreaList.begin(), sortedAreaList.end(), sortAreaComparator());

    //Distribute vaccines to each area in descending order
    for(int i = 0; i < sortedAreaList.size(); i++) {

        int currentArea = sortedAreaList[i].areaNumber;

        if(vaccineTotal > areaList[currentArea].GetPopulation()) {
            sortedAreaList[i].vaccineAmt = areaList[currentArea].GetPopulation();
            vaccineTotal = vaccineTotal - areaList[currentArea].GetPopulation();
        }
        else {
            sortedAreaList[i].vaccineAmt = vaccineTotal;
            vaccineTotal = 0;
        }
    }

    //Push vaccine amounts to vector in order of their corresponding area
    for(int i = 0; i < sortedAreaList.size(); i++) {

        for(int j = 0; j < sortedAreaList.size(); j++) {

            if(sortedAreaList[j].areaNumber == i) {

                vaccineList.push_back(sortedAreaList[j].vaccineAmt);

            }

        }

    }

    return vaccineList;

}

//Find shortest distance between 2 nodes based on Dijkstra's algorithm
int shortestDistance(std::vector<int> edges[], int start, int end, int size) {

    //Keep track of distance from start and visited status per node
    std::vector<bool> visitedList(size, 0);
    std::vector<int> distanceList(size, 0);

    //Queue to hold nodes that we will visit
    std::queue <int> nodesQueue;
    distanceList[start] = 0;

    //Push starting node and set it to visited
    nodesQueue.push(start);
    visitedList[start] = true;

    //While there are nodes to visit
    while( !nodesQueue.empty() ){

        //Start searching on the front node
        int currentNode = nodesQueue.front();
        nodesQueue.pop();

        //For every edge on the current node
        for(int i = 0; i < edges[currentNode].size(); i++) {

            //Continue if edge has been visited
            if(visitedList[ edges[currentNode][i] ]) {
                continue;
            }

            distanceList[edges[currentNode][i]] = distanceList[currentNode] + 1;
            nodesQueue.push( edges[currentNode][i] );
            visitedList[ edges[currentNode][i] ] = 1;

        }

    }

    return distanceList[end];

}

//Distribute vaccines based on amount of adjacent areas (degree)
std::vector<int> GetDegreeDist(std::vector<Area> areaList, configData cfgData) {
    
    std::vector<int> vaccineList;
    int vaccineTotal = cfgData.vaccines;
    int distributedVaccines = 0;
    
    int degree[areaList.size()];
    int priorityList[areaList.size()];

    //Calculate the degree for each node
    std::vector<int> degList;
    for (int i = 0; i < areaList.size(); i++) {
        degList = areaList[i].GetAdjacencyList();
        degree[i] = degList.size();
    }
    //sorting the areas to specification (highest degree first, lowest area number second)
    int highestDeg = 0;
    int position = 0;
    for (int i = 0; i < areaList.size(); i++) {
        for (int j = 0; j < areaList.size(); j++) {
            if (degree[j] > highestDeg) {
                highestDeg = degree[j];
                position = j;
            }
        }
        priorityList[i] = position;
        degree[position] = 0;
        highestDeg = 0;
    }
    //loop through the areas using priorityList to allocate as many vaccines as possible
    for (int i = 0; i < areaList.size(); i++) {
        vaccineList.push_back(0);
    }
    for (int i = 0; i < areaList.size(); i++) {
        if (vaccineTotal > areaList[priorityList[i]].GetPopulation()) {
            vaccineList[priorityList[i]] = areaList[priorityList[i]].GetPopulation();
            vaccineTotal = vaccineTotal - areaList[priorityList[i]].GetPopulation();
        }
        else {
            vaccineList[priorityList[i]] = vaccineTotal; 
            vaccineTotal = 0;
        }
    }
    return vaccineList;
}

#endif
