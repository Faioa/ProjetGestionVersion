#include "instantanewt.h" 


/*Creer un fichier temporaire avec la representation du WorkTree et qui renvoie son hash*/
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
	free(wtree);

	return buffer;
}

/*Creer recursivement tous les instantanes du WorkTree et enfin l'instantane de celui-ci*/
/*char * saveWorkTree(WorkTree* wt, char* path){
	//cette fonction est probablement incorrecte
	for(int i=0;i<wt->n;i++){
		WorkFile * WF=wt->tab[i];
		if( is_regular_file(WF->name)){
			blobFile(WF->name);
			int mode=getChmod(path); 
			setMode(mode,path);
		}else{
			if(is_directory(WF->name)){
				List * l=listdir(WF->name);

				WorkFile* wf = NULL;
				WorkTree* newWt = initWorkTree();

				while(*l){
					wf = stwf((*l)->date);
					appendWorkTree(newWt, wf->name, wf->hash, wf->mode);
					freeWorkFile(wf);
				}

				saveWorkTree(newWt,path);
				int mode=getChmod(path); 
				setMode(mode,path);
			}
		}
	}

	char *hash=blobWorkTree(wt);

	return hash;



	//return "aie";
}

void restoreWorkTree(WorkTree* wt, char* path){
	//cette fonction est problement incorrecte 
	for(int i=0;i<wt->n;i++){
		WorkFile * WF=wt->tab[i];

		//cas sans ".t"
		if(){
			cp(path,WF->name);
			setMode(Wf->mode,path);
			//donner noms et autorisations j'ai pas bien compris
		}
		//cas avec ".t"
		if(){

			List * l=listdir(WF->name);

			WorkFile* wf = NULL;
			WorkTree* newWt = initWorkTree();

			while(*l){
				wf = stwf((*l)->date);
				appendWorkTree(newWt, wf->name, wf->hash, wf->mode);
				freeWorkFile(wf);
			}
			char newpath[256];
			strcpy(newpath,path);
			strcat(newpath,"/");
			strcat(newpath,WF->name);
			restoreWorkTree(newWt,newpath);

		}

	}



}*/