#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "file_utility.h"
#include "hash_utility.h"

#ifndef COMMIT_H
#define COMMIT_H
#define TAILLE_C 256

typedef struct key_value_pair{
	char* key;
	char *value;
}kvp;

typedef struct hash_table{
	kvp** T;
	int n;
	int size;
}HashTable;

typedef HashTable Commit;

kvp* createKeyVal(char* key, char* val);

void freeKeyVal(kvp* kv);

char* kvts(kvp* k);

kvp* stkv(char* str);

Commit* initCommit();

unsigned long hash(unsigned char *str);

void commitSet(Commit* c, char* key, char* value);

Commit* createCommit(char* hash);

char* commitGet(Commit* c, char* key);

char* cts(Commit* c);

Commit* stc(char* ch);

void ctf(Commit* c, char* file);

Commit* ftc(char* file);

void freeCommit(Commit *c);

char* blobCommit(Commit* c);

#endif
