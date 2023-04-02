#ifndef BRANCH_H
#define BRANCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ref.h"



void initBranch();

int branchExists(char* branch);


void createBranch(char* branch);

char* getCurrentBranch();

char *hashToPathCommit(char * hash);

void printBranch(char* branch);

List* branchList(char* branch);

List* getAllCommits();

#endif


/*List * branchList(char * branch){
	List*l=initList();
	char * commit_hash=getRef(branch);
	Commit * c=ftc(hashToPathCommit(commit_hash));
	while( c != NULL ){
		Cell*cellule=buildCell(commit_hash);
		insertFirst(l,cellule);
		commit_hash=commitGet(c,"predecessor");
		c=ftc(hashToPathCommit(commit_hash));
	}
	return l;
}*/
