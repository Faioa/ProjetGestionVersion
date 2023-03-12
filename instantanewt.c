#include "instantanewt.h" 

int getChmod ( const char * path ) {
	struct stat ret ;

	if ( stat ( path , & ret ) == -1) {
 		return -1;
 	}

return ( ret . st_mode & S_IRUSR ) |( ret . st_mode & S_IWUSR ) |( ret . st_mode & S_IXUSR ) |/*owner*/
 ( ret . st_mode & S_IRGRP ) |( ret . st_mode & S_IWGRP ) |( ret . st_mode & S_IXGRP ) |/*group*/
( ret . st_mode & S_IROTH ) |( ret . st_mode & S_IWOTH ) |( ret . st_mode & S_IXOTH ) ;/*other*/

}

void setMode(int mode, char* path){

	char buff[100];
	sprintf(buff, "chmod %o %s",mode, path);
	system(buff);


}



char* blobWorkTree(WorkTree* wt){
	/*Initialisation et declaration des variables*/
	static char template [] = "/tmp/myfileXXXXXX";

	int sortie;
	char fname[1000], cmd[1000+TAILLE_MAX], *buffer = (char*) malloc(sizeof(char) * 256), *wtree;
	;

	if (buffer == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'une chaine de caracteres pour la fonction blobWorkTree !\n");
		exit(1);
	}

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
	wtree = wtts(wt);
	sprintf(cmd, "cat %s | sha256sum >%s", wtree, fname);
	system(cmd);

	/*Recuperation de la première ligne du fichier temporaire (la commande de hash renvoie de toute façon une unique ligne si un seul fichier est renseigne)*/
	sortie = read(descripteur, buffer, 256);

	if (sortie == -1) {
		fprintf(stderr, "Erreur lors de la lecture du fichier temporaire %s de la fonction blobWorkTree !\n", fname);
		
		/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
		sprintf(cmd, "rm %s", fname);
		system(cmd);
		free(buffer);
		free(wtree);

		exit(1);
	}
	if (sortie == 0) {
		buffer[0] = '\0';
	}

	sprintf(cmd, "rm %s", fname);
	system(cmd);
	free(wtree);

	return buffer;
}

char * saveWorkTree(WorkTree* wt, char* path){



}
















