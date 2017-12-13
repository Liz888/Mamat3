#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ex2.h"
#include "flight.h"
#include "runway.h"
#include "airport.h"

runwayNode* airport = NULL; 

//*********************************************************************************************************************************
//* Function name: createAirport
//* Description: creates the airport pointer which is actually a pointer to a runwayNode, later it will point to the begining of the runway nodes list
//* Parameters: none
//* Return value: none
//*********************************************************************************************************************************
void createAirport()
{
	airport = (runwayNode*)malloc(sizeof(runwayNode));
	if (airport == NULL)
		return;
	airport->runway = NULL;
	airport->pNext = NULL;
}


//*********************************************************************************************************************************
//* Function name: createRunwayNode
//* Description:  creates a runway node, which is a list element that contains a runway struct and a pointer to the next runway on the list
//* Parameters: runway_num, runway_type
//* Return value: a pointer to the runwayNode
//*********************************************************************************************************************************
runwayNode* createRunwayNode(int runway_num, FlightType runway_type)
{
	runwayNode* pRunwayNode;
	pRunwayNode = (runwayNode*)malloc(sizeof(runwayNode)); 
	if (pRunwayNode == NULL)
		return NULL;

	//put the runway node parametes in the new runway node we created
	pRunwayNode->runway = createRunway(runway_num, runway_type);
	pRunwayNode->pNext = NULL;
	return pRunwayNode;
}


//*********************************************************************************************************************************
//* Function name: addRunway
//* Description: adds a new runway with the given runway details to the airport
//* Parameters: runway_num, runway_type
//* Return value: SUCCESS or FAILURE
//*********************************************************************************************************************************
Result addRunway(int runway_num, FlightType runway_type)
{
	//check if parameters are ok
	if (runway_num > MAX_ID || runway_num < 1)
		return FAILURE;

	runwayNode *pRunwayNode = createRunwayNode(runway_num, runway_type);

	if (airport->pNext == NULL) //if the airport is empty, it will point to the created runway because it's the only runway
	{
		airport->pNext = pRunwayNode;
	}
		
	else
	{
		runwayNode *tempCurrentRunwayNode = airport->pNext;
		while (tempCurrentRunwayNode) //go through all the runways in the runway list
		{
			if (tempCurrentRunwayNode->runway != NULL && tempCurrentRunwayNode->runway->runway_num == runway_num) //check if there is a runway with the given runway number already exists
			{
				destroyRunway(pRunwayNode->runway);
				free(pRunwayNode);
				return FAILURE;
			}
			tempCurrentRunwayNode = tempCurrentRunwayNode->pNext;
		}

		runwayNode *currentRunwayNode = airport->pNext;

		while (currentRunwayNode->pNext) //go to the end of the runway list
			{
				currentRunwayNode = currentRunwayNode->pNext;
			}

		currentRunwayNode->pNext = pRunwayNode;//add the new runway to the end of the runway list
	}
	
	return SUCCESS;
}


//*********************************************************************************************************************************
//* Function name: removeRunway
//* Description: removes a runway with the given runway num from the list
//* Parameters: runway_num
//* Return value: SUCCESS or FAILURE
//*********************************************************************************************************************************
Result removeRunway(int runway_num)
{
	if (airport->pNext == NULL) //if the given pointer is NULL, airport is empty and there are no runways
		return FAILURE;

	runwayNode *currentRunwayNode = airport->pNext;
	runwayNode *prevRunwayNode = NULL;

	while (currentRunwayNode) //check if there is a runway with the given runway num in the list
	{
		if (currentRunwayNode->runway != NULL && currentRunwayNode->runway->runway_num == runway_num)
			break;
		prevRunwayNode = currentRunwayNode;
		currentRunwayNode = currentRunwayNode->pNext;
	}

	if (currentRunwayNode == NULL) //if we got to the end of the list, there is no runway with the given runway num
		return FAILURE;
	else //remove the found right runway
	{
		if (prevRunwayNode != NULL)
			prevRunwayNode->pNext = currentRunwayNode->pNext;
		else
			airport->pNext = currentRunwayNode->pNext;

		pRunway currentRunway = currentRunwayNode->runway;
		destroyRunway(currentRunway);
		free(currentRunwayNode);
	}
	return SUCCESS;
}


