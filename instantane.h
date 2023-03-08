#ifndef INSTANTANE_H
#define INSTANTANE_H

#include "list_str.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

List* listdir(char* root_dir);

char* sha256file(char* file);

int hashFile(char* source, char* dest);

int file_exists(char* file);

void cp(char* to, char* from);

char* hashToPath(char* hash);

void blobFile(char* file);

char* dirName(char* path);

char* baseName(char* path);

#endif