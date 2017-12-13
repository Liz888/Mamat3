#ifndef LIST_H_
#define LIST_H_
#include "ex2.h" //remove !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

typedef struct List_* PList;
typedef void* PElem;
//typedef enum{FAIL, SUCCESS} Result;

/*User functions*/
typedef PElem (*CLONE_FUNC)(PElem);
typedef void (*DESTROY_FUNC)(PElem);

/*Interface functions*/
PList ListCreate(CLONE_FUNC, DESTROY_FUNC);
void ListDestroy(PList);
Result ListAdd(PList, PElem);
PElem ListGetFirst(PList);
PElem ListGetNext(PList);

Result ListRemove(PList);
int ListGetSize(PList);

#endif