#include "ref.h"


void initRefs(){
	char cmd[256];
	sprintf(cmd,"mkdir -p .refs && touch .refs/{master,HEAD}");
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

	if (file_exists(".add")) {
		system("touch .add");
	}
	
	WorkTree*wt=ftwt(".add");

	if(file_exists(file_or_folder) == 1){
		appendWorkTree(wt,file_or_folder,NULL,0);
		wttf(wt, ".add");
	}
		
}


void myGitCommit(char * branch_name,char * message){


	if(file_exists(".refs")){
		fprintf(stderr,"Initialiser d'abord les références du projet\n");
		return ;
	}
	
	if(file_exists(branch_name)){
		fprintf(stderr,"La branche n'existe pas\n");
		return ;
	}
	
	char *test=sha256file(".refs/HEAD");
	
	char buffer[256];
	sprintf(buffer,".refs/%s",branch_name);
	char *test2=sha256file(buffer);
	
	if(strcmp(test,test2)!=0){
		fprintf(stderr,"“HEAD doit pointer sur le dernier commit de la branche\n");
		free(test);
		free(test2);
		return ;
	}
	
	WorkTree*wt=ftwt(".add");
	
	system("rm -f .add");
	
	char *hash=saveWorkTree(wt,".");
	
	Commit *c=initCommit();
	
	commitSet(c,"tree",hash);
	
	FILE*fp=fopen(branch_name,"r");
	
	char result[256];

	
	
	
	if(fgets(result,256,fp)!= NULL && strlen(fgets(result,256,fp))!=0){
		commitSet(c,"predecessor",result);
	}
	
	if(message!=NULL){
		commitSet(c,"message",message);
	
	}
	
	char *hash_commit=blobCommit(c);
	
	createUpdateRef(branch_name,hash_commit);
	createUpdateRef("HEAD",hash_commit);
	
	
	
	
	free(test);
	free(test2);
	
	
	


}



