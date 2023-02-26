#include "list_dir.h"


/*Renvoie la liste contenant tous les noms des fichiers contenus dans le repertoire passe en parametre*/
List* listdir(char* root_dir) {
	/*Initialisation des variables*/
	DIR* dp;
	struct dirent* ep;
	List* list = initList();
	Cell* cell;
	dp = opendir(root_dir);
	
	if (dp == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du repertoire %s pour la fonction listdir !\n", root_dir);
		exit(1);
	}

	/*Parcours du repertoire et recuperation des noms des fichiers qu'il contient*/
	while ((ep = readdir(dp)) != NULL) {
		cell = buildCell(ep -> d_name);
		insertFirst(list, cell);
	}

	closedir(dp);

	return list;
}

/*Renvoie 1 si le fichier passe en parametre existe dans le repertoire courant, 0 sinon*/
int file_exists(char* file) {
	DIR* dp;
	struct dirent* ep;
	dp = opendir(".");
	
	if (dp == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du repertoire courant pour la fonction file_exists !\n");
		exit(1);
	}

	/*Parcours du repertoire et comparaison des noms des fichiers qu'il contient avec la chaine de caractere passee en parametres*/
	while ((ep = readdir(dp)) != NULL) {
		/*Retourne 1 si le fichier est trouve*/
		if(strcmp(file, ep -> d_name) == 0) {
			return 1;
		}
	}

	closedir(dp);

	return 0;
}

/*Copie le contenu du fichier from dans le fichier to*/
void cp(char* to, char* from) {
	/*Initialisation et declaration des variables*/
	static char template [] = "/tmp/myfileXXXXXX";

	DIR* d;	
	FILE* src;
	FILE* dest;
	FILE* f;
	struct dirent* ep;
	int taille, i, present = 0;
	char buffer[256], fname [1000], cmd[256], dir[256], fich[256], source[256];

	/*Sauvegarde du template dans une nouvelle string et creation d'un fichier temporaire cree a partir du template*/
	strcpy (fname, template);
	int descripteur = mkstemp(fname);

	if (descripteur == -1) {
		fprintf(stderr, "Erreur lors de la creation d'un fichier temporaire pour la fonction cp !\n");
		exit(1);
	}

	/*Ecriture dans un buffer de la commande a utiliser pour la recuperation du nom du repertoire du fichier source*/
	sprintf(cmd, "basename %s >%s && dirname %s >>%s", from, fname, from, fname);
	system(cmd);

	f = fdopen(descripteur, "r");

	if (f == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier temporaire %s pour la fonction cp !\n", fname);
	}

	/*Parcours du fichier temporaire*/
	for(i = 0; i < 2 && (fgets(buffer, 256, f) != NULL); i++) {
		if (i == 0) {
			/*Suppression du caractere '\n' a la fin du nom du fichier*/
			taille = strcspn(buffer, "\n");
			strncpy(fich, buffer, taille);
			fich[taille] = '\0';
		}
		else {
			/*Suppression du caractere '\n' a la fin du nom du repertoire*/
			taille = strcspn(buffer, "\n");
			strncpy(dir, buffer, taille);
			dir[taille] = '\0';
		}
	}

	/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
	sprintf(cmd, "rm %s", fname);
	system(cmd);

	/*Ouverture du repertoire pour verifier l'existence du fichier*/
	d = opendir(dir);

	if (d == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du repertoire %s pour la fonction cp !\n", dir);
		exit(1);
	}


	/*On verifie si le fichier source existe dans le repertoire*/
	while ((ep = readdir(d)) != NULL) {
		/*Retourne 1 si le fichier est trouve*/
		if(strcmp(fich, ep -> d_name) == 0) {
			present = 1;
		}
	}

	if (present == 0) {
		fprintf(stderr, "Le fichier %s n'existe pas !\n", from);
		exit(1);
	}

	closedir(d);

	sprintf(source, "%s/%s", dir, fich);

	/*Ouverture des fichiers source et destination*/
	src = fopen(source, "r");
	
	if (src == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s pour la fonction cp !\n", from);
		exit(1);
	}
	
	dest = fopen(to, "w");
	
	if (dest == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s pour la fonction cp !\n", to);
		fclose(src);
		exit(1);
	}

	/*Parcours du fichier source et ecriture en simultane dans le fichier destination*/
	while(fgets(buffer, 256, src) != NULL) {
		fputs(buffer, dest);
	}

	fclose(src);
	fclose(dest);

	return;
}

