#ifndef REF_H
#define REF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "worktree.h"
#include "file_utility.h"
#include "hash_utility.h"
#include "instantanewt.h"
#include "worktree.h"
#include "commit.h"


void initRefs();

void createUpdateRef(char* ref_name, char* hash);

void deleteRef(char* ref_name);

char* getRef(char* ref_name);

void myGitAdd(char* file_or_folder);

void myGitCommit(char* branch_name, char* message);

#endif