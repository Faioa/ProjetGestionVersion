#ifndef FUSION_H
#define FUSION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ref.h"
#include "commit.h"
#include "worktree.h"
#include "branch.h"
#include "hash_utility.h"
#include "list_utility.h"
#include "worktree.h"

 WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts);

  List* merge(char* remote_branch, char* message);

  void createDeletionCommit(char* branch, List* conflicts, char* message);

  #endif