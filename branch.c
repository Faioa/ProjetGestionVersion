#include "branch.h"
#include "file_utility.h"

void initBranch(){
	FILE*F=fopen(".current_branch","w");
	fputs("master",f);
	fclose(f);
}

int branchExists(char * branch){
	int res = 0;

	List*refs=listdir(".refs");

	if (searchList(refs,branch) != NULL) {
		res = 1;
	}

	freeList(refs);
	return res;
}

void createBranch(char * branch){
	char *hash=getRef("HEAD");
	createUpdateRef(branch,hash);
}
char * getCurrentBranch(){
	FILE*f=fopen(".current_branch","r");
	char*buff=malloc(sizeof(char)*1000);
	fscanf(f,"%s",buff);
	return buff;
}

char *hashToPathCommit(char * hash){
	char * buff=malloc(sizeof(char)*1000);
	sprintf(buff,"%s.c",hashToPath(hash));
	return buff;
}

void printBranch(char* branch){
	//recupérer le fichier contenant la référence(le hash) du commit
	char*commit_hash=getRef(branch);
	//conversion en commit
	Commit * c=ftc(hashToPathCommit(commit_hash));
	while(c != NULL){
		if (commitGet(c,"message") != NULL){
			printf("%s -> %s\n",commit_hash,commitGet(c,"message"));
		}else{
			printf("%s \n",commit_hash);
		}
		if(commitGet(c,"predecessor") != NULL){
			commit_hash=commitGet(c,"predecessor");
			c=ftc(hashToPathCommit(commit_hash));
		}else{
			c=NULL;
		}
	}
}

List* branchList(char * branch){
	List*l=initList();
	char * commit_hash=getRef(branch);
	Commit * c=ftc(hashToPathCommit(commit_hash));
	while( c != NULL ){
		Cell*cellule=buildCell(commit_hash);
		insertFirst(l,cellule);
		if(commitGet(c,"predecessor") != NULL){
			commit_hash=commitGet(c,"predecessor");
			c=ftc(hashToPathCommit(commit_hash));
		}else{
			c=NULL;
		}
	}
	return l;
}

List* getAllCommits() {

}