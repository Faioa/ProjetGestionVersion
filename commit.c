#include "commit.h"

kvp* createKeyVal(char* key, char* val){
	kvp* k=malloc(sizeof(kvp));
	if (k == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation de memoire pour une association KeyVal dans la fonction createKeyVal !\n");
	}
	if (key != NULL)
		k->key=strdup(key);
	else
		k->key = NULL;
	if (val != NULL)
		k->value = strdup(val);
	else
		k->value = NULL;
	return k;
}

void freeKeyVal(kvp* kv){
	free(kv->key);
	free(kv->value);
	free(kv);
}

char* kvts(kvp* k){
	int taille = 256*2+3;
	char * buffer=malloc(sizeof(char)*taille);
	if (buffer == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'un buffer pour la fonction kvts !\n");
		exit(1);
	}
	memset(buffer, 0, taille);
	sprintf(buffer,"%s :%s",k->key,k->value);
	return buffer;
}

kvp* stkv(char* str){
	char key[256];
	memset(key, 0, 256);
 
	char val[256];
	memset(val, 0, 256);
 
 
	sscanf(str,"%s :%s",key,val);
 
	kvp*nouv=createKeyVal(key,val);

 
	return nouv;
}

Commit* initCommit(){
	Commit*c=malloc(sizeof(Commit));
	c->size=TAILLE_C;
	c->n=0;
	c->T=malloc(sizeof(kvp*)*TAILLE_C);
	for (int i = 0; i < c -> size; i++) {
		c->T[i] = NULL;
	}
	return c;
}

unsigned long hash(unsigned char *str){
    unsigned long hash = 5381;
    int c;

    while (c = *str++ != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

void commitSet(Commit* c, char* key, char* value){
	int pos = hash((unsigned char *)key), i;
	
	for(i=0; i < c->size; i++){
		if (c->T[((pos+(unsigned long)i)%(unsigned long)c->size)] == NULL) {
				c->T[((pos+(unsigned long)i)%(unsigned long)c->size)] = createKeyVal(key,value);
				c->n++;
				break;
		}
	}
	
	if (i == c -> size) {
		fprintf(stderr, "Il n'y a plus de place dans le Commit !\n");
		exit(1);
	}
}

Commit* createCommit(char* hash){
	Commit * c=initCommit();
	commitSet(c,"tree",hash);
	return c;
}

char* commitGet(Commit* c, char* key){
	if (c == NULL)
		return NULL;
	//parcours du tableau pour chercher la clé
	for(int i=0;i<c->size;i++){
		if (c->T[i] != NULL) {
			if(strcmp(c->T[i]->key,key)==0)
				return c->T[i]->value;
		}
	}
	return NULL;


}

char* cts(Commit* c){

/*Initialisations des variables*/
	int taille = (256 * 2 + 3) * (c->n) + 1; /*256*2+3 est la taille max d'une représentation de KeyVal*/
	if (taille == 0) {
		taille = 1;
	}

	/*Allocation d'un buffer*/
	char *buffer=malloc(sizeof(char)*taille);
	if (buffer == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'un buffer pour la fonction cts !\n");
		exit(1);
	}

	/*Initialisation du buffer*/
	memset(buffer, 0, taille);
	
	/*Parcours des elements et ajouts de la representation a la suite du buffer*/
	for(int i = 0; i < c -> size; i++){
		if ((c->T)[i] != NULL) {
			char* kv_string = kvts((c->T)[i]);
			strcat(buffer, kv_string);
			strcat(buffer,"\n");
			free(kv_string);
		}
	}
	
	return buffer;


}
Commit* stc(char* ch){
/*Initialisation des variables*/
	Commit* c = initCommit();
	kvp* k = NULL;
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
			k=stkv(buffer);
			commitSet(c,k->key,k->value);
			free(k->key);
			free(k->value);
			free(k);
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
	return c;

}

void ctf(Commit* c, char* file){
	/*Initialisation des variables*/
	char* buffer = cts(c);

	FILE* f = fopen(file, "w");
	if (f == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s dans la fonction ctf !\n", file);
		exit(1);
	}

	fputs(buffer, f);

	fclose(f);
	free(buffer);

	return ;
}

Commit* ftc(char* file){
	/*Initialisation des variables*/
	kvp* k = NULL;
	Commit* c = initCommit();
	char buffer[256];

	FILE* f = fopen(file,"r");
	if (f == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s dans la fonction ftwt !\n", file);
		exit(1);
	}

	/*Lecture du fichier et traitement des donnees*/
	while(fgets(buffer, 256, f)){
		k = stkv(buffer);
		commitSet(c,k->key,k->value);
		freeKeyVal(k);
	}
	
	fclose(f);

	return c;
	

}

char* blobCommit(Commit* c){
	/*Initialisation et declaration des variables*/
	static char template[] = "/tmp/myfileXXXXXX";

	char fname[1000], cmd[10000], *res, *path, *buffer, *tmp;

	/*Sauvegarde du template dans une nouvelle string et creation d'un fichier temporaire cree a partir du template*/
	strcpy (fname, template);
	int descripteur = mkstemp(fname);

	if (descripteur == -1) {
		fprintf(stderr, "Erreur lors de la creation d'un fichier temporaire pour la fonction blobCommit !\n");
		
		/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
		sprintf(cmd, "rm %s", fname);
		system(cmd);

		exit(1);
	}

	ctf(c, fname);
	tmp = sha256file(fname);
	res = malloc(sizeof(char)*256);
	memset(res, 0, 256);
	snprintf(res, 65, "%s", tmp);
	buffer = hashToPath(res);
	path = malloc(sizeof(char)*strlen(buffer)+3);
	sprintf(path, "%s.c", buffer);
	cp(path, fname);

	free(buffer);
	free(path);
	free(tmp);
	close(descripteur);

	return res;
}

void free_kvp(kvp *k){
	if (k != NULL) {
		if (k->key != NULL)
			free(k->key);
		if (k->value != NULL)
			free(k->value);
		free(k);
	}
}

void freeCommit(Commit *c){
	if (c != NULL) {
		for(int i=0; i < c->size; i++){
			if (c->T[i] != NULL) {
				free_kvp(c->T[i]);
			}
		}
		free(c->T);
		free(c);
	}
}