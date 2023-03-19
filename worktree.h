#ifndef WORKTREE_H
#define WORKTREE_H

#define TAILLE_MAX 256
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "workfile.h"


typedef struct {
	WorkFile* tab;
	int size;
	int n;
} WorkTree;


WorkTree* initWorkTree();

void freeWorkTree(WorkTree* wt);

int inWorkTree(WorkTree* wt, char* name);

int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode);

char* wtts(WorkTree* wt);

WorkTree* stwt(char* ch);

int wttf(WorkTree* wt, char* file);

WorkTree* ftwt(char* file);

#endif