/*Retourne le chemin d'un fichier a partir de son hash*/
char* hashToPath(char* hash) {
	/*Initialisation des variables*/
	int i;
	char* res;

	res = (char *) malloc(sizeof(char) * (strlen(hash) + 1));
	
	if (res == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'une chaine de caracteres pour la fonction hashToPath !\n");
		exit(1);
	}

	strncpy(res, hash, 2);

	if (strlen(res) < 4) {
		return res;
	}

	res[2] = '/';
	res[3] = '\0';
	strcat(res, hash + 2);

	return res;
}

/*Enregistre un instantane du fichier donne en parametre*/
void blobFile(char* file) {
	/*Initialisation et declaration des variables*/
	static char template [] = "/tmp/myfileXXXXXX";

	int taille, i;
	FILE* f;
	char fname [1000], cmd[256], buffer[256], fich[256], dir[256];

	/*Sauvegarde du template dans une nouvelle string et creation d'un fichier temporaire cree a partir du template*/
	strcpy (fname, template);
	int descripteur = mkstemp(fname);

	if (descripteur == -1) {
		fprintf(stderr, "Erreur lors de la creation d'un fichier temporaire pour la fonction blobFile !\n");
		exit(1);
	}

	/*Ecriture dans un buffer de la commande a utiliser pour la recuperation des noms du fichier et du repertoire*/
	sprintf(cmd, "basename %s >%s && dirname %s >>%s", file, fname, file, fname);
	system(cmd);

	f = fdopen(descripteur, "r");

	if (f == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier temporaire %s pour la fonction blobFile !\n", fname);
	}

	/*Parcours du fichier temporaire*/
	for(i = 0; i < 2 && (fgets(buffer, 256, f) != NULL); i++) {
		if (i == 0) {
			/*Recuperation de la première ligne du fichier temporaire*/
			if (read(descripteur, buffer, 256) == -1) {
				fprintf(stderr, "Erreur lors de la lecture du fichier temporaire %s de la fonction blobFile !\n", fname);
				exit(1);
			}	

			/*Suppression du caractere '\n' a la fin du nom du fichier*/
			taille = strcspn(buffer, "\n");
			strncpy(fich, buffer, taille);
			fich[taille] = '\0';
		}
		else {
			/*Recuperation de la seconde ligne du fichier temporaire pour avoir le nom du repertoire du fichier*/
			if (read(descripteur, buffer, 256) == -1) {
				fprintf(stderr, "Erreur lors de la lecture du fichier temporaire %s de la fonction blobFile !\n", fname);
				exit(1);
			}

			/*Suppression du caractere '\n' a la fin du nom du repertoire*/
			taille = strcspn(buffer, "\n");
			strncpy(dir, buffer, taille);
			dir[taille] = '\0';
		}
	}	

	/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
	sprintf(cmd, "rm %s", fname);
	system(cmd);


	/*Ecriture dans un buffer de la commande pour creer un repertoire pour l'instantane du fichier a l'emplacement du fichier original*/
	sprintf(cmd, "mkdir -p %s/snapshot", dir);
	system(cmd);

	/*Formattage du nom de l'instantane a partir du nom de l'original*/
	sprintf(buffer, "%s/snapshot/%s_snapshot", dir, fich);

	/*Ecriture dans un buffer de la commande pour effectuer la copie du fichier vers l'instantane*/
	cp(buffer, file);

	return;
}