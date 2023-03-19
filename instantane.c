#include "instantane.h"


/*Enregistre un instantane du fichier donne en parametre*/
void blobFile(char* file) {
	/*Initialisation et declaration des variables*/
	char cmd[256], *hash = sha256file(file), *path = hashToPath(hash);

	/*Ecriture dans un buffer de la commande pour creer un repertoire pour l'instantane du fichier a l'emplacement du fichier original*/
	sprintf(cmd, "mkdir -p $(dirname %s)", path);
	system(cmd);

	/*Copie du fichier vers son instantané*/
	cp(path, file);

	free(hash);
	free(path);

	return;
}