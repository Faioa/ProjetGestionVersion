#ifndef WORKFILE_H
#define WORKFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
	char * name;
	char * hash;
	mode_t mode;
} WorkFile;


WorkFile* createWorkFile(char* name);

void freeWorkFile(WorkFile* w);
 
char* wfts(WorkFile* wf);

WorkFile* stwf(char* ch);

#endif