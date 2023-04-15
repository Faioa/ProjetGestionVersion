#include "ref.h"


void initRefs(){
	char cmd[256];
	sprintf(cmd,"mkdir -p .refs && touch .refs/master .refs/HEAD");
	system(cmd);

}

void createUpdateRef(char* ref_name, char* hash){
	
	char cmd[256], *name = baseName(ref_name);
	sprintf(cmd,"echo %s > .refs/%s", hash, name);
	system(cmd);
	
	free(name);
}

void deleteRef(char* ref_name){
	char cmd[256], *name = baseName(ref_name);
	sprintf(cmd,"rm -f .refs/%s", name);
	system(cmd);
	
	free(name);
}
char* getRef(char* ref_name){
	
	char*result=malloc(sizeof(char)*256);
	memset(result, 0, 256);
	
	char buffer[256];
	
	sprintf(buffer,".refs/%s",ref_name);
	
	if(file_exists(buffer)==0)return NULL;
	
	FILE*fp=fopen(buffer,"r");
	
	if (fp == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s dans la fonction getRef\n", buffer);
		exit(1);
	}
	
	fgets(result,256,fp);
	
	fclose(fp);
	
	return result;
}

void myGitAdd(char* file_or_folder){

	if (! file_exists(".add")) {
		system("touch .add");
	}
	
	WorkTree*wt=ftwt(".add");

	if(file_exists(file_or_folder) == 1){
		appendWorkTree(wt,file_or_folder,NULL,0);
		wttf(wt, ".add");
	}

	freeWorkTree(wt);
}


void myGitCommit(char* branch_name, char * message){

	if(is_directory(".refs") != 0){
		fprintf(stderr,"Initialiser d'abord les références du projet\n");
		return ;
	}
	
	if(file_exists(branch_name) != 0){
		fprintf(stderr,"La branche n'existe pas\n");
		return ;
	}
	
	char *test=sha256file(".refs/HEAD");
	
	char buffer[256];
	sprintf(buffer,".refs/%s",branch_name);
	char *test2=sha256file(buffer);
	
	if(strcmp(test,test2)!=0){
		fprintf(stderr,"HEAD doit pointer sur le dernier commit de la branche\n");
		free(test);
		free(test2);
		return ;
	}

	free(test);
	free(test2);
	
	WorkTree*wt=ftwt(".add");
	
	system("rm -f .add");
	
	char *hash=saveWorkTree(wt,".");
	
	Commit *c=initCommit();
	
	commitSet(c,"tree",hash);

	free(hash);
	
	FILE* fp=fopen(buffer,"r");
	if (fp == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s dans la fonction myGitCommit !\n", branch_name);
		exit(1);
	}
	
	char result[256];
	memset(result, 0, 256);

	if(fgets(result,256,fp) != NULL){
		commitSet(c,"predecessor",result);
	}
	
	if(message!=NULL){
		commitSet(c,"message",message);
	}

	char *hash_commit=blobCommit(c);
	
	createUpdateRef(buffer,hash_commit);
	createUpdateRef("HEAD",hash_commit);
	
	fclose(fp);
	
	free(hash_commit);
	freeCommit(c);
}
