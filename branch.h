#ifndef BRANCH_H
#define BRANCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ref.h"
#include "commit.h"
#include "list_utility.h"

void initBranch();

int branchExists(char* branch);

void createBranch(char* branch);

char* getCurrentBranch();

char *hashToPathCommit(char * hash);

void printBranch(char* branch);

List* branchList(char* branch);

List* getAllCommits();

List * branchList(char * branch);

List* getAllCommits();

#endif