#include "instantane.h"

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

/*Renvoie le chemin absolu du chemin donne en parametre*/
char* absolutePath(char* path) {
	/*Initialisation des variables*/
	static char template [] = "/tmp/myfileXXXXXX";

	FILE* f;
	int descripteur, i, taille;
	char cmd[2509], fich[256], dir[256], buffer[256], fname[1000];
	char* res;

	memset(dir, 0, 256);
	memset(fich, 0, 256);

	/*Sauvegarde du template dans une nouvelle string et creation d'un fichier temporaire cree a partir du template*/
	strcpy (fname, template);
	descripteur = mkstemp(fname);

	if (descripteur == -1) {
		fprintf(stderr, "Erreur lors de la creation d'un fichier temporaire pour la fonction absolutePath !\n");
		exit(1);
	}

	/*Ecriture dans un buffer de la commande a utiliser pour la recuperation du nom du repertoire du fichier*/
	sprintf(cmd, "basename %s >%s && dirname %s >>%s", path, fname, path, fname);
	system(cmd);

	f = fdopen(descripteur, "r");

	if (f == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier temporaire %s pour la fonction absolutePath !\n", fname);
		
		/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
		sprintf(cmd, "rm %s", fname);
		system(cmd);

		exit(1);
	}

	/*Parcours du fichier temporaire*/
	for (i = 0; i < 2 && (fgets(buffer, 256, f) != NULL); i++) {
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
	fclose(f);

	res = (char*) malloc(sizeof(char) * (strlen(dir) + strlen(fich) + 2));

	if (res == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'une chaine de caracteres dans la fonction absolutePath !\n");
		exit(1);
	}

	sprintf(res, "%s/%s", dir, fich);
	
	return res;
}

/*Retourne une nouvelle chaine de caracteres contenant la partie du chemin donne en parametre qui correspond a un repertoire*/
char* dirName(char* path) {
	/*Initialisation des variables*/
	static char template [] = "/tmp/myfileXXXXXX";

	FILE* f;
	int descripteur, taille;
	char cmd[1255], dir[256], fname[1000];
	char* res;

	/*Sauvegarde du template dans une nouvelle string et creation d'un fichier temporaire cree a partir du template*/
	strcpy (fname, template);
	descripteur = mkstemp(fname);

	if (descripteur == -1) {
		fprintf(stderr, "Erreur lors de la creation d'un fichier temporaire pour la fonction dirName !\n");
		exit(1);
	}

	/*Ecriture dans un buffer de la commande a utiliser pour la recuperation du nom du repertoire du fichier*/
	sprintf(cmd, "dirname %s >%s", path, fname);
	system(cmd);

	f = fdopen(descripteur, "r");

	if (f == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier temporaire %s pour la fonction dirName !\n", fname);
		
		/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
		sprintf(cmd, "rm %s", fname);
		system(cmd);

		exit(1);
	}

	/*Parcours du fichier temporaire*/
	if (fgets(dir, 256, f) != NULL) {
		/*Suppression du caractere '\n' a la fin du nom du fichier*/
		taille = strcspn(dir, "\n");
		res = (char*) malloc(sizeof(char) * (taille + 1));

		if (res == NULL) {
			fprintf(stderr, "Erreur lors de l'allocation d'une chaine de caracteres pour la fonction dirName !\n");
			
			/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
			sprintf(cmd, "rm %s", fname);
			system(cmd);
			fclose(f);
			/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
			sprintf(cmd, "rm %s", fname);
			system(cmd);

			exit(1);
		}

		strncpy(res, dir, taille);
		res[taille] = '\0';
	}
	else {
		res = (char*) (malloc(sizeof(char)));
		res[0] = '\0';
	}

	/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
	sprintf(cmd, "rm %s", fname);
	system(cmd);
	fclose(f);
	
	return res;
}

/*Retourne une nouvelle chaine de caracteres contenant le nom du fichier au bout d'un chemin*/
char* baseName(char* path) {
	/*Initialisation des variables*/
	static char template [] = "/tmp/myfileXXXXXX";

	FILE* f;
	int descripteur, taille;
	char cmd[1255], fich[256], fname[1000];
	char* res;

	/*Sauvegarde du template dans une nouvelle string et creation d'un fichier temporaire cree a partir du template*/
	strcpy (fname, template);
	descripteur = mkstemp(fname);

	if (descripteur == -1) {
		fprintf(stderr, "Erreur lors de la creation d'un fichier temporaire pour la fonction baseName !\n");
		exit(1);
	}

	/*Ecriture dans un buffer de la commande a utiliser pour la recuperation du nom du fichier*/
	sprintf(cmd, "basename %s >%s", path, fname);
	system(cmd);

	f = fdopen(descripteur, "r");

	if (f == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier temporaire %s pour la fonction baseName !\n", fname);
		/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
		sprintf(cmd, "rm %s", fname);
		system(cmd);
		exit(1);
	}

	/*Parcours du fichier temporaire*/
	if (fgets(fich, 256, f) != NULL) {
		/*Suppression du caractere '\n' a la fin du nom du fichier*/
		taille = strcspn(fich, "\n");
		res = (char*) malloc(sizeof(char) * (taille + 1));

		if (res == NULL) {
			fprintf(stderr, "Erreur lors de l'allocation d'une chaine de caracteres pour la fonction baseName !\n");
			
			/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
			sprintf(cmd, "rm %s", fname);
			system(cmd);
			fclose(f);

			exit(1);
		}

		strncpy(res, fich, taille);
		res[taille] = '\0';
	}
	else {
		res = (char*) (malloc(sizeof(char)));
		res[0] = '\0';
	}

	/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
	sprintf(cmd, "rm %s", fname);
	system(cmd);
	fclose(f);
	
	return res;
}

/*Copie le contenu du fichier from dans le fichier to*/
void cp(char* to, char* from) {
	/*Initialisation et declaration des variables*/
	DIR* d;	
	FILE* src;
	FILE* dest;
	struct dirent* ep;
	int present = 0;
	char buffer[256], *dir = dirName(from), *fich = baseName(from);

	/*Verification de la presence du fichier source sur le systeme*/
	d = opendir(dir);

	if (d == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du repertoire %s pour la fonction cp !\n", dir);
		free(dir);
		free(fich);
		exit(1);
	}

	/*Parcours du repertoire et comparaison des noms des fichiers qu'il contient avec le fichier source*/
	while ((ep = readdir(d)) != NULL) {
		/*Retourne 1 si le fichier est trouve*/
		if(strcmp(fich, ep -> d_name) == 0) {
			present = 1;
		}
	}

	closedir(d);

	if (present == 0) {
		fprintf(stderr, "Erreur dans la fonction cp : le fichier source %s n'existe pas !\n", from);
		free(dir);
		free(fich);
		exit(1);
	}

	/*Ouverture des fichiers source et destination*/
	src = fopen(from, "r");
	
	if (src == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier source %s pour la fonction cp !\n", from);
		free(dir);
		free(fich);
		exit(1);
	}
	
	dest = fopen(to, "w");
	
	if (dest == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier destination %s pour la fonction cp !\n", to);
		free(dir);
		free(fich);
		fclose(src);
		exit(1);
	}

	/*Parcours du fichier source et ecriture en simultane dans le fichier destination*/
	while(fgets(buffer, 256, src) != NULL) {
		fputs(buffer, dest);
	}

	free(dir);
	free(fich);
	fclose(src);
	fclose(dest);

	return;
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