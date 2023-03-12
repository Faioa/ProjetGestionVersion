#include "worktree.h"


WorkTree* initWorkTree(){

	WorkTree*wt=malloc(sizeof(worktree));
	wt->tab=malloc(sizeof(workfile)*TAILLE_MAX);
	wt->size=TAILLE_MAX;
	wt->n=0;
	return wt;
}

int inWorkTree(WorkTree* wt, char* name){

	for(int i=0;i<wt->n;i++){
		if(strcmp(wt->tab[i]->name,name)==0)return i;
	}

	return -1;

}

int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode){
	if(inWorkTree(wt,name)==-1){
		WorkFile*w=createWorkFile(name);
		w->hash=hash;
		w->mode=mode;
		wt->tab[n++]=w;
		return 0;
	}
	return -1;
}

char* wtts(WorkTree* wt){
	
	int taille = 256*wt->n;
	char *buffer=malloc(sizeof(char)*taille);
	memset(buffer, 0, taille);
	
	for(int i=0;i<wt->n;i++){
		strcat(buffer,wfts(wt->tab[i]));
		strcat(buffer,"\n");
	}
	
	return buffer;

}

WorkTree* stwt(char*ch){
	/*Initialisation des variables*/
	WorkTree* wt=initWorkTree();
	WorkFile* wf=NULL;
	int taille = strlen(ch) + 1;
	char *buffer = (char*) malloc(sizeof(char) * taille);

	if (buffer == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'une chaine de caracteres (buffer) pour la fonction stwt !\n");

		exit(1);
	}

	char *save = (char*) malloc(sizeof(char) * taille);

	if (save == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'une chaine de caracteres (save) pour la fonction stwt !\n");

		free(buffer);
		exit(1);
	}

	memset(buffer, 0, taille);
	int i = 0;

	/*Parcours de l'entierete de la chaine de caracteres*/
	while (s[i] != '\0') {
		/*Si on rencontre le separateur '\n' on creer une cellule avec ce qu'on à lu jusqu'a maintenant et on l'insere dans la liste*/
		if (s[i++] == '\n') {
			wf=stwf(buffer);
			appendWorkTree(wt,wf->name,wf->hash,wf->mode);
			free(wf->name);
			free(wf->hash);
			free(wf);
			memset(buffer, 0, taille);
		}
		/*Sinon on sauvegarde le caractere*/
		else {
			strcpy(save, buffer);
			sprintf(buffer, "%s%c", save, s[i - 1]);
		}
	}
	
	free(buffer);
	free(save);
	return wt;
}

WorkTree* ftwt(char* file){
	WorkFile* wf=NULL;
	FILE* f=fopen(file,"r");
	WorkTree*wt=initWorkTree();
	char buffer[256];
	while(fgets(buffer,256,f)){
		wf=stwf(buffer);
		appendWorkTree(wt,wf->name,wf->hash,wf->mode);
		free(wf->name);
		free(wf->hash);
		free(wf);
	}
	
	return wt;



}

