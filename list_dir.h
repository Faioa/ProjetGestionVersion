#ifndef LIST_DIR_H
#define LIST_DIR_H

#include "list_str.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

List* listdir(char* root_dir);

int file_exists(char* file);

void cp(char* to, char* from);

char* hashToPath(char* hash);

void blobFile(char* file);

#endif