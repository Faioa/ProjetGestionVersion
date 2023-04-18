#include "ref.h"


/*Initialise les reference par defaut du depot local*/
void initRefs(){
	char cmd[256];
	sprintf(cmd,"mkdir -p .refs && touch .refs/master .refs/HEAD");
	system(cmd);

}

/*Cree/met a jour une reference avec le hash passe en parametre*/
void createUpdateRef(char* ref_name, char* hash){
	
	char cmd[256], *name = baseName(ref_name);
	sprintf(cmd,"(echo %s) > $(echo .refs/%s)", hash, name);
	system(cmd);
	
	free(name);
}

/*Supprime une reference*/
void deleteRef(char* ref_name){
	char cmd[256], *name = baseName(ref_name);
	sprintf(cmd,"rm -f .refs/%s", name);
	system(cmd);
	
	free(name);
}

/*Renvoie la chaine de caracteres contenant le hash du dernier commit de la reference passee en parametre*/
char* getRef(char* ref_name){
	char*result;
	
	/*Recuperation du chemin vers la reference*/
	char buffer[256];
	sprintf(buffer,".refs/%s",ref_name);
	if(file_exists(buffer) == 0) {
		return NULL;
	}
	
	FILE*fp=fopen(buffer,"r");
	if (fp == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s dans la fonction getRef\n", buffer);
		exit(1);
	}
	
	/*Recuperation du contenu du fichier*/
	result = malloc(sizeof(char)*256);
	memset(result, 0, 256);
	if (result == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation de memoire pour le buffer de la fonction getRef !\n");
		exit(1);
	}
	fgets(result,256,fp);

	/*On supprime un potentiel \n a la fin de la chaine*/
	for (int i = strlen(result); i > -1; i--) {
		if (result[i] == '\n') {
			result[i] = '\0';
			break;
		}
	}

	fclose(fp);

	if (strlen(result) == 0 || strcmp(result, "(null)") == 0) {
		free(result);
		return NULL;
	}
	
	return result;
}

/*Ajoute le fichier file_or_folder a la zone de preparation*/
void myGitAdd(char* file_or_folder){

	/*On verifie que la zone de preparation existe*/
	if (! file_exists(".add")) {
		system("touch .add");
	}
	
	/*On charge la zone de preparation existante*/
	WorkTree*wt=ftwt(".add");

	/*On ajoute le fichier*/
	if(file_exists(file_or_folder) == 1){
		appendWorkTree(wt,file_or_folder,NULL,0);
		wttf(wt, ".add");
	}

	freeWorkTree(wt);
}

/*Effectue le commit de la zone de preparation vers la branche passee en parametre*/
void myGitCommit(char* branch_name, char * message){

	/*On verifie que la zone de preparation existe*/
	if (file_exists(".add") != 1) {
		printf("La zone de preparation est vide ! Il n'y a rien a commit !\n");
		return;
	}

	/*On verifie que le repertoire .refs existe bien*/
	if(is_directory(".refs") != 0){
		fprintf(stderr,"Initialiser d'abord les références du projet !\n");
		return ;
	}
	
	/*On verifie que la branche passee en parametre est une branche existante*/
	char buffer[256];
	sprintf(buffer,".refs/%s", branch_name);
	if(file_exists(buffer) != 1){
		fprintf(stderr,"La branche %s n'existe pas\n", branch_name);
		return ;
	}

	/*On recupere les hashs des references branch_name et HEAD*/
	char* content1 = getRef("HEAD");
	char* content2 = getRef(branch_name);
	
	/*On verifie que les hashs ne soit pas vide*/
	if(content1 != NULL) {
		free(content1);
		content1 = strdup("(null)");
	}
	if(content2 != NULL) {
		free(content2);
		content2 = strdup("(null)");
	}
	
	/*On verifie que les deux references precedentes on pointent bien sur le meme commit*/
	if(content1 != NULL && content2 != NULL && strcmp(content1,content2) != 0){
		fprintf(stderr,"HEAD doit pointer sur le dernier commit de la branche\n");
		if (content1 != NULL)
			free(content1);
		if (content2 != NULL)
			free(content2);
		return ;
	}
	
	/*On recupere le WorkTree de la zone de preparation*/
	WorkTree*wt=ftwt(".add");
	
	/*Suppression de la zone de preparation*/
	system("rm -f .add");
	
	/*On sauvegarde l'instantane du WorkTree*/
	char *hash=saveWorkTree(wt,".");
	
	/*On construit le Commit*/
	Commit *c=initCommit();
	
	commitSet(c,"tree",hash);

	free(hash);
	
	commitSet(c,"predecessor",content2);
	
	/*On sauvegarde le message du commit si il a ete fournit*/
	commitSet(c,"message",message);

	/*On sauvegarde l'instantane du commit*/
	char *hash_commit=blobCommit(c);
		
	/*On met a jour les references*/
	createUpdateRef(buffer,hash_commit);
	createUpdateRef("HEAD",hash_commit);

	/*Liberation des ressources*/
	free(hash_commit);
	freeCommit(c);
	free(content1);
	free(content2);
				
	printf("Le commit sur la branche %s s'est bien passe !\n", branch_name);
}
