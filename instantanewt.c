#include "instantanewt.h" 


/*Creer un fichier temporaire avec la representation du WorkTree et qui renvoie son hash*/
char* blobWorkTree(WorkTree* wt){
	/*Initialisation et declaration des variables*/
	static char template [] = "/tmp/myfileXXXXXX";

	int sortie;
	char fname[1000], cmd[1000+TAILLE_MAX], *buffer = (char*) malloc(sizeof(char) * 256), *wtree, *path;
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

	strcat(buffer, ".t");

	/*Creation de l'instantane et ecriture de sa representation a l'interieur*/
	path = hashToPath(buffer);

	/*Ecriture dans un buffer de la commande pour creer l'instantane*/
	sprintf(cmd, "mkdir -p $(dirname %s) && echo %s > %s", path, wtree, path);
	system(cmd);


	free(wtree);
	free(path);
	close(descripteur);

	return buffer;
}

/*Creer recursivement tous les instantanes du WorkTree et enfin l'instantane de celui-ci*/
char* saveWorkTree(WorkTree* wt, char* path) {
	/*Initialisation des variables*/
	WorkFile* wf;
	int i;
	List* l;
	WorkTree* wtree;
	char buffer[256];

	memset(buffer, 0, 256);

	/*Parcours du tableau de WorkFile du WorkTree*/
	for (i = 0; i < wt -> n; i++) {
		wf = &(wt -> tab)[i];

		/*Si la cibles est un fichier standard*/
		if (is_regular_file(wf -> name) == 0) {
			blobFile(wf -> name);
			wf -> mode = getChmod(wf -> name);
			wf -> hash = sha256file(wf -> name);
		}

		/*Si la cible est un repertoire*/
		else if (is_directory(wf -> name ) == 0) {
			/*On recupere les noms des fichiers*/
			l = listdir(wf -> name);
			wtree = initWorkTree();

			/*On reconstruit le worktree*/
			while ((*l) != NULL) {
				appendWorkTree(wtree, (*l) -> date, NULL, 0);
			}

			/*On construit le path du nouveau WorkTree*/
			sprintf(buffer, "%s/%s", path, wf -> name);

			/*On appelle recursivement la fonction pour ce nouveau WorkTree*/
			wf -> hash = saveWorkTree(wtree, buffer);
			wf -> mode = getChmod(wf -> name);

			/*Libération des variables*/
			freeList(l);
			freeWorkTree(wtree);
		}
	}

	return blobWorkTree(wt);
}

/*Restore l'arborescence du WorkTree et les fichiers associes*/
void restoreWorkTree(WorkTree* wt, char* path) {
	/*Initialisation des variables*/
	WorkFile* wf;
	WorkTree* wtree;
	int i;
	char* point;
	char* t;
	char* from;
	char to[256], cmd[1000], new_path[256];

	memset(to, 0, 256);
	memset(cmd, 0, 256);
	memset(new_path, 0, 256);

	/*Parcours du tableau de WorkFile du WorkTree*/
	for (i = 0; i < wt -> n; i++) {

		wf = &(wt -> tab)[i];
		from = hashToPath(wf -> hash);

		/*On cherche l'extension .t*/
		point = strrchr(wf -> hash, '.');
		t = strrchr(wf -> hash, 't');

		/* Si l'extension .t est presente alors les pointeurs sont consecutifs*/
		if (point != NULL && t != NULL && point == t - 1) {
			wtree = ftwt(from);

			sprintf(new_path, "%s/%s", path, wf -> name);

			/*On creer le repertoire du nouveau WorkTree*/
			sprintf(cmd, "mkdir -p %s", new_path);
			system(cmd);

			restoreWorkTree(wtree, new_path);

			memset(cmd, 0, 256);
			memset(new_path, 0, 256);
			freeWorkTree(wtree);
		}

		else {

			sprintf(to, "%s/%s", path, wf -> name);
			cp(to, from);
			setMode(wf -> mode, to);

			memset(to, 0, 256);
		}
		
		free(from);
	}
}