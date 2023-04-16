#include "branch.h"

void initBranch(){
	FILE*f=fopen(".current_branch","w");
	fputs("master",f);
	fclose(f);
}

int branchExists(char* branch){
	int res = 0;

	List*refs=listdir(".refs");

	if (searchList(refs,branch) != NULL) {
		res = 1;
	}

	freeList(refs);
	return res;
}

void createBranch(char* branch){
	char *hash=getRef("HEAD");
	createUpdateRef(branch,hash);
	free(hash);
}

char * getCurrentBranch(){

	if (file_exists(".current_branch") != 1) {
		fprintf(stderr, "Le fichier .current_branch n'existe pas !\n");
		return NULL;
	}

	FILE*f=fopen(".current_branch","r");
	if(f == NULL){
		fprintf(stderr, "Erreur lors de l'ouverture du fichier .current-branch pour la fonction getCurrentBranch !\n");
		exit(1);
	}

	char*buff=malloc(sizeof(char)*1000);
	fscanf(f,"%s",buff);

	fclose(f);

	return buff;

	fclose(f);
}

char *hashToPathCommit(char* hash){
	char * buff=malloc(sizeof(char)*1000);
	if (buff == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation de memoire pour la fonction hashToPathCommit !\n");
		exit(1);
	}
	
	char* tmp = hashToPath(hash);
	sprintf(buff,"%s.c", tmp);

	free(tmp);

	return buff;
}

void printBranch(char* branch){
	//recupérer le fichier contenant la référence(le hash) du commit
	char* commit_hash = getRef(branch);

	//conversion en commit
	char* path_commit = hashToPathCommit(commit_hash);
	Commit* c = ftc(path_commit);

	while(c != NULL){
		char* c_hash_tmp = commitGet(c, "message");
		if (c_hash_tmp != NULL){
			printf("%s -> %s\n", commit_hash, c_hash_tmp);
			free(c_hash_tmp);
		}else{
			printf("%s\n", commit_hash);
		}
		c_hash_tmp = commitGet(c, "predecessor");
		if(c_hash_tmp != NULL){
			freeCommit(c);
			c = ftc(path_commit);
		}else{
			freeCommit(c);
			c = NULL;
		}
	}

	free(commit_hash);
	free(path_commit);
}

List* branchList(char* branch){
	List*l=initList();
	char * commit_hash=getRef(branch);
	char* path = hashToPathCommit(commit_hash);
	Commit * c=ftc(path);

	while( c != NULL ){
		insertFirst(l, buildCell(commit_hash));
		free(commit_hash);
		commit_hash = commitGet(c,"predecessor");

		if(commit_hash != NULL){
			char* hash = hashToPathCommit(commit_hash);
			freeCommit(c);
			c=ftc(hash);
			free(hash);
		}else{
			freeCommit(c);
			c=NULL;
		}
	}

	free(path);

	return l;
}

List * getAllCommits(){
	List* result = initList();
	List* all = listdir(".refs");
	Cell* cellule = *all;

	while (cellule != NULL){
		if(cellule->data[0] == '.') {
			cellule = cellule -> next;
			continue;
		}

		List* br_list = branchList(cellule->data);

		Cell* cellule2 = *br_list;
		while(cellule2 != NULL){
			if(searchList(result, cellule2->data) == NULL){
				insertFirst(result, buildCell(cellule2->data));
			}
			cellule2 = cellule2->next;
		}

		freeList(br_list);
		
		cellule = cellule->next;
	}

	freeList(all);
	
	return result;
}