#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list_utility.h"
#include "file_utility.h"
#include "hash_utility.h"
#include "instantane.h"
#include "instantanewt.h"
#include "workfile.h"
#include "worktree.h"

/*Afficher le menu et renvoie l'option choisie*/
int menu(void) {
	/*Initialisation des variables*/
	int state = 0;
	char buffer[256];
	memset(buffer, 0, 256);

	/*Recuperation du choix tant que la donnee rentree n'est pas bonne et traitement de la demande*/
	while (strcmp(buffer, "-1") != 0) {
		if (state == 0) {
			/*Print des choix*/
			printf("Veuillez choisir une option (numero) :\n1 - Creer une liste\n2 - Inserer un element dans la liste\n3 - Rechercher un element dans la liste\n4 - Recuperer un element de la liste\n5 - Sauvegarder la liste\n6 - Supprimer la liste\n7 - Voir les operations sur les instantanes\n8 - Voir les fonctions utiles sur les fichiers\n-1 - Sortir du programme\n");

			/*Recuperation du choix*/
			scanf("%s", buffer);

			/*Analyse*/
			if(strcmp(buffer, "1") == 0) {
				return 1;
			}
			else if(strcmp(buffer, "2") == 0) {
				return 2;
			}
			else if(strcmp(buffer, "3") == 0) {
				return 3;
			}
			else if(strcmp(buffer, "4") == 0) {
				return 4;
			}
			else if(strcmp(buffer, "5") == 0) {
				return 5;
			}
			else if(strcmp(buffer, "6") == 0) {
				return 6;
			}
			else if(strcmp(buffer, "7") == 0) {
				state = 1;
				continue;
			}
			else if(strcmp(buffer, "8") == 0) {
				state = 2;
				continue;
			}

			/*Si on continue c'est que la donnee est egale a -1 ou est incorrecte*/
			continue;
		}

		else if (state == 1) {
			/*Print des choix*/
			printf("Veuillez choisir une option (numero) :\n1 - Realiser l'instantane d'un fichier\n2 - Realiser l'instantane d'un espace de travail\n3 - Restaurer un espace de travail a partir de son instantane\n4 - Voir les operations sur les listes\n5 - Voir les operations utiles sur les fichiers\n-1 - Sortir du programme\n");
			
			/*Recuperation du choix*/
			scanf("%s", buffer);

			/*Analyse*/
			if(strcmp(buffer, "1") == 0) {
				return 7;
			}
			else if(strcmp(buffer, "2") == 0) {
				return 8;
			}
			else if(strcmp(buffer, "3") == 0) {
				return 9;
			}
			else if(strcmp(buffer, "4") == 0) {
				state = 0;
				continue;
			}
			else if(strcmp(buffer, "5") == 0) {
				state = 2;
				continue;
			}

			/*Si on continue c'est que la donnee est egale a -1 ou est incorrecte*/
			continue;
		}

		else {
			/*Print des choix*/
			printf("Veuillez choisir une option (numero) :\n1 - Verifier que le fichier existe\n2 - Verifier que la cible est un fichier standard\n3 - Verifier que la cible est un repertoire\n4 - Copier la cible\n5 - Recuperer le repertoire parent de la cible\n6 - Recuperer le nom reel de la cible\n7 - Recuperer le mode de la cible\n8 - Changer le mode de la cible\n9 - Lister le contenu du repertoire cible\n10 - Voir les operations sur les listes\n11 - Voir les operations sur les instantanes\n-1 - Sortir du programme\n");

			/*Recuperation du choix*/
			scanf("%s", buffer);

			/*Analyse*/
			if(strcmp(buffer, "1") == 0) {
				return 10;
			}
			else if(strcmp(buffer, "2") == 0) {
				return 11;
			}
			else if(strcmp(buffer, "3") == 0) {
				return 12;
			}
			else if(strcmp(buffer, "4") == 0) {
				return 13;
			}
			else if(strcmp(buffer, "5") == 0) {
				return 14;
			}
			else if(strcmp(buffer, "6") == 0) {
				return 15;
			}
			else if(strcmp(buffer, "7") == 0) {
				return 16;
				continue;
			}
			else if(strcmp(buffer, "8") == 0) {
				return 17;
			}
			else if(strcmp(buffer, "9") == 0) {
				return 18;
			}
			else if(strcmp(buffer, "10") == 0) {
				state = 0;
				continue;
			}
			else if(strcmp(buffer, "11") == 0) {
				state = 1;
				continue;
			}

			/*Si on continue c'est que la donnee est egale a -1 ou est incorrecte*/
			continue;
		}
	}

	return 0;
}

int main(){

	int choix;

	choix = menu();

	switch (choix) {
		/*Creer une liste*/
		case 1:
			break;
		/*Inserer un element dans la liste*/
		case 2:
			break;
		/*Rechercher un element dans la liste*/
		case 3:
			break;
		/*Recuperer un element de la liste*/
		case 4:
			break;
		/*Sauvegarder la liste*/
		case 5:
			break;
		/*Supprimer la liste*/
		case 6:
			break;
		/*Realiser l'instantane d'un fichier*/
		case 7:
			break;
		/*Realiser l'instantane d'un espace de travail*/
		case 8:
			break;
		/*Restaurer un espace de travail a partir de son instantane*/
		case 9:
			break;
		/*Verifier que le fichier existe*/
		case 10:
			break;
		/*Verifier que la cible est un fichier standard*/
		case 11:
			break;
		/*Verifier que la cible est un repertoire*/
		case 12:
			break;
		/*Copier la cible*/
		case 13:
			break;
		/*Recuperer le repertoire parent de la cible*/
		case 14:
			break;
		/*Recuperer le nom reel de la cible*/
		case 15:
			break;
		/*Recuperer le mode de la cible*/
		case 16:
			break;
		/*Changer le mode de la cible*/
		case 17:
			break;
		/*Lister le contenu du repertoire cible*/
		case 18:
			break;
		/*Sortie du programme (verification de l'etat du programme et de toutes les variables dynamiques*/
		default:
			break;
	}

	return 0;
}