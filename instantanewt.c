#include "instantanewt.h" 


/*Creer un fichier temporaire avec la representation du WorkTree et qui renvoie son hash*/
char* blobWorkTree(WorkTree* wt){
	/*Initialisation et declaration des variables*/
	static char template[] = "/tmp/myfileXXXXXX";

	char fname[1000], cmd[1000+TAILLE_MAX], *res, *path, *buffer, *tmp;

	/*Sauvegarde du template dans une nouvelle string et creation d'un fichier temporaire cree a partir du template*/
	strcpy (fname, template);
	int descripteur = mkstemp(fname);

	if (descripteur == -1) {
		fprintf(stderr, "Erreur lors de la creation d'un fichier temporaire pour la fonction blobWorkTree !\n");
		
		/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
		sprintf(cmd, "rm %s", fname);
		system(cmd);

		exit(1);
	}

	wttf(wt, fname);
	tmp = sha256file(fname);
	res = malloc(sizeof(char)*256);
	memset(res, 0, 256);
	snprintf(res, 65, "%s", tmp);
	buffer = hashToPath(res);
	path = malloc(sizeof(char)*strlen(buffer)+3);
	sprintf(path, "%s.t", buffer);
	cp(path, fname);

	free(buffer);
	free(path);
	free(tmp);
	close(descripteur);

	return res;
}

/*Creer recursivement tous les instantanes du WorkTree et enfin l'instantane de celui-ci*/
char* saveWorkTree(WorkTree* wt, char* path) {
	/*Initialisation des variables*/
	WorkFile* wf;
	int i;
	List* l;
	WorkTree* wtree;
	char buffer[256], *new_path;

	memset(buffer, 0, 256);

	/*Parcours du tableau de WorkFile du WorkTree*/
	for (i = 0; i < wt -> n; i++) {
		wf = wt -> tab+i;

		/*Si la cibles est un fichier standard*/
		if (is_regular_file(wf -> name) == 0) {
			new_path = pathConcat(path, wf->name);
			blobFile(new_path);
			wf -> mode = getChmod(new_path);
			wf -> hash = sha256file(new_path);
			free(new_path);
		}

		/*Si la cible est un repertoire*/
		else if (is_directory(wf -> name) == 0) {
			printf("%s\n", wf->name);
			/*On recupere les noms des fichiers*/
			new_path = pathConcat(path, wf->name);
			l = listdir(new_path);
			wtree = initWorkTree();

			/*On reconstruit le worktree*/
			for (Cell* cell = *l; cell != NULL; cell = cell -> next) {
				if (cell -> data[0] == '.') {
					continue;
				}
				appendWorkTree(wtree, cell -> data, NULL, 0);
			}

			/*On construit le path du nouveau WorkTree*/
			sprintf(buffer, "%s/%s", path, new_path);

			/*On appelle recursivement la fonction pour ce nouveau WorkTree*/
			wf -> hash = saveWorkTree(wtree, buffer);
			wf -> mode = getChmod(new_path);

			/*Libération des variables*/
			freeList(l);
			freeWorkTree(wtree);
			free(new_path);
		}
	}

	char* res = blobWorkTree(wt);

	freeWorkTree(wt);

	return res;
}

/*Restore l'arborescence du WorkTree et les fichiers associes*/
void restoreWorkTree(WorkTree* wt, char* path) {
	/*Declaration des variables*/
	char* new_path, *copyPath, *hash, *buffer;
	WorkTree* new_wt;

	/*Parcours du tableau de WorkFile du WorkTree*/
	for (int i = 0; i < wt -> n; i++) {
		new_path = pathConcat(path, wt->tab[i].name);
		copyPath = hashToPath(wt->tab[i].hash);
		hash = wt->tab[i].hash;

		/*Si c'est un WorkTree*/
		if (isWorkTree(hash) == 1) {
			buffer = malloc(sizeof(char) * (strlen(hash) + 2));
			sprintf(buffer, "%s.t", hash);
			new_wt = ftwt(copyPath);
			restoreWorkTree(new_wt, new_path);
			setMode(getChmod(copyPath), new_path);
			free(buffer);
			freeWorkTree(new_wt);
		}
		else if (isWorkTree(hash) == 0) {
			cp(new_path, copyPath);
			setMode(getChmod(copyPath), new_path);
		}
	}
}
