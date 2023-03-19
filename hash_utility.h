#ifndef HASH_UTILITY_H
#define HASH_UTILITY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


char* hashToPath(char* hash);

char* sha256file(char* file);

int hashFile(char* source, char* dest);

#endif