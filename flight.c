#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ex2.h"
#include "flight.h"

pflight createFlight(int flight_num, FlightType flight_type, char *flight_dest, BOOL emergency, bool isDelayed) //func receives flight details and creates a new flight using dynamic memory allocation
{
	pflight p;
	if (!flight_parameters(flight_num, flight_dest)) //check if parameters are correct
		return NULL;

	p = (pflight)malloc(sizeof(flight)); 
	if (p == NULL)
		return NULL;

	//put the flight parametes in the new flight we created
	p->flight_num = flight_num;
	p->flight_type = flight_type;
	p->emergency = emergency;
	p->isDelayed = isDelayed;

	p->flight_dest = (char*)malloc(4 * sizeof(char));
	if (p->flight_dest == NULL)
		return NULL;
	strcpy(p->flight_dest, flight_dest);

	return p;
}

BOOL flight_parameters(int flight_num, char* flight_dest) //func checks if flight parameters are correct
{
	if (flight_num > MAX_ID || flight_num < 1)
		return false;
	if (strlen(flight_dest) != 3)
		return false;
	unsigned int i;
	for (i = 0; i < strlen(flight_dest); i++)
	{
		if (!(flight_dest[i] <= 'Z' && flight_dest[i] >= 'A'))
			return false;
	}
	return true;
}

void destroyFlight(pflight p) //func receives a pointer to a flight and destroys it
{
	if (p == NULL)
		return;
	if (p->flight_dest != NULL)
		free(p->flight_dest);
	free(p);
}

void printFlight(pflight p) //func prints the flight details
{
	char type;
	if (p->flight_type)
		type = 'I';
	else
		type = 'D';

	char emerg;
	if (p->emergency)
		emerg = 'E';
	else
		emerg = 'R';

	printf("Flight %d %c %s %c\n", p->flight_num, type, p->flight_dest, emerg);
}