//*********************************************************************************************************************************
//* Function name: addFlightToAirport
//* Description: adds a flight to the airport according to the given rules
//* Parameters: flight_num, flight_type, flight_dest, emergency
//* Return value: SUCCESS or FAILURE
//*********************************************************************************************************************************
Result addFlightToAirport(int flight_num, FlightType flight_type, char *flight_dest, BOOL emergency)
{
	//check if parameters are ok
	if (!flight_parameters(flight_num, flight_dest) || airport == NULL || airport->pNext == NULL)
		return FAILURE;

	pflight addedFlight = createFlight(flight_num, flight_type, flight_dest, emergency, false);//create a copy of the flight
	runwayNode *correctRunway = NULL;

	if (flightAlreadyExists(flight_num)) //check if the given flight already exists
	{
		destroyFlight(addedFlight);
		return FAILURE;
	}

	correctRunway = findCorrectRunway(flight_type); //find the correct runway for the flight
	if (correctRunway == NULL)
	{
		destroyFlight(addedFlight);
		return FAILURE;
	}

	addFlight(correctRunway->runway, addedFlight); //add the flight to the correct runway
	destroyFlight(addedFlight);

	return SUCCESS;
}


//*********************************************************************************************************************************
//* Function name: flightAlreadyExists
//* Description: checks if a flight with the given flight number already exists in the airport
//* Parameters: flight_num
//* Return value: true or false
//*********************************************************************************************************************************
bool flightAlreadyExists(int flight_num)
{
	runwayNode *currentRunway = airport->pNext;
	while (currentRunway) //go through all the runways
	{
		if (isFlightExists(currentRunway->runway, flight_num)) //return true if a flight with the given flight number already exists in the airport
			return true;
		currentRunway = currentRunway->pNext;
	}

	return false;
}


//*********************************************************************************************************************************
//* Function name: findCorrectRunway
//* Description: finds the correct runway to add the requested flight to, according to the flight type
//* Parameters: flight_type
//* Return value: a pointer to the runway node
//*********************************************************************************************************************************
runwayNode *findCorrectRunway(FlightType flight_type)
{
	runwayNode *currentRunway = airport->pNext;
	runwayNode *correctRunway = NULL;
	int minRunwayLen = -1;

	while (currentRunway) //check every existing runway
	{
		if (currentRunway->runway->runway_type == flight_type) //if the runway type is correct
		{
			if (minRunwayLen == -1) //if it is the first correct runway found
			{
				minRunwayLen = getFlightNum(currentRunway->runway); //initiate length to the current runway
				correctRunway = currentRunway; //save the current runway
			}

			if (getFlightNum(currentRunway->runway) <= minRunwayLen) //if the current correct runway length is equal or shorter that the minimun length so far
			{
				if (getFlightNum(currentRunway->runway) == minRunwayLen)
					if (currentRunway->runway->runway_num < correctRunway->runway->runway_num) //check which runway has the lowest ID
						correctRunway = currentRunway;
				if (getFlightNum(currentRunway->runway) < minRunwayLen)
				{
					minRunwayLen = getFlightNum(currentRunway->runway); //the correct runway is the current runway
					correctRunway = currentRunway;
				}
			}
		}
		currentRunway = currentRunway->pNext;
	}
	return correctRunway;
}


//*********************************************************************************************************************************
//* Function name: departFromAirport
//* Description: removes a flight from the airport, according to given rules
//* Parameters: none
//* Return value: SUCCESS or FAILURE
//*********************************************************************************************************************************
Result departFromAirport()
{
	//check status of airport
	if (airport == NULL || airport->pNext == NULL)
		return FAILURE;

	runwayNode *correctRunway = airport->pNext;
	bool sameEmergencyFlightsNum = false;

	//check if there is exactly 1 runway with max emergency flights, and if so then depart from it
	correctRunway = isMaxEmergRunway(&sameEmergencyFlightsNum);
	if (!sameEmergencyFlightsNum)
	{
		depart(correctRunway->runway);
		return SUCCESS;
	}
		
	else //if there is no 1 runway with max emergency flights, look for the longest runway and depart from it
	{
		correctRunway = runwayWithMaxFlights();
		depart(correctRunway->runway);
	}

	return SUCCESS;
}


