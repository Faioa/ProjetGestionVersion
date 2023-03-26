#include "commit.h"

kvp* createKeyVal(char* key, char* val){
	kvp* k=malloc(sizeof(kvp));
	k->key=strdup(key);
	k->value=strdup(val);
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
 
	char val[256];
 
 
	sscanf(str,"%s :%s",key,val);
 
	kvp*nouv=createKeyVal(key,val);

 
	return nouv;
}

Commit* initCommit(){
	Commit*c=malloc(sizeof(Commit));
	c->size=TAILLE_C;
	c->n=0;
	c->T=malloc(sizeof(kvp*)*TAILLE_C);
	memset(c->T, 0, TAILLE_C);
	return c;
}

unsigned long hash(unsigned char *str){
    unsigned long hash = 5381;
    int c;

    while ((c = (*str)++) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void commitSet(Commit* c, char* key, char* value){

	kvp*k=createKeyVal(key,value);
	
	int pos=hash((unsigned char *)key), i;
	
	for(i=0;i<c->size;i++){
		if(c->T[((pos+i)%c->size)]==NULL){
			c->T[((pos+i)%c->size)]=k;
		}
	}
	
	if (i == c -> size) {
		fprintf(stderr, "Il n'y a plus de place dans le Commit !\n");
	}
	

}

char* commitGet(Commit* c, char* key){
	if (c == NULL)
		return NULL;
	//parcours du tableau pour chercher la clé
	for(int i=0;i<c->n;i++){
		if(strcmp(c->T[i]->key,key)==0)return c->T[i]->value;
	}
	return NULL;


}

char* cts(Commit* c){

/*Initialisations des variables*/
	int taille = (256 * 2 + 3) * (c->n); /*256*2+3 est la taille max d'une représentation de WorkFile*/
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
	for(int i = 0; i < c -> n; i++){
		strcat(buffer,kvts((c->T)[i]));
		strcat(buffer,"\n");
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
	/*Initialisation et declaration des variables*//*Initialisation et declaration des variables*/
	static char template [] = "/tmp/myfileXXXXXX";

	int sortie;
	char fname[1000], cmd[1000+TAILLE_C], *buffer = (char*) malloc(sizeof(char) * 256), *com;
	;

	if (buffer == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'une chaine de caracteres pour la fonction blobWorkTree !\n");
		exit(1);
	}

	/*Initialisation du buffer*/
	memset(buffer, 0, 256);

	/*Sauvegarde du template dans une nouvelle string et creation d'un fichier temporaire cree a partir du template*/
	strcpy (fname, template);
	int descripteur = mkstemp(fname);

	if (descripteur == -1) {
		fprintf(stderr, "Erreur lors de la creation d'un fichier temporaire pour la fonction blobWorkTree !\n");
		
		/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
		sprintf(cmd, "rm %s", fname);
		system(cmd);
		free(buffer);

		exit(1);
	}

	/*Ecriture dans un buffer de la commande a utiliser pour le hachage*/
	com = cts(c);
	sprintf(cmd, "cat %s | sha256sum >%s", com, fname);
	system(cmd);

	/*Recuperation de la première ligne du fichier temporaire (la commande de hash renvoie de toute façon une unique ligne si un seul fichier est renseigne)*/
	sortie = read(descripteur, buffer, 256);

	if (sortie == -1) {
		fprintf(stderr, "Erreur lors de la lecture du fichier temporaire %s de la fonction blobWorkTree !\n", fname);
		
		/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
		sprintf(cmd, "rm %s", fname);
		system(cmd);
		free(buffer);
		free(com);

		exit(1);
	}
	if (sortie == 0) {
		buffer[0] = '\0';
	}

	sprintf(cmd, "rm %s", fname);
	system(cmd);
	free(com);

	return buffer;
	
	
	
}



































