#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "list_str.h"
#include "instantane.h"

int main(){
	/*Initialisation des variables*/
	FILE* f;
	char buffer[256];
	char* hash = sha256file("main.c"), *path = hashToPath(hash);

	/*Tests des fonctions*/
	//hashFile("main.c", "files.tmp");
	printf("%s\n", hash);
	blobFile("main.c");

	f = fopen("main.c", "r");

	if (f == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s pour la fonction main !\n", "main.c");
		free(hash);
		free(path);
		exit(1);
	}

	/*Affichage du contenu de l'ancien fichier*/
	printf("\nAffichage de l'ancien fichier :\n\n");
	while(fgets(buffer, 256, f) != NULL) {
		printf("%s", buffer);
	}

	fclose(f);
	printf("\n\n");

	f = fopen(path, "r");

	if (f == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s pour la fonction main !\n", path);
		free(hash);
		free(path);
		exit(1);
	}

	/*Affichage du contenu de la sauvegarde du fichier*/
	printf("\nAffichage de l'instantane du fichier :\n\n");
	while(fgets(buffer, 256, f) != NULL) {
		printf("%s", buffer);
	}
	
	fclose(f);
	free(hash);
	free(path);

	return 0;
}