//*********************************************************************************************************************************
//* Function name: isMaxEmergRunway
//* Description: checks which runway from the runway list contains the maximum amount of emergency flights
//* Parameters: sameEmergencyFlightsNum
//* Return value: a pointer to the runway node
//*********************************************************************************************************************************
runwayNode *isMaxEmergRunway(bool *sameEmergencyFlightsNum)
{
	int maxEmergFlights = -1; //a start value to signal that we haven't checked any runways yet
	runwayNode *currentRunway = airport->pNext;
	runwayNode *correctRunway = airport->pNext;
	bool isSameNum = false;

	while (currentRunway) //check every existing runway
	{
		int currentEmergNum = getEmergencyNum(currentRunway->runway);
		if (maxEmergFlights == -1) //if it is the first runway checked
		{
			maxEmergFlights = currentEmergNum;
			correctRunway = currentRunway;
		}

		else
		{
			if (currentEmergNum >= maxEmergFlights) //if the current runway has the same or larger number of emergency flights
			{
				if (currentEmergNum == maxEmergFlights) //if the current runway has same number of emergency flights than we choose flight to depart based on the second condition
				{
					isSameNum = true;
					currentRunway = currentRunway->pNext;
					continue;
				}
				else if (currentEmergNum > maxEmergFlights) //if the current runway has more emergency flights the current max, it is the correct runway so far
				{
					maxEmergFlights = currentEmergNum;
					correctRunway = currentRunway;
					isSameNum = false;
				}
			}
		}
		currentRunway = currentRunway->pNext;
	}

	//when we get here, we have the correct runway saved
	*sameEmergencyFlightsNum = isSameNum;
	return correctRunway;
}


//*********************************************************************************************************************************
//* Function name: runwayWithMaxFlights
//* Description: checks which runway from the runway list contains the maximum amount of flights
//* Parameters: none
//* Return value: a pointer to the runway node
//*********************************************************************************************************************************
runwayNode *runwayWithMaxFlights()
{
	runwayNode *currentRunway = airport->pNext;
	runwayNode *correctRunway = airport->pNext;
	int maxFlightNum = getFlightNum(currentRunway->runway);
	int currFlightNum = 0;

	while (currentRunway) //check every existing runway
	{
		currFlightNum = getFlightNum(currentRunway->runway);
		if (currFlightNum >= maxFlightNum) //if the current runway is as long or longer than the max runway so far
		{
			if (currFlightNum == maxFlightNum)
			{
				if (currentRunway->runway->runway_num < correctRunway->runway->runway_num) //if the current runway is as long as the current max runway check which ID is lower
				{
					correctRunway = currentRunway;
					maxFlightNum = currFlightNum;
				}
			}
			else if (currFlightNum > maxFlightNum) //if the current runway is longer than the max runway than IT IS the longest runway
			{
				correctRunway = currentRunway;
				maxFlightNum = currFlightNum;
			}
		}
		currentRunway = currentRunway->pNext;
	}

	//when we get here we have the correct runway saved
	return correctRunway;
}


//*********************************************************************************************************************************
//* Function name: changeDest
//* Description: changes the destination of every flight of the given original destination to the given new destination 
//* Parameters: origin_dest, new_dest
//* Return value: SUCCESS or FAILURE
//*********************************************************************************************************************************
Result changeDest(char* origin_dest, char* new_dest)
{
	//check if parameters are ok
	if (!(is_flight_dest_legal(origin_dest) && is_flight_dest_legal(new_dest)))
		return FAILURE;
	
	runwayNode *currentRunway = airport->pNext;
	flightNode *currentFlight = NULL;
	char* currentFlightDest = NULL;

	while (currentRunway) //go through all the runways in airport
	{
		currentFlight = currentRunway->runway->firstFlight;
		while (currentFlight) //go through all the flights in runway
		{
			currentFlightDest = currentFlight->flight_details->flight_dest;
			if (strcmp(currentFlightDest, origin_dest) == 0) //if the flight destination is the same as the given original destination, change it to the new destination
				strcpy(currentFlight->flight_details->flight_dest, new_dest);
			currentFlight = currentFlight->pNext;
		}
		currentRunway = currentRunway->pNext;
	}

	return SUCCESS;
}


