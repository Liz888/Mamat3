#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ex2.h"
#include "flight.h"
#include "runway.h"

flightNode* createFlightNode(pflight flight_details, struct list_element* pNext) //func creates a flight node, an node in the linked list of flights
{
	flightNode* pflightNode;
	pflightNode = (flightNode*)malloc(sizeof(flightNode)); //dynamic memory allocation
	if (pflightNode == NULL) //checks if dynamic memory allocation was successful
		return NULL;
	pflightNode->flight_details = flight_details;
	pflightNode->pNext = pNext;
	return pflightNode;
}

pRunway createRunway(int runway_num, FlightType runway_type) //func creates a runway with the given parameters
{
	if (runway_num > MAX_ID || runway_num < 1) //checks if parameters are correct
		return NULL;
	pRunway r;
	r = (pRunway)malloc(sizeof(runway)); //dynamic memory allocation
	if (r == NULL) //checks if dynamic memory allocation was successful
		return NULL;
	//puts the runway parametes in the new runway we created
	r->runway_num = runway_num;
	r->runway_type = runway_type;
	r->firstFlight = NULL; //the linked list of flights is empty now
	return r;
}

void destroyRunway(pRunway p) //func receives a pointer to a runway and destroys it
{
	if (p == NULL) //if the given pointer points to NULL, runway does not exist
		return;

	flightNode *currentNode = p->firstFlight;
	while (currentNode) //we destroy every flight node the runway contains
	{
		flightNode *prevNode = currentNode;
		pflight currentFlight = currentNode->flight_details;
		currentNode = currentNode->pNext;
		destroyFlight(currentFlight);
		free(prevNode);
	}
	free(p); //destroy the runway
}

BOOL isFlightExists(pRunway r, int flight_number) //func checks if the flight with the given number exists in the given runway, if it exists it returns true, if not or if the parameters are wrong it returns false
{
	if (r == NULL || ((flight_number > MAX_ID) || (flight_number < 1))) //checks if parameters are correct
		return FALSE;
	flightNode *currentFlight = r->firstFlight;
	int currentFlightNum;
	while (currentFlight) //go through all the flights in the runway
	{
		currentFlightNum = currentFlight->flight_details->flight_num;
		if (currentFlightNum == flight_number) //check if the number of the given runway is the requested number
			return TRUE;
		currentFlight = currentFlight->pNext;
	}
	return FALSE;
}

int getFlightNum(pRunway r) //func tells how many flights there are in the given runway
{
	if (r == NULL) //if the given pointer points to NULL, runway does not exist
		return -1;
	int counter = 0;
	flightNode *currentFlight = r->firstFlight;
	while (currentFlight) //go through all the flights in the given runway and count them
	{
		counter++;
		currentFlight = currentFlight->pNext;
	}
	return counter;
}

int getEmergencyNum(pRunway r) //func tells how many emergency flights there are in the given runway
{
	if (r == NULL) //if the given pointer points to NULL, runway does not exist
		return -1;
	int counter = 0;
	BOOL isEmergency;
	flightNode *currentFlight = r->firstFlight;
	while (currentFlight) //go through all the flights in the given runway
	{
		isEmergency = currentFlight->flight_details->emergency;
		if (isEmergency) //count ounly the emergency flights the runway contains
			counter++;
		currentFlight = currentFlight->pNext;
	}
	return counter;
}

