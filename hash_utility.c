#include "hash_utility.h"


/*Renvoie une chaine de caracteres contenant le hash du contenu du fichier passe en parametre*/
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

	/*On retire le "-\n" que renvoie la commande sha256sum*/
	int i = 0;
	while (buffer[i] != '\0') {
		if (buffer[i] == ' ')
			break;
		i++;
	}
	for (int j = i; j < 255; j++){
		buffer[j] = '\0';
	}

	sprintf(cmd, "rm %s", fname);
	system(cmd);

	return buffer;
}

/*Hash le contenu du fichier 'source' et l'ecrit dans le fichier 'dest'*/
int hashFile(char* source, char* dest){
	/*Initialisation du buffer*/
	char cmd[256];

	char* hash = sha256file(source);

	/*Ecrture dans le buffer de la commande a utiliser pour le hachage*/
	sprintf(cmd, "echo %s > %s", hash, dest);

	free(hash);
	
	return system(cmd);
}

/*Retourne le chemin d'un fichier a partir de son hash*/
char* hashToPath(char* hash) {
	/*Initialisation des variables*/
	char* res;
	int taille = (strlen(hash) + 2);

	if (taille <= 4) {
		fprintf(stderr, "Erreur lors de la recherche du chemin du hash %s : cas non pris en charge !\n", hash);
		exit(1);
	}

	res = (char *) malloc(sizeof(char) * taille);
	if (res == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'une chaine de caracteres pour la fonction hashToPath !\n");
		exit(1);
	}

	strncpy(res, hash, 2);

	res[2] = '/';
	res[3] = '\0';
	strcat(res, hash + 2);

	return res;
}