//*********************************************************************************************************************************
//* Function name: is_flight_dest_legal
//* Description: checks if the flight destination is correct and legal
//* Parameters: flight_dest
//* Return value: true or false
//*********************************************************************************************************************************
bool is_flight_dest_legal(char* flight_dest)
{
	if (strlen(flight_dest) != 3)
		return false;
	unsigned int i;
	for (i=0; i < strlen(flight_dest); i++)
	{
		if (!(flight_dest[i] <= 'Z' && flight_dest[i] >= 'A'))
			return false;
	}
	return true;
}


//*********************************************************************************************************************************
//* Function name: delay
//* Description: delays a flight of a given destination according to the rules
//* Parameters: dest
//* Return value: FAILURE or SUCCESS
//*********************************************************************************************************************************
Result delay(char* dest)
{
	//check if parameters are ok
	if (!(is_flight_dest_legal(dest)))
		return FAILURE;

	runwayNode *currentRunway = airport->pNext;
	flightNode *currentFlight = currentRunway->runway->firstFlight;
	pflight currentFlightCopy = NULL;
	flightNode *currentFlightNext = NULL;
	char* currentFlightDest = NULL;
	int currentFlightNum = 0;

	while (currentRunway) //go through all the runways in airport
	{
		while (currentFlight) //go through all the flights in runway
		{
			currentFlightDest = currentFlight->flight_details->flight_dest;
			if (strcmp(currentFlightDest, dest) == 0) //if the flight destination is the same as the given original destination, relocate it
			{
				if (!currentFlight->flight_details->isDelayed)
				{
					currentFlightCopy = createFlight(currentFlight->flight_details->flight_num, currentFlight->flight_details->flight_type, currentFlight->flight_details->flight_dest, currentFlight->flight_details->emergency, true); //create a copy of the flight
					currentFlightNext = currentFlight->pNext; //save the pointer to the next flight
					currentFlightNum = currentFlight->flight_details->flight_num; //save the flight number
					removeFlight(currentRunway->runway, currentFlightNum);
					addFlight(currentRunway->runway, currentFlightCopy);
					destroyFlight(currentFlightCopy);
					currentFlight = currentFlightNext; //advance to the next flight
				}
				else
				currentFlight = currentFlight->pNext;
			}
			else
				currentFlight = currentFlight->pNext;
		}

		currentRunway = currentRunway->pNext; //advance to the next runway
		if (currentRunway)
			currentFlight = currentRunway->runway->firstFlight;
	}

	changeIsDelayed(); //nullify this field for all the flights
	return SUCCESS;
}


//*********************************************************************************************************************************
//* Function name: changeIsDelayed
//* Description: sets 'false' in the isDelayed field of every existing flight
//* Parameters: none
//* Return value: none
//*********************************************************************************************************************************
void changeIsDelayed()
{
	runwayNode *currentRunway = airport->pNext;
	flightNode *currentFlight = currentRunway->runway->firstFlight;
	
	while (currentRunway) //go through all the runways in airport
	{
		while (currentFlight) //go through all the flights in runway
		{
			currentFlight->flight_details->isDelayed = false;
			currentFlight = currentFlight->pNext;
		}
		currentRunway = currentRunway->pNext;
		if (currentRunway)
			currentFlight = currentRunway->runway->firstFlight;
	}
}


//*********************************************************************************************************************************
//* Function name: printAirport
//* Description: prints the airport status, including all the existing runways and flights awaiting
//* Parameters: none
//* Return value: none
//*********************************************************************************************************************************
void printAirport()
{
	runwayNode *currentRunway = airport->pNext;

	printf("Airport status:\n");
	while (currentRunway) //go through all the runways in airport and print them
	{
		printRunway(currentRunway->runway);
		currentRunway = currentRunway->pNext;
	}
	printf("\n");
}


//*********************************************************************************************************************************
//* Function name: destroyAirport
//* Description: deletes all of the runways, flights ,and their nodes in the airport
//* Parameters: none
//* Return value: none
//*********************************************************************************************************************************
void destroyAirport()
{
	runwayNode *currentRunwayNode = airport->pNext; //start from the first runway
	if (currentRunwayNode == NULL) //if the given pointer is NULL, airport is empty
		return;

	while (currentRunwayNode) //go over every runway and destroy it
	{
		runwayNode *prevRunwayNode = currentRunwayNode;
		pRunway currentRunway = currentRunwayNode->runway;
		currentRunwayNode = currentRunwayNode->pNext;
		destroyRunway(currentRunway);
		free(prevRunwayNode);
	}
	free(airport);
}