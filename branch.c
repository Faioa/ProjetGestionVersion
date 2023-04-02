#include "branch.h"
#include "file_utility.h"

void initBranch(){
	FILE*F=fopen(".current_branch","w");
	fputs("matser",f);
	fclose(f);
}

int branchExists(char * branch){
	List*refs=listdir(branch);
	
	return searchList(refs,branch) != NULL;
}
void createBranch(char * branch){
	char *hash=getRef("HEAD");
	createUpdateRef(branch,hash);
}
char * getCurrentBranch(){
	FILE*f=fopen(".current_branch","r");
	char*buff=malloc(sizeof(char)*100);
	fsacnf(f,"%s",buff);
	return buff,
}
