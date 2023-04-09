#include "workfile.h"


/*Renvoie un WorkFile initialiser avec ses champs vides*/
WorkFile* createWorkFile(char * name){
	/*Allocation de la memoire pour la structure*/
	WorkFile* w = malloc(sizeof(WorkFile));
	if (w == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'un workfile pour la fonction createWorkFile !\n");
		exit(1);
	}

	/*Recuperation du nom*/
	w->name = strdup(name);

	/*Initialisation des autres variables*/
	w->hash = NULL;
	w->mode = 0;

	return w;
}

/*Libere un la memoire utilisee par un workfile*/
void freeWorkFile(WorkFile* w) {
	if (w != NULL) {
		if (w -> name != NULL) {
			free(w -> name);
		}

		if (w -> hash != NULL) {
			free(w -> hash);
		}

		free(w);
	}
}

/*Renvoie la chiane de caractere contenant les champs du WorkFile wf, separes par \t*/
char* wfts(WorkFile* wf){
	int taille = 256*2+3;
	char * buffer=malloc(sizeof(char)*taille);
	if (buffer == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'un buffer pour la fonction wfts !\n");
		exit(1);
	}
	memset(buffer, 0, taille);
	
	sprintf(buffer,"%s\t%s\t%d",wf->name,wf->hash,wf->mode);
	
	return buffer;
}

WorkFile* stwf(char* ch){

	char name[256];
 
	char hash[256];
 
	int mode;
 
	sscanf(ch,"%s\t%s\t%d",name,hash,&mode);
 
	WorkFile*w=createWorkFile(name);

	w->hash=strdup(hash);

	w->mode=mode;
 
	return w;
}