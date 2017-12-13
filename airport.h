#ifndef _airport_h
#define _airport_h
#include "runway.h"
#include "flight.h"
#include "ex2.h"

struct airport_element
{
	pRunway runway;
	struct airport_element* pNext;
};

typedef struct airport_element runwayNode;

void createAirport();
runwayNode* createRunwayNode(int runway_num, FlightType runway_type);
Result addRunway(int runway_num, FlightType runway_type);
Result removeRunway(int runway_num);
Result addFlightToAirport(int flight_num, FlightType flight_type, char *flight_dest, BOOL emergency);
bool flightAlreadyExists(int flight_num);
runwayNode *findCorrectRunway(FlightType flight_type);
Result departFromAirport();
runwayNode *isMaxEmergRunway(bool *sameEmergencyFlightsNum);
runwayNode *runwayWithMaxFlights();
Result changeDest(char* origin_dest, char* new_dest);
bool is_flight_dest_legal(char* flight_dest);
Result delay(char* dest);
void changeIsDelayed();
void printAirport();
void destroyAirport();

#endif