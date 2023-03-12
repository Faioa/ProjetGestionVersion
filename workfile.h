#ifndef WORKFILE_H
#define WORKFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char * name;
	char * hash;
	int mode;
} Workfile;




WorkFile* createWorkFile(char* name);
 
char* wfts(WorkFile* wf);

WorkFile* stwf(char* ch);









#endif
