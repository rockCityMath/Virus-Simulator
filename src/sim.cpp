#include "../include/sim.h"

///Get/Set attributes of the area class
void Area::SetPopulation(int population) { this->population = population; }
void Area::SetID(int ID) { this->ID = ID; }
void Area::SetAdjacencyList(std::vector<int> adjacencyList) { this-> adjacencyList = adjacencyList; }
void Area::SetInfected(int infected) { this->infected = infected; }
void Area::SetDefaults() { susceptible = 0; infected = 0; recovered = 0; vaccinated = 0; }
void Area::SetSusceptible(int susceptible) { this->susceptible = susceptible; }
void Area::SetRecovered(int recovered) { this->recovered = recovered; }
void Area::SetVaccines(int vaccinated) { this->vaccinated = vaccinated; }

int Area::GetInfected() { return infected; }
int Area::GetRecovered() { return recovered; }
int Area::GetID() { return ID; }
int Area::GetPopulation() { return population; }
int Area::GetVaccinated() { return vaccinated; }
std::vector<int> Area::GetAdjacencyList() { return adjacencyList; }

///Print info about the area
void Area::PrintInfo() {

    std::cout << std::left << ID << "   Pop: " << std::setw(5) << population << "   S: " << std::setw(5) << susceptible << "   I: " << std::setw(5) << infected
              << "   R: " << std::setw(6) << recovered << "   V: " << vaccinated << std::endl;

}

///Main simulation function that simulates days
void RunSim(std::vector<Area> areaList, distribution type, configData cfgData) {
	
    //Initial Variables
    int day = 0;
    int infected = 0;
    int recovered = 0;
    int susceptible = 0;
    std::vector<int> vaccineList;
    int vaccines;
	
	//Final result variables
	int peakInfected[4] = { 0, 0, 0, 0 };
	int totalInfected[4] = { 0, 0, 0, 0 };
	int arrLoc; // array location to determined by distribution method

    //Set vaccines and title based on distribution
	if (type =='C') {

        std::cout << "CLOSENESS DISTRIBUTION";
        vaccineList = GetClosenessDist(areaList, cfgData);
		arrLoc = 0;

    }
    else if (type == 'D') {

        std::cout << "DEGREE DISTRIBUTION";
        vaccineList = GetDegreeDist(areaList, cfgData);
		arrLoc = 1;

    }
	else if (type == 'R') {

		std::cout << "RANDOM DISTRIBUTION";
        vaccineList = GetRandomDist(areaList, cfgData);
		arrLoc = 2;

	}
    else if (type == 'E') {

        std::cout << "EQUAL DISTRIBUTION";
        vaccineList = GetEqualDist(areaList, cfgData);
		arrLoc = 3;

    }
    else if (type == 'P') {
        
        std::cout << "WEIGHTED DISTRIBUTION";
        vaccineList = GetWeightedDist(areaList, cfgData);
		arrLoc = 4;

    }
	
    //Assign vaccines based on vector returned from appropriate distribution
    for (int i = 0; i < areaList.size(); i++) {

        vaccines = vaccineList.at(i);
        areaList.at(i).SetVaccines( vaccines );

    }

    areaList[cfgData.infectedArea - 1].SetInfected(1); //Initial infection

    //Per day
    while ( isInfectious(areaList) ) {
	
        std::cout << std::endl << "Day " << day << " " << std::endl;
        infectAdjacent(areaList);

        //Per area
        for (int i = 0; i < areaList.size(); i++ ) {
			
            //Get variables
            infected = areaList[i].GetInfected();
            recovered = 0;
            susceptible = 0;
            int infectedSize  = areaList[i].infectedList.size();
            int infectiousPeriod = cfgData.infectiousPeriod;
            std::vector<int> infectedList = areaList[i].infectedList;
            std::vector<int> recoveredList = areaList[i].recoveredList;
            int contactRate = cfgData.contactRate;
            int population = areaList[i].GetPopulation();
            int vaccines = areaList[i].GetVaccinated();
            int availiblePopulation = population - vaccines;

            //    1. ----------------- Calculate Recovered ----------------------

            //If its been less days since the infection started in that region than days in the infectious period,
            //there will never be any recovered
            if(infectedSize < infectiousPeriod) {

                recovered = 0;

            }

            //If days since infection started in the area is between the infectious period, and the infectious period * 2
            //Then recovered people will always be the amount that were infected that period ago
            else if ( (infectedSize >= infectiousPeriod) && (infectedSize < infectiousPeriod * 2) ) {

                recovered = infectedList[infectedList.size() - infectiousPeriod];

            }

            //If niether of the first statements are true, we must use the true formula for calculating recovered
            //Recovered will always be the amount that are recovering that day, from the infectious period of time ago
            //Plus the amount of people that were already recovered on that day
            else {

                //recovered = infectedList[day - infectiousPeriod] + infectedList[day - infectiousPeriod * 2] + recoveredList[day - infectiousPeriod * 2];
                recovered = infectedList[infectedList.size() - infectiousPeriod] + recoveredList[infectedList.size() - infectiousPeriod];

            }

            //   2. ------------------------ Calculate Infected ---------------------

            //If the infection has not yet begun in the region, or its it's first day being infected
            //Then the infected will always be the amount specified already
            if (infectedSize == 0) {

                infected = areaList.at(i).GetInfected();

                if(infected < 0) {
                    infected = 0;
                }

            }
            
            //Weird case that has to be here, if its the first day someone is recovering in the region
            //Then the infected will always be the formula for infected minus 1
            else if (recovered == 1) {

                infected = infectedList[infectedList.size() - 1] + ((infectedList[infectedList.size() - 1] - recoveredList[infectedList.size() - 1]) * contactRate);
                infected--;

                if(infected < 0) {
                    infected = 0;
                }

            }

            //If the first 2 cases are not valid, then we must use the true formula for determining infected
            //Then infected will always be the infected from one day ago
            //Plus (the infected from one day ago, minus the recovered from one day ago, times the contact rate) (this is the amount that can do infecting times the rate at which they infect)
            else {

                infected = infectedList[infectedList.size() - 1] + ((infectedList[infectedList.size() - 1] - recoveredList[infectedList.size() - 1]) * contactRate);
                
                if (infectedSize > infectiousPeriod) {

                    infected = infectedList[infectedSize - 1] + ((infectedList[infectedSize - 1] - infectedList[infectedSize - (infectiousPeriod + 1) ]) * contactRate);

                }
            
                if (infected < 0) {
                    infected = 0;
                }

            }

            //Infected can never be greater than the available population minus the recovered
            if (infected > (availiblePopulation - recovered)) {

                infected  = availiblePopulation - recovered;

            }

            //   3. ----------------------- Calculate Susceptible -------------------

            //Since susceptible is found last, it can always be determined by subtracting the infected and recovered from the total
            //population that is available to be infected
            susceptible = availiblePopulation - infected - recovered;

            //Add all things we found to the area object in the list
            areaList.at(i).SetInfected(infected);
            areaList.at(i).SetSusceptible(susceptible);
            areaList.at(i).SetRecovered(recovered);

            //Print out the SIRV information for the area as specified
            areaList.at(i).PrintInfo();

            //Only push if region has started its infection, dont want to always push to the list. List size represents days infected
            if (infected > 0) {

                areaList.at(i).recoveredList.push_back(recovered);
                areaList.at(i).infectedList.push_back(infected);

            }
			
			
			
        } //area
		
		///Final Results Calculations
		int local_peakInfected = 0; //Day's infected used to compare against peak infected
		int totalRecovered = 0; //Used to calculate total infected
		
		for (int i = 0; i < areaList.size(); i++) {
			
			//Peak Infected
			local_peakInfected += areaList.at(i).GetInfected();
			
			if (local_peakInfected > peakInfected[arrLoc]) {
				peakInfected[arrLoc] = local_peakInfected;
				
				//Day of peak infected
				peakDay[arrLoc] = day;
			}
			
			//Total Infected (determined by total recoveries)
			totalRecovered += areaList.at(i).GetRecovered();
			
			if (totalRecovered > totalInfected[arrLoc])
				totalInfected[arrLoc] = totalRecovered;
			
		}
		
		//Last day of outbreak
		endDay[arrLoc] = day;
		
		day++;

    } //day
	
    std::cout << std::endl;

}

