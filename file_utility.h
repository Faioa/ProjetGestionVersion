#ifndef FILE_UTILITY_H
#define FILE_UTILITY_H

#include "list_utility.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>


List* listdir(char* root_dir);

int file_exists(char* file);

int is_regular_file(char* file);

int is_directory(char* file);

void cp(char* to, char* from);

char* dirName(char* path);

char* baseName(char* path);

int getChmod(const char* path);

void setMode(int mode, char* path); 

#endif