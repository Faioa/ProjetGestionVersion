#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "list_str.h"
#include "list_dir.h"


/*Renvoie une chaine de caracteres contenant le has du contenu du fichier passe en parametre*/
char* sha256file(char* file){
	/*Initialisation et declaration des variables*/
	static char template [] = "/tmp/myfileXXXXXX";

	int sortie;
	char fname[1000], cmd[1000+strlen(file)], *buffer = (char*) malloc(sizeof(char) * 256);

	if (buffer == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'une chaine de caracteres pour la fonction sha256file !\n");
		exit(1);
	}

	memset(buffer, 0, 256);

	/*Sauvegarde du template dans une nouvelle string et creation d'un fichier temporaire cree a partir du template*/
	strcpy (fname, template);
	int descripteur = mkstemp(fname);

	if (descripteur == -1) {
		fprintf(stderr, "Erreur lors de la creation d'un fichier temporaire pour la fonction sha256file !\n");
		
		/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
		sprintf(cmd, "rm %s", fname);
		system(cmd);
		free(buffer);

		exit(1);
	}

	/*Ecriture dans un buffer de la commande a utiliser pour le hachage*/
	sprintf(cmd, "cat %s | sha256sum >%s", file, fname);
	system(cmd);

	/*Recuperation de la première ligne du fichier temporaire (la commande de hash renvoie de toute façon une unique ligne si un seul fichier est renseigne)*/
	sortie = read(descripteur, buffer, 256);

	if (sortie == -1) {
		fprintf(stderr, "Erreur lors de la lecture du fichier temporaire %s de la fonction sha256file !\n", fname);
		
		/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
		sprintf(cmd, "rm %s", fname);
		system(cmd);
		free(buffer);

		exit(1);
	}
	if (sortie == 0) {
		buffer[0] = '\0';
	}

	sprintf(cmd, "rm %s", fname);
	system(cmd);

	return buffer;
}

/*Hash le contenu du fichier 'source' et l'ecrit dans le fichier 'dest'*/
int hashFile(char* source, char* dest){
	/*Initialisation du buffer*/
	char cmd[256];

	/*Ecrture dans le buffer de la commande a utiliser pour le hachage*/
	sprintf(cmd, "cat %s | sha256sum > %s", source, dest);
	
	return system(cmd);
}

int main(){
	/*Initialisation des variables*/
	FILE* f;
	char buffer[256];
	char* hash = sha256file("main.c");

	/*Tests des fonctions*/
	//hashFile("main.c", "files.tmp");
	printf("%s\n", hash);
	blobFile("main.c");

	f = fopen("main.c", "r");

	if (f == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s pour la fonction main !\n", "main.c");
		free(hash);
		exit(1);
	}

	/*Affichage du contenu de l'ancien fichier*/
	printf("\nAffichage de l'ancien fichier :\n\n");
	while(fgets(buffer, 256, f) != NULL) {
		printf("%s", buffer);
	}

	fclose(f);
	printf("\n\n");
	f = fopen("snapshot/main.c_snapshot", "r");

	if (f == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s pour la fonction main !\n", "snapshot/main.c_snapshot");
		free(hash);
		exit(1);
	}

	/*Affichage du contenu de la sauvegarde du fichier*/
	printf("\nAffichage de l'instantane du fichier :\n\n");
	while(fgets(buffer, 256, f) != NULL) {
		printf("%s", buffer);
	}
	
	fclose(f);
	free(hash);

	return 0;
}