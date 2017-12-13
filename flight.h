#ifndef _flight_h
#define _flight_h
#include "ex2.h"

typedef struct flight
{
	int flight_num;
	FlightType flight_type;
	char* flight_dest;
	BOOL emergency;
	bool isDelayed;
} flight;
typedef struct flight* pflight;

pflight createFlight(int flight_num, FlightType flight_type, char* flight_dest, BOOL emergency, bool isDelayed);
BOOL flight_parameters(int flight_num, char* flight_dest);
void printFlight(pflight p);
void destroyFlight(pflight p);

#endif