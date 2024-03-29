#include "list_utility.h"


/*Initialise une liste vide et renvoie un pointeur dessus*/
List* initList() {
	/*Initialisation de la variable et allocation*/
	List* res = (List*) malloc(sizeof(List));
	
	if (res == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'une List pour la fonction initList !\n");
		exit(1);
	}
	/*initialisation de la liste vide */
	*res=NULL;
	
	return res;
}

/*Construit une cellule et initialise son champs 'data' avec la chaine de caracteres passee en parametre*/
Cell* buildCell(char* ch) {
	/*Initialisation de la variable et allocation*/
	Cell* res = (Cell*) malloc(sizeof(Cell));
	
	if (res == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'une chaine de caracteres pour la fonction buildCell !\n");
		exit(1);
	}
	
	/*Remplissage des champs de la cellule*/
	res -> data = strdup(ch);
	res -> next = NULL;
	
	return res;
}

/*Insere la cellule C en tete de la liste L)*/
void insertFirst(List* L, Cell* C) {
	/*Initialisation du champ 'next' de C a la tete de la liste L et changement de la tete de L a C*/
	C -> next = *L;
	*L = C;
	
	return;
}

/*Retourne le champs 'data' d'une cellule passee en parametre*/
char* ctos(Cell* c) {

	return c -> data;
}

/*Retoune une chaine de caractere contenant tous les champs 'data' des cellules de la liste avec '|' comme separateur*/
char* ltos(List* L) {
	/*Initialisation et declaration des variables*/
	char* res;
	int taille = 0;
	List tmp = *L;

	/*Premier parcours de la liste pour recuperer la taille de la zone memoire a allouer (+1 pour '\0'*/
	while (tmp != NULL) {
		taille += strlen(tmp -> data);
		if (tmp -> next != NULL) {
			taille ++;
		}
		tmp = tmp -> next;
	}

	/*Si la liste est vide, on retourne la chaine de caractere vide*/
	if (taille == 0) {
		return "";
	}

	/*Initialisation des variables*/
	res = (char *) malloc(sizeof(char) * (taille + 1));
	if (res == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'une chaine de caracteres pour la fonction ltos !\n");
		exit(1);
	}
	tmp = *L;

	/*Second parcours de la liste pour recuperer les champs 'data'*/
	while (tmp != NULL) {
		strcat(res, tmp -> data);
		if(tmp -> next != NULL) {
			strcat(res, "|");
		}
		tmp = tmp -> next;
	}

	return res;
}

/*Retourne le champ 'data' de la i-ieme cellule de la liste si cette cellule existe, NULL sinon*/
Cell* listGet(List* L, int i) {
	/*Initialisation et déclaration des variables*/
	int j = 0;
	List tmp = *L;

	/*Parcours de la liste jusqu'à atteindre l'element souhaite ou la fin de la liste*/
	while (j < i && tmp != NULL) {
		tmp = tmp -> next;
		j++;
	}

	return tmp;
}

/*Retourne un pointeur sur la cellule dont le champ 'data' est egal au parametre str si elle existe, NULL sinon*/
Cell* searchList(List* L, char* str) {
	/*Initialisation de la variable*/
	List tmp = *L;

	/*Parcous de la liste jusqu'a trouver l'element ou atteindre la fin de la liste*/
	while (tmp != NULL) {
		if (strcmp(tmp -> data, str) == 0) {
			return tmp;
		}
		tmp = tmp -> next;
	}
	
	return NULL;
}

/*Retourne un pointeur sur une liste lue à partir d'une chaine de caractere (avec le separateur '|')*/
List* stol(char* s) {
	/*Initialisation des variables*/
	List* res = initList();
	Cell* cell = NULL;

	int taille = strlen(s) + 1;
	char *buffer = (char*) malloc(sizeof(char) * taille);
	if (buffer == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'une chaine de caracteres (buffer) pour la fonction stol !\n");

		exit(1);
	}

	char *save = (char*) malloc(sizeof(char) * taille);
	if (save == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation d'une chaine de caracteres (save) pour la fonction stol !\n");

		free(buffer);
		exit(1);
	}

	memset(buffer, 0, taille);
	int i = 0;

	/*Parcours de l'entierete de la chaine de caracteres*/
	while (s[i] != '\0') {
		/*Si on rencontre le separateur '|' on creer une cellule avec ce qu'on à lu jusqu'a maintenant et on l'insere dans la liste*/
		if (s[i++] == '|') {
			cell = buildCell(buffer);
			insertFirst(res, cell);
			memset(buffer, 0, taille);
		}
		/*Sinon on sauvegarde le caractere*/
		else {
			strcpy(save, buffer);
			sprintf(buffer, "%s%c", save, s[i - 1]);
		}
	}
	
	free(buffer);
	free(save);
	return res;
}

/*Sauvegarde la liste passee en parametre dans le fichier path*/
void ltof(List* L, char* path) {
	/*Initialisation de la variable*/
	char* str = ltos(L);
	FILE* f;

	/*Ouverture du fichier*/
	f = fopen(path, "w");

	/*Ecriture de la liste dans le fichier*/
	fprintf(f, "%s", str);

	/*Fermeture du fichier*/
	fclose(f);
	
	return;
}

/*Retourne une liste lue a partir du fichier path*/
List* ftol(char* path) {
	/*Initialisation des variables*/
	FILE* f;
	char buffer[256];
	List* liste;

	/*Ouverture du fichier*/
	f = fopen(path, "r");
	if (f == NULL) {
		fprintf(stderr, "Erreur lors de l'ouverture du fichier %s pour la fonction ftol !\n", path);
		exit(1);
	}

	/*Recuperation de la chaine de caracteres representant la liste*/
	fgets(buffer, 256, f);

	/*Recuperation de la liste a partir de la chaine de caracteres*/
	liste = stol(buffer);

	/*Fermeture du fichier*/
	fclose(f);

	return liste;	
}

/*Libere une cellule*/
void freeCell(Cell* cell) {
	/*Liberation de la chaine de caracteres contenue dans le champs 'data' de la structure*/
	free(cell -> data);

	/*Liberation de la cellule*/
	free(cell);

	return;
}

/*Libere une liste de cellule*/
void freeList(List* list) {
	if (list != NULL) {
		/*Initialisation des variables*/
		List tmp = *list, suiv;

		/*Parcours de la liste et liberation des cellules croisees*/
		while (tmp != NULL) {
			suiv = tmp -> next;
			freeCell(tmp);
			tmp = suiv;
		}

		free(list);
	}

	return;
}

/*Affiche la liste*/
void afficheList(List* list) {
	/*Initialisation de la variable*/
	List tmp = *list;

	/*Parcours de la liste et affichage des champs 'data' des cellules croisees*/
	while (tmp != NULL) {
		printf("%s\n", tmp -> data);
		tmp = tmp -> next;
	}

	return;
}