///Return true if there are any infectious members in the region
bool isInfectious(std::vector<Area> areaList) {

    int infectious = 0;

    for (int i = 0; i < areaList.size(); i++) {

        infectious = infectious + areaList.at(i).GetInfected();

    }

    if (infectious > 0) { return true; }
    else { return false; }

}

///Create list of area objects with appropriate information
std::vector<Area> createAreaList(std::map<int, int> popData, std::vector<std::vector<int> > adjacencyList) {

    std::vector<Area> areaList;
    Area tempArea;

    //Loop through all areas and assign appropriate data to an area object
    for (int i = 1; i < adjacencyList.size() - 1; i++ ) {

        tempArea.SetAdjacencyList( adjacencyList.at(i) );
        tempArea.SetPopulation( popData.at(i) );
        tempArea.SetID( i );
        tempArea.SetDefaults();

        areaList.push_back(tempArea);

    }

    return areaList;

}

///Print all info for all areas in a the areaList
void printInfo(std::vector<Area> areaList) {

    for (int i = 0; i < areaList.size(); i++){

        areaList.at(i).PrintInfo();

    }

}

//Check areas and infect adjacent areas if an area is over 50% population
void infectAdjacent(std::vector<Area>& areaList){

    for (int i = 0; i < areaList.size(); i++) {

        int infected = areaList[i].GetInfected();

        //If area's infected populus is greater than 50% of its total
        if (infected > (areaList[i].GetPopulation() / 2)) {

                std::vector<int> adjList = areaList[i].GetAdjacencyList();

                //Loop through list of adjacent areas and add an infected member if appropriate
                for (int j = 0; j < adjList.size(); j++) {

                    int adjArea = adjList.at(j) - 1;

                    if (areaList[adjArea].GetInfected() == 0 && (areaList[adjArea].GetPopulation() > areaList[adjArea].GetVaccinated()) ) {
                        areaList[adjArea].SetInfected(1);
                        
                    }

                }

        }

    }

}

void printFinalResults() {
	std::string distributionList[5] =  {"closeness", "degree", "random", "equal", "weighted"};
	
	for (int i = 0; i < 5; i++)
		std::cout << "Using the " << distributionList[i] << " distribution method, the peak number of infected was " << peakInfected[i] << " on day " << peakDay[i] << ". The outbreak ended on day " << endDay[i] << " and the total number of infected individuals was " << totalInfected[i] << "." << std::endl;
}
