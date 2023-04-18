#include "branch.h"

/*Initialise .current_branch avec la branche par defaut*/
void initBranch(){
	FILE*f=fopen(".current_branch","w");
	fputs("master",f);
	fclose(f);
}

/*Renvoie 1 si la branche existe, 0 sinon*/
int branchExists(char* branch){
	int res = 0;

	/*On recupere la liste des fichiers presents dans .refs*/
	List*refs=listdir(".refs");

	/*On cherche le nom de la branche*/
	if (searchList(refs,branch) != NULL) {
		res = 1;
	}

	freeList(refs);
	return res;
}

/*Creer une nouvelle branche et l'initialise avec le dernier commit pointe par HEAD*/
void createBranch(char* branch){
	char *hash=getRef("HEAD");
	createUpdateRef(branch,hash);
	free(hash);
}

/*Renvoie la chaine de caracteres correspondant au contenu de .current_branch*/
char * getCurrentBranch(){
	/*On verifie que le fichier existe*/
	if (file_exists(".current_branch") != 1) {
		fprintf(stderr, "Le fichier .current_branch n'existe pas !\n");
		return NULL;
	}

	/*On recupere le contenu du fichier*/
	FILE*f=fopen(".current_branch","r");
	if(f == NULL){
		fprintf(stderr, "Erreur lors de l'ouverture du fichier .current-branch pour la fonction getCurrentBranch !\n");
		exit(1);
	}

	char*buff=malloc(sizeof(char)*1000);
	fscanf(f,"%s",buff);

	fclose(f);

	return buff;
}

/*Renvoie la chaine de caractere correspondant au chemin du hash d'un commit*/
char *hashToPathCommit(char* hash){
	/*Allocation de la memoire*/
	char * buff=malloc(sizeof(char)*1000);
	if (buff == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation de memoire pour la fonction hashToPathCommit !\n");
		exit(1);
	}
	
	/*On recupere le chemin et on rajoute l'extension .c*/
	char* tmp = hashToPath(hash);
	sprintf(buff,"%s.c", tmp);

	free(tmp);

	return buff;
}

/*Affiche l'arborescence des commit de la branche*/
void printBranch(char* branch){
	/*Recuperation du dernier Commit de la branche*/
	char * commit_hash = getRef(branch);
	if (commit_hash == NULL) {
		return;
	}
	char* path = hashToPathCommit(commit_hash);
	Commit* c=ftc(path);

	/*Parcours du Commit avec son champs predecessor*/
	while( c != NULL ){
		/*Recuperation du message*/
		char* message = commitGet(c, "message");
		printf("%s -> %s\n", commit_hash, message);

		/*Recuperation du predecesseur*/
		char* pred_hash_tmp = commitGet(c,"predecessor");
		if(pred_hash_tmp != NULL && strcmp(pred_hash_tmp, "(null)") != 0){
			free(commit_hash);
			commit_hash = strdup(pred_hash_tmp);
			char* path_pred = hashToPathCommit(pred_hash_tmp);
			freeCommit(c);
			c=ftc(path_pred);
			free(path_pred);
		}else{
			freeCommit(c);
			c=NULL;
		}
	}

	free(path);
	free(commit_hash);
}

/*Retourne la liste contenant tous les commits de la branche*/
List* branchList(char* branch){
	/*Initialisation de la liste*/
	List*l=initList();

	/*Recuperation du dernier Commit de la branche*/
	char * commit_hash=getRef(branch);
	char* path = hashToPathCommit(commit_hash);
	Commit* c=ftc(path);

	/*Parcours du Commit avec son champs predecessor*/
	while( c != NULL ){
		/*Insertion du hash du Commit actuel*/
		insertFirst(l, buildCell(commit_hash));

		/*Recuperation du predecesseur*/
		char* pred_hash_tmp = commitGet(c,"predecessor");
		if(pred_hash_tmp != NULL && strcmp(pred_hash_tmp, "(null)") != 0){
			free(commit_hash);
			commit_hash = strdup(pred_hash_tmp);
			char* path_pred = hashToPathCommit(pred_hash_tmp);
			freeCommit(c);
			c=ftc(path_pred);
			free(path_pred);
		}else{
			freeCommit(c);
			c=NULL;
		}
	}

	free(path);
	free(commit_hash);

	return l;
}

/*Renvoie la liste des commits de toutes les branches*/
List * getAllCommits(){
	/*Initialisation de la liste*/
	List* result = initList();
	List* all = listdir(".refs");
	Cell* cellule = *all;

	/*Parcours de toutes les branches*/
	while (cellule != NULL){
		/*On passe les fichiers caches*/
		if(cellule->data[0] == '.') {
			cellule = cellule -> next;
			continue;
		}

		/*Recuperation de la liste des commits de la branche courante à la boucle*/
		List* br_list = branchList(cellule->data);

		/*On verifie que les Commits ne soient pas deja dans la liste resultat*/
		Cell* cellule2 = *br_list;
		while(cellule2 != NULL){
			if(searchList(result, cellule2->data) == NULL){
				insertFirst(result, buildCell(cellule2->data));
			}
			cellule2 = cellule2->next;
		}

		freeList(br_list);
		
		cellule = cellule->next;
	}

	freeList(all);
	
	return result;
}