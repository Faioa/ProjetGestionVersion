#ifndef CHECK_H
#define CHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ref.h"
#include "commit.h"
#include "worktree.h"
#include "branch.h"
#include "hash_utility.h"

void restoreCommit(char* hash commit);

void myGitCheckoutBranch(char* branch);

List* filterList(List* L, char* pattern);

void myGitCheckoutCommit(char* pattern);

#endif