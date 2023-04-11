#include "worktree.h"


/*Alloue et initialise un WorkTree*/
WorkTree* initWorkTree(){
	/*Allocation de la memoire*/
	WorkTree* wt = malloc(sizeof(WorkTree));
	if (wt == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'un WorkTree pour la fonction initWorkTree !\n");
		exit(1);
	}

	/*Initialisation des champs*/
	wt->size=TAILLE_MAX;
	wt->n=0;

	wt -> tab = malloc(sizeof(WorkFile) * TAILLE_MAX);
	if (wt -> tab == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation du tableau d'un WorkTree pour la fonction initWorkTree !\n");
		free(wt);
		exit(1);
	}

	for(int i = 0; i < wt -> size; i++) {
		(wt -> tab[i]).name = NULL;
		(wt -> tab[i]).hash = NULL;
		(wt -> tab[i]).mode = 0;
	}

	return wt;
}

/*Libere la memoire allouee a un WorkTree*/
void freeWorkTree(WorkTree* wt) {
	for (int i = 0; i < wt -> n; i++) {
		if ((wt->tab+i)->name != NULL) {
			free((wt->tab+i)->name);
		}

		if ((wt->tab+i)->hash != NULL) {
			free((wt->tab+i)->hash);
		}
	}
	free(wt -> tab);
	free(wt);
}

/*Verifie si le fichier ou le repertoire passe en parametre existe dans le WorkTree. Renvoie la position de la cible si elle est trouvee et -1 sinon*/
int inWorkTree(WorkTree* wt, char* name){
	for (int i = 0; i< wt -> n; i++) {
		if (((wt -> tab)[i]).name != NULL && strcmp(((wt -> tab)[i]).name, name) == 0) {
			return i;
		}
	}

	return -1;
}

/*Ajoute un element au WorkTree si il n'y est pas deja*/
int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode){
	if (inWorkTree(wt,name) == -1 && wt -> n < wt -> size){
		wt->tab[wt->n].mode = mode;

		if (hash != NULL) {
			wt->tab[wt->n].hash = strdup(hash);
		} else {
			wt->tab[wt->n].hash = NULL;
		}

		if (name != NULL) {
			wt->tab[wt->n].name = strdup(name);
		} else {
			wt->tab[wt->n].name = NULL;
		}

		wt -> n = wt -> n + 1;

		return 0;
	}

	return -1;
}

/*Renvoie la chaine de caractere contenant la representation du WorkTree. Chaque WorkFile est separe par \n*/
char* wtts(WorkTree* wt){
	/*Initialisations des variables*/
	int taille = (256 * 2 + 3) * (wt->n); /*256*2+3 est la taille max d'une représentation de WorkFile*/
	if (taille == 0) {
		taille = 1;
	}

	/*Allocation d'un buffer*/
	char *buffer=malloc(sizeof(char)*taille);
	if (buffer == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'un buffer pour la fonction wtts !\n");
		exit(1);
	}

	/*Initialisation du buffer*/
	memset(buffer, 0, taille);
	
	/*Parcours des elements et ajouts de la representation a la suite du buffer*/
	for(int i = 0; i < wt -> n; i++){
		char* swf = wfts((wt->tab) + i);
		strcat(buffer,swf);
		strcat(buffer,"\n");
		free(swf);
	}
	
	return buffer;
}

/*Convertit une chaine de caracteres representant un WorkTree en un WorkTree*/
WorkTree* stwt(char* ch){
	/*Initialisation des variables*/
	WorkTree* wt = initWorkTree();
	WorkFile* wf = NULL;
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

	/*Initialisation du buffer*/
	memset(buffer, 0, taille);
	int i = 0;

	/*Parcours de l'entierete de la chaine de caracteres*/
	while (ch[i] != '\0') {
		/*Si on rencontre le separateur '\n' on creer une cellule avec ce qu'on à lu jusqu'a maintenant et on l'insere dans la liste*/
		if (ch[i++] == '\n') {
			wf=stwf(buffer);
			appendWorkTree(wt,wf->name,wf->hash,wf->mode);
			free(wf->name);
			free(wf->hash);
			free(wf);
			/*Reinitialisation du buffer*/
			memset(buffer, 0, taille);
		}
		/*Sinon on sauvegarde le caractere*/
		else {
			strcpy(save, buffer);
			sprintf(buffer, "%s%c", save, ch[i - 1]);
		}
	}
	
	free(buffer);
	free(save);
	return wt;
}

/*Ecrit dans le fichier file la representation du WorkTree. Renvoie 0 si tout s'est bien passe, une valeur positive sinon*/
int wttf(WorkTree* wt, char* file) {
	/*Initialisation des variables*/
	char* buffer = wtts(wt);

	FILE* f = fopen(file, "w");
	if (f == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s dans la fonction wttf !\n", file);
		exit(1);
	}

	fputs(buffer, f);

	fclose(f);
	free(buffer);

	return 0;
}

/*Construit un WorkTree a partir d'un fichier contenant sa representation*/
WorkTree* ftwt(char* file){
	/*Initialisation des variables*/
	WorkFile* wf = NULL;
	WorkTree* wt = initWorkTree();
	char buffer[256];

	FILE* f = fopen(file,"r");
	if (f == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s dans la fonction ftwt !\n", file);
		exit(1);
	}

	/*Lecture du fichier et traitement des donnees*/
	while(fgets(buffer, 256, f)){
		wf = stwf(buffer);
		appendWorkTree(wt, wf->name, wf->hash, wf->mode);
		freeWorkFile(wf);
	}
	
	fclose(f);

	return wt;
}