#ifndef WORKTREE_H
#define WORKTREE_H

#define TAILLE_MAX 256
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "workfile.h"
#include "file_utility.h"


typedef struct {
	WorkFile* tab;
	int size;
	int n;
} WorkTree;


WorkTree* initWorkTree();

void freeWorkTree(WorkTree* wt);

int inWorkTree(WorkTree* wt, char* name);

int isWorkTree(char* hash);

int appendWorkTree(WorkTree* wt, char* name, char* hash, mode_t mode);

char* wtts(WorkTree* wt);

WorkTree* stwt(char* ch);

int wttf(WorkTree* wt, char* file);

WorkTree* ftwt(char* file);

#endif