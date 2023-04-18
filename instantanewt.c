#include "instantanewt.h" 


/*Creer un fichier temporaire avec la representation du WorkTree et qui renvoie son hash*/
char* blobWorkTree(WorkTree* wt){
	/*Initialisation et declaration des variables*/
	static char template[] = "/tmp/myfileXXXXXX";

	char fname[1000], cmd[1000+TAILLE_MAX], *res;

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
	close(descripteur);

	/*Sauvegarde du WorkTree dans le fichier temporaire*/
	wttf(wt, fname);

	/*Recuperation du chemin ou sauvegarder le WorkTree*/
	res = sha256file(fname);
	char *buffer = hashToPath(res);
	int taille = strlen(buffer)+3;
	char path[taille];
	memset(path, 0, taille);
	sprintf(path, "%s.t", buffer);

	/*Sauvegarde du WorkTree*/
	cp(path, fname);

	/*Ecriture dans un buffer de la commande pour supprimer le fichier temporaire*/
	sprintf(cmd, "rm %s", fname);
	system(cmd);
	
	free(buffer);

	return res;
}

/*Creer recursivement tous les instantanes du WorkTree et enfin l'instantane de celui-ci*/
char* saveWorkTree(WorkTree* wt, char* path) {
	/*Initialisation des variables*/
	WorkFile* wf;
	int i;
	List* l;
	WorkTree* wtree;
	char *new_path;

	/*Parcours du tableau de WorkFile du WorkTree*/
	for (i = 0; i < wt -> n; i++) {
		wf = wt -> tab+i;

		new_path = pathConcat(path, wf->name);

		/*Si la cibles est un fichier standard*/
		if (is_regular_file(new_path) == 0) {
			blobFile(new_path);
			wf -> mode = getChmod(new_path);
			if (wf -> hash != NULL)
				free(wf->hash);
			wf -> hash = sha256file(new_path);
		}

		/*Si la cible est un repertoire*/
		else if (is_directory(new_path) == 0) {
			/*On recupere les noms des fichiers*/
			l = listdir(new_path);
			wtree = initWorkTree();

			/*On reconstruit le worktree*/
			for (Cell* cell = *l; cell != NULL; cell = cell -> next) {
				if (cell -> data[0] == '.') {
					continue;
				}

				char* nom_fich = pathConcat(new_path, cell->data);
				char* hash = NULL;
				mode_t mode = 0;
				if (is_regular_file(nom_fich) == 0) {
					hash = sha256file(nom_fich);
					mode = getChmod(nom_fich);
				}		
				
				appendWorkTree(wtree, cell -> data, hash, mode);

				if(hash != NULL) {
					free(hash);
				}
				free(nom_fich);
			}

			/*On appelle recursivement la fonction pour ce nouveau WorkTree*/
			wf -> hash = saveWorkTree(wtree, new_path);
			wf -> mode = getChmod(new_path);

			/*Libération des variables*/
			freeList(l);
		}

		free(new_path);
	}

	char* res = blobWorkTree(wt);

	freeWorkTree(wt);

	return res;
}

/*Restore l'arborescence du WorkTree et les fichiers associes*/
void restoreWorkTree(WorkTree* wt, char* path) {
	/*Declaration des variables*/
	char* new_path, *hashPath, *basename;
	WorkTree* new_wt;
	int chmod, is_wt;

	/*Parcours du tableau de WorkFile du WorkTree*/
	for (int i = 0; i < wt -> n; i++) {
		if (wt->tab[i].hash == NULL || wt->tab[i].name == NULL)
			continue;

		/*On recupere le nom du fichier (sans le chemin) pour etre sur de construire le fichier au bon endroit dans tous les cas*/
		basename = baseName(wt->tab[i].name);
		if (strcmp("myGit", basename) == 0) {
			free(basename);
			continue;
		}

		/*On recupere le hash du fichier*/
		hashPath = hashToPath(wt->tab[i].hash);
		
		/*On creer le chemin vers ce nouveau fichier*/
		new_path = pathConcat(path, basename);
		free(basename);
		
		/*On recupere le mode du fichier*/
		chmod = wt->tab[i].mode;

		is_wt = isWorkTree(hashPath);
		/*Si c'est un WorkTree*/
		if (is_wt == 1) {
			/*On change le nom du nouveau path pour qu'il ait l'extension .t des WorkTree*/
			int taille = strlen(hashPath) + 2;
			char buffer[taille];
			memset(buffer, 0, taille);
			sprintf(buffer, "%s.t", hashPath);

			/*On creer un nouveau WorkTree pour appeler la fonction recursivement*/
			new_wt = ftwt(buffer);

			/*Comme la fonction cp s'occupe deja de creer les repertoires parents d'un fichier lorsqu'ils n'existent pas, on n'a besoin de le creer manuellement ici pour changer le mode.*/

			/*On restore recursivement le WorkTree*/
			restoreWorkTree(new_wt, new_path);

			/*On change quand meme le mode du repertoire*/
			setMode(chmod, new_path);

			freeWorkTree(new_wt);
		}
		else if (is_wt == 0) {
			/*On recreer le fichier*/
			cp(new_path, hashPath);

			/*On change son mode*/
			setMode(chmod, new_path);
		} else {
			fprintf(stderr, "Impossible de retrouver les informations de %s, veuillez verifier l'integriter des fichiers du commit !\n", hashPath);
		}

		free(hashPath);
		free(new_path);
	}
}
