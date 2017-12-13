#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include <assert.h>
#include "list.h"
#include "airport.h"


typedef struct Node_
{
	PElem pElem;
	struct Node_ *pNext;
} Node;


typedef struct List_
{
	Node* pFirstNode;

	//Function pointers
	CLONE_FUNC cloneFunc;
	DESTROY_FUNC destroyFunc;
} List;


Node* iterator = NULL;


PList ListCreate(CLONE_FUNC cloneFunc, DESTROY_FUNC destroyFunc)
{
	PList pList = (PList)malloc(sizeof(List));

	if (pList != NULL)
	{
		pList->pFirstNode = NULL;
		pList->cloneFunc = cloneFunc;
		pList->destroyFunc = destroyFunc;
	}

	return pList;
}


Node *NodeCreate(PElem pElem, Node *pNext)
{
	Node *pNode = (Node*)malloc(sizeof(Node));
	if (pNode == NULL)
		return NULL;

	pNode->pElem = pElem;
	pNode->pNext = pNext;

	return pNode;
}


void ListDestroy(PList pList)
{
	if (pList == NULL)
		return;

	if (pList->pFirstNode != NULL)
	{
		Node *currentNode = pList->pFirstNode;
		Node *nextNode = NULL;

		while (currentNode)
		{
			nextNode = currentNode->pNext;
			pList->destroyFunc(currentNode->pElem);
			free(currentNode);
			currentNode = nextNode;
		}
	}
	free(pList);
}


Result ListAdd(PList pList, PElem pElem)
{
	if (pList == NULL || pElem == NULL)
		return FAILURE; //change to FAIL!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	PElem pElemCopy = pList->cloneFunc(pElem);
	Node *pNewNode = NodeCreate(pElemCopy, pList->pFirstNode);

	pList->pFirstNode = pNewNode;
	iterator = NULL;

	return SUCCESS;
}


PElem ListGetFirst(PList pList)
{
	if (pList == NULL || pList->pFirstNode == NULL)
		return NULL;

	iterator = pList->pFirstNode;
	return iterator->pElem;
}


PElem ListGetNext(PList pList)
{
	if (pList == NULL || pList->pFirstNode == NULL || iterator == NULL)
		return NULL;

	iterator = iterator->pNext;
	if (iterator == NULL)
		return NULL;

	return iterator->pElem;
}


int ListGetSize(PList pList)
{
	if (pList == NULL || pList->pFirstNode == NULL)
		return 0;

	Node* countIterator = pList->pFirstNode;
	int counter = 0;

	while (countIterator)
	{
		counter++;
		countIterator = countIterator->pNext;
	}
	
	return counter;
}


Result ListRemove(PList pList)
{
	if (pList == NULL || pList->pFirstNode == NULL || iterator == NULL)
		return FAILURE; //change to FAIL!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	Node* currentNode = pList->pFirstNode;

	if (iterator == pList->pFirstNode)
	{
		pList->pFirstNode = iterator->pNext;
		pList->destroyFunc(iterator->pElem);
		free(iterator);
		iterator = NULL;
		return SUCCESS;
	}

	while (currentNode->pNext != iterator)
	{
		currentNode = currentNode->pNext;
	}
	currentNode->pNext = iterator->pNext;

	pList->destroyFunc(iterator->pElem);
	free(iterator);
	iterator = NULL;

	return SUCCESS;
}


pflight createFlightCopy(pflight p) //func receives flight details and creates a new flight using dynamic memory allocation
{
	pflight pCopy;

	pCopy = (pflight)malloc(sizeof(flight));
	if (pCopy == NULL)
		return NULL;

	//put the flight parametes in the new flight we created
	pCopy->flight_num = p->flight_num;
	pCopy->flight_type = p->flight_type;
	pCopy->emergency = p->emergency;
	pCopy->isDelayed = p->isDelayed;

	pCopy->flight_dest = (char*)malloc(4 * sizeof(char));
	if (pCopy->flight_dest == NULL)
		return NULL;
	strcpy(pCopy->flight_dest, p->flight_dest);

	return pCopy;
}



int main()
{
	pflight flight1 = createFlight(1, 'I', "TLV", TRUE, false);
	pflight flight2 = createFlight(2, 'D', "ETH", FALSE, false);

	PList list = ListCreate(createFlightCopy, destroyFlight);
	ListAdd(list, flight1);
	//ListAdd(list, flight2);
	iterator = list->pFirstNode;
	//PElem nextElem = ListGetNext(list);
	BOOL is = ListRemove(list);

	return 0;
}
