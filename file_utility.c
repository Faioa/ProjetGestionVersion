#include "file_utility.h"


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
		freeList(list);
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

/*Renvoie 1 si le fichier passe en parametre existe (on supppose que le chemin est fournit avec le nom du fichier)*/
int file_exists(char* file) {
	struct stat file_stat;

	if(lstat(file, &file_stat) == 0) {
		return 1;
	}

	return 0;
}

/*Renvoie 0 si la chaine pointe sur un fichier standard, 1 si la cible n'est pas un fichier standard, 2 si la cible n'existe pas*/
int is_regular_file(char* file) {
	struct stat file_stat;

	if(lstat(file, &file_stat) != 0) {
		return 2;
	}

	if(S_ISREG(file_stat.st_mode) == 0) {
		return 1;
	}

	return 0;
}

/*Renvoie 0 si la chaine pointe sur un repertoire, 1 si la cible n'est pas un repertoire, 2 si la cible n'existe pas*/
int is_directory(char* file) {
	struct stat file_stat;

	if(lstat(file, &file_stat) != 0) {
		return 2;
	}

	if(S_ISDIR(file_stat.st_mode) == 0) {
		return 1;
	}

	return 0;
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
	sprintf(cmd, "echo $(dirname %s) > %s", path, fname);
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
		taille = strlen(dir) + 1;
		res = (char*) malloc(sizeof(char) * taille);

		memset(res, 0, taille);

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

		strncpy(res, dir, taille - 1);

		if (res[taille - 2] == '\n')
			res[taille - 2] = '\0';
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
		taille = strlen(fich) + 1;
		res = (char*) malloc(sizeof(char) * taille);

		memset(res, 0, taille);

		if (res == NULL) {
			fprintf(stderr, "Erreur lors de l'allocation d'une chaine de caracteres pour la fonction baseName !\n");
			
			/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
			sprintf(cmd, "rm %s", fname);
			system(cmd);
			fclose(f);

			exit(1);
		}

		strncpy(res, fich, taille - 1);
		if (res[taille - 2] == '\n')
			res[taille - 2] = '\0';
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

/*Concatene les deux chemin en prenant path1 comme parent de path2. Retourne une chaine de caracteres contenant le chemin obtenu apres la fusion.*/
char* pathConcat(char* path1, char* path2) {
	/*Initialisation des variables*/
	int taille = strlen(path1)+strlen(path2)+2;
	char* res = malloc(sizeof(char)*taille);
	if (res == NULL) {
		fprintf(stderr, "Erreur de l'allocation de la memoire pour la fonction pathConcat !\n");
		exit(1);
	}

	memset(res, 0, taille);
	sprintf(res, "%s/%s", path1, path2);

	return res;
}

/*Copie le contenu du fichier from dans le fichier to*/
void cp(char* to, char* from) {
	/*Initialisation et declaration des variables*/
	FILE* src;
	FILE* dest;
	char buffer[256], *dir, cmd[1000];

	if(is_regular_file(from) != 0) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s pour la fonction cp : soit il n'existe pas ou bien les droits ne sont pas suffisants !\n", from);
		exit(1);
	}
	
	dir = dirName(to);
	if (strcmp(dir, ".") != 0 && strcmp(dir, "..") != 0 ) {
		sprintf(cmd, "mkdir -p $(dirname %s)", to);
		system(cmd);
	}
	free(dir);

	/*Ouverture des fichiers source et destination*/
	src = fopen(from, "r");	
	if (src == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier source %s pour la fonction cp !\n", from);
		exit(1);
	}
	
	dest = fopen(to, "w");
	if (dest == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier destination %s pour la fonction cp !\n", to);
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

int getChmod ( const char * path ) {
	struct stat ret;

	if ( stat( path, &ret ) == -1) {
		return -1;
	}

	return ( ret . st_mode & S_IRUSR ) | ( ret . st_mode & S_IWUSR ) | ( ret . st_mode & S_IXUSR ) |/*owner*/
	( ret . st_mode & S_IRGRP ) | ( ret . st_mode & S_IWGRP ) | ( ret . st_mode & S_IXGRP ) |/*group*/
	( ret . st_mode & S_IROTH ) | ( ret . st_mode & S_IWOTH ) | ( ret . st_mode & S_IXOTH );/*other*/
}

void setMode ( int mode , char * path ) {
	char buff[100];

	sprintf( buff , "chmod %d %s ",mode ,path );

	system(buff);
}
