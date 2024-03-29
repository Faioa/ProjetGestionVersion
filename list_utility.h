#ifndef LIST_UTILITY_H
#define LIST_UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct cell {
	char* data;
	struct cell* next;
}Cell;

typedef Cell* List;


List* initList();

Cell* buildCell(char* ch);

void insertFirst(List* L, Cell* C);

char* ctos(Cell* c);

char* ltos(List* L);

Cell* listGet(List* L, int i);

Cell* searchList(List* L, char* str);

List* stol(char* s);

void ltof(List* L, char* path);

List* ftol(char* path);

void freeCell(Cell* cell);

void freeList(List* list);

void afficheList(List* list);

#endif