Result addFlight(pRunway r, pflight f) //func adds a flight to the runway according to the given rules
{
	//check if parameters are ok
	if (r == NULL || f == NULL)
		return FAILURE;
	if (f->flight_type != r->runway_type)
		return FAILURE;

	//stand on the first flight
	flightNode *currentFlight = r->firstFlight;
	
	//check if flight num already exists
	while (currentFlight)
	{
		if ((currentFlight->flight_details->flight_num) == (f->flight_num))
			return FAILURE;
		currentFlight = currentFlight->pNext;
	}

	//create a copy of the given flight
	flightNode *flightCopy = createFlightNode(f, NULL); 
	flightCopy->flight_details = (createFlight(f->flight_num, f->flight_type, f->flight_dest, f->emergency, f->isDelayed));
	if (flightCopy == NULL)
		return FAILURE;

	//if the given flight is an emergency flight, put it in the end of the line for the emergency flights
	if (flightCopy->flight_details->emergency)
	{
		int emergencyFlights = getEmergencyNum(r);
		if (emergencyFlights > 0) 
		{
			flightNode *currentEmerg = r->firstFlight;

			if (emergencyFlights == 1) //if there is only 1 emergency flight
			{
				flightNode *temp_pNext = currentEmerg->pNext;
				currentEmerg->pNext = flightCopy;
				flightCopy->pNext = temp_pNext;
				return SUCCESS;
			}

			emergencyFlights--;
			while (emergencyFlights) //advance to the last emergency flight
			{
				currentEmerg = currentEmerg->pNext;
				emergencyFlights--;
			}
			//put the given flight in the right place in the list
			flightNode *temp_pNext = currentEmerg->pNext;
			currentEmerg->pNext = flightCopy;
			flightCopy->pNext = temp_pNext;
			currentEmerg = currentEmerg->pNext; //??????????????????????????????
		}
		else //if it is the first emergency flight, put it at the top of the list
		{
			flightCopy->pNext = r->firstFlight;
			r->firstFlight = flightCopy;
		}
	}
	else //if it is not an emergency flight put it at the end of the list
	{
		int flightAmount = getFlightNum(r);
		if (flightAmount > 0)
		{
			flightNode *currentFl = r->firstFlight;
			flightAmount--;
			while (flightAmount) //advance to the last flight
			{
				currentFl = currentFl->pNext;
				flightAmount--;
			}
			currentFl->pNext = flightCopy; //put the flight at the end
		}
		else
		{
			r->firstFlight = flightCopy; //if the list is empty put the flight at the top
		}
	}
	return SUCCESS;
}

Result removeFlight(pRunway r, int flight_num) //the func removes a flight with the given flight number from the runway
{
	//check if parameters are ok
	if (r == NULL || r->firstFlight == NULL)
		return FAILURE;
	if (flight_num > MAX_ID || flight_num < 1)
		return FAILURE;

	flightNode *currentFlight = r->firstFlight;
	flightNode *prevFlight = NULL;

	//find the flight with the given number
	while (currentFlight)
	{
		if ((currentFlight->flight_details->flight_num) == flight_num)
			break;
		prevFlight = currentFlight;
		currentFlight = currentFlight->pNext;
		if (currentFlight == NULL)
			return FAILURE;
	}
	//save the necessary values
	flightNode *temp_pNext = currentFlight->pNext;
	if (prevFlight != NULL)
		prevFlight->pNext = temp_pNext;
	else
		r->firstFlight = temp_pNext;

	//delete the flight
	destroyFlight(currentFlight->flight_details);
	free(currentFlight);
	return SUCCESS;
}

Result depart(pRunway r) //the func removes the first flight from the runway
{
	//check if parameters are ok
	if (r == NULL || r->firstFlight == NULL)
		return FAILURE;

	flightNode *currentFlight = r->firstFlight; //stand on the first flight
	flightNode *temp_pNext = currentFlight->pNext; //save the pointer to the 2nd flight
	r->firstFlight = temp_pNext; //make the 2nd flight be the top flight

	destroyFlight(currentFlight->flight_details);
	free(currentFlight);
	return SUCCESS;
}

void printRunway(pRunway r) //the func prints the runway and all of the flights it contains
{
	//check if parameter is ok
	if (r == NULL)
		return;

	char *runway_type; //convert the runway type for printf
	if (r->runway_type)
		runway_type = "international";
	else
		runway_type = "domestic";

	printf("Runway %d %s\n", r->runway_num, runway_type);

	int flight_amount = getFlightNum(r);
	printf("%d flights are waiting:\n", flight_amount);

	flightNode *currentFlightNode = r->firstFlight; 
	while (currentFlightNode) //print the flights one by one
	{
		pflight currentFlight = currentFlightNode->flight_details;
		printFlight(currentFlight);
		currentFlightNode = currentFlightNode->pNext;
	}
}
