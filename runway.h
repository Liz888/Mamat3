#ifndef _runway_h
#define _runway_h
#include "flight.h"
#include "ex2.h"

struct list_element
{
	pflight flight_details;
	struct list_element* pNext;
};

typedef struct list_element flightNode;

typedef struct runway
{
	int runway_num;
	FlightType runway_type;
	flightNode *firstFlight;
} runway;

typedef struct runway* pRunway;

pRunway createRunway(int runway_num, FlightType runway_type);
flightNode* createFlightNode(pflight flight_details, struct list_element* pNext);
void destroyRunway(pRunway p);
BOOL isFlightExists(pRunway p, int flight_number);
int getFlightNum(pRunway r);
int getEmergencyNum(pRunway r);
Result addFlight(pRunway r, pflight f);
Result removeFlight(pRunway r, int flight_num);
Result depart(pRunway r);
void printRunway(pRunway r);

#endif