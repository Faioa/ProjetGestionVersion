#ifndef INSTANTANEWT_H
#define INSTANTANEWT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "worktree.h"
#include "instantane.h"


char* blobWorkTree(WorkTree* wt);

char* saveWorkTree(WorkTree* wt, char* path);

void restoreWorkTree(WorkTree* wt, char* path);

#endif