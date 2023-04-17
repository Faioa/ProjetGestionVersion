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
#include "ref.h"
#include "commit.h"
#include "branch.h"
#include "check.h"
#include "fusion.h"

int main(int argc, char* argv[]) {

	if (argc > 1 && strcmp(argv[1], "init") == 0) {
		/*On initialise les references par defaut*/
 		initRefs();
 		initBranch();
 		printf("Depot local initialise !\n");
 		return 0;
	}


	if (argc > 1 && strcmp(argv[1], "refs-list") == 0) {
		/*On recupere toutes les references et on affiche la liste*/
		if (file_exists(".refs")) {
			List* L = listdir(".refs");
			printf("REFS :\n");

			/*Parcours et affichage de la liste*/
			for (Cell* ptr = *L; ptr != NULL; ptr = ptr -> next) {
				if ( ptr -> data[0] == '.' )
					continue;

				/*On recupere le hash du dernier commit de la reference*/
				char* content = getRef(ptr -> data);

				printf("−%s\t%s\n" , ptr -> data, content);

				free(content);
			}

			freeList(L);

			return 0;
		} else {
			fprintf(stderr, "Il n'y a pas de reference.\nUtilisez './myGit help' pour plus d'informations !\n");
			
			return 1;
		}
	}


	if (argc > 1 && strcmp(argv[1], "create-refs") == 0) {
		if (argc > 3) {
			/*Si il y a le bon nombre d'arguments, on creer/modifie la reference*/
			createUpdateRef(argv[2], argv[3]);

			printf("La reference %s a ete creee avec succes !\n", argv[2]);
			
			return 0;
		}
		else {
			fprintf(stderr, "Veuillez renseigner le nom de la reference et son hash.\nUtilisez './myGit help' pour plus d'informations !\n");
			
			return 1;
		}
	}
	

	if (argc > 1 && strcmp(argv[1], "delete-ref") == 0) {
		/*Si la reference existe, on la supprime*/
		if (argc > 2) {
			if (branchExists(argv[2]) == 1) {
				deleteRef(argv[2]);

				printf("La reference %s a ete supprimee avec succes !\n", argv[2]);

				return 0;
			}
			else {
				fprintf(stderr, "La reference %s existe deja !\nUtilisez './myGit help' pour plus d'informations !\n", argv[2]);
				
				return 1;
			}
		}
		else {
			fprintf(stderr, "Veuillez renseigner le nom de la reference.\nUtilisez './myGit help' pour plus d'informations !\n");
			
			return 1;
		}
	}


	if (argc > 1 &&  strcmp(argv[1], "add") == 0) {
		/*On ajoute tous les fichiers donnes en parametres a la zone de preparation*/
		if (argc > 2) {
			for (int i = 2; i < argc; i ++) {
				myGitAdd(argv[i]);
			}

			printf("Les fichiers ont ete ajoutes a la zone de preparation avec succes !\n");

			return 0;
		}
		else {
			fprintf(stderr, "Veuillez renseigner le(s) fichier(s) a ajouter.\nUtilisez './myGit help' pour plus d'informations !\n");
			
			return 1;
		}
	}


	if (argc > 1 &&  strcmp(argv[1], "clear-add") == 0) {
		/*On supprime le fichier si il existe*/
		if (file_exists(".add") == 1) {
			system("rm .add");

			printf("La zone de preparation a ete videe avec succes !\n");

			return 0;
		} else {
			fprintf(stderr, "La zone de preparation n'existe pas !\nUtilisez './myGit help' pour plus d'informations !\n");
			
			return 1;
		}
	}


	if (argc > 1 && strcmp(argv[1], "add-list") ==0) {
		/*On affiche le contenu de la zone de preparation*/
		if (file_exists(".add")) {
			/*On recupere le WorkTree associe a la zone de preparation*/
			WorkTree* wt = ftwt(".add");

			/*On recupere sa representation en chaine de caracteres (= contenu du fichier de .add)*/
			char* swt = wtts(wt);
			printf("Zone de preparation :\n%s\n", swt);

			freeWorkTree(wt);
			free(swt);

			return 0;
		} else {
			fprintf(stderr, "La zone de preparation n'existe pas !\nUtilisez './myGit help' pour plus d'informations !\n");
			
			return 1;
		}
	}


	if (argc > 1 && strcmp(argv[1], "commit") == 0) {
		/*On verifie la presence d'un message pour le commit*/
		if (argc > 4 && strcmp(argv[3], "-m") == 0) {
			/*Si la branche renseignee n'existe pas*/
			if (branchExists(argv[2]) != 1) {
				fprintf(stderr, "La branche %s n'est pas valide !\nUtilisez './myGit help' pour plus d'informations !\n", argv[2]);

				return 1;
			} else {
				myGitCommit(argv[2], argv[4]);

				printf("Le commit sur la branche %s s'est bien passe !\n", argv[2]);

				return 0;
			}
		} else if (argc > 2) {
			/*Si la branche renseignee n'existe pas*/
			if (branchExists(argv[2]) != 1) {
				fprintf(stderr, "La branche %s n'est pas valide !\nUtilisez './myGit help' pour plus d'informations !\n", argv[2]);

				return 1;
			} else {
				myGitCommit(argv[2], NULL);

				printf("Le commit sur la branche %s s'est bien passe !\n", argv[2]);

				return 0;
			}
		} else {
			fprintf(stderr, "Veuillez renseigner le nom du commit [et le message].\nUtilisez './myGit help' pour plus d'informations !\n");
			
			return 1;
		}
	}


	if (argc > 1 && strcmp(argv[1], "get-current-branch") == 0) {
		/*On recupere le contenu du fichier .current_branch*/
		char* branch = getCurrentBranch();
		if (branch == NULL) {
			fprintf(stderr, "Le fichier .current_branch n'existe pas !\nUtilisez './myGit help' pour plus d'informations !\n");

			return 1;
		} else {
			printf("La branche courante est %s\n", branch);

			free(branch);

			return 0;
		}
	}


	if (argc > 1 && strcmp(argv[1], "branch") == 0) {
		/*On verifie la presence d'un nom de branche*/
		if (argc >2) {
			/*On verifie que ce nom est valide*/
			if (branchExists(argv[2]) == 0) {
				/*On creer la branche*/
				createBranch(argv[2]);

				printf("La branche %s a ete creee avec succes !\n", argv[2]);

				return 0;
			}
			else {
				fprintf(stderr, "La branche existe deja.\nUtilisez './myGit help' pour plus d'informations !\n");

				return 1;
			}
		} else {
			fprintf(stderr, "Veuillez preciser le nom de la branche a creer !\nUtilisez './myGit help' pour plus d'informations !\n");

			return 1;
		}
	}


	if (argc > 1 && strcmp(argv[1], "branch-print") == 0) {
		/*On verifie la presence d'un nom de branche*/
		if (argc > 2) {
			/*On verifie que ce nom soit valide*/
			if (branchExists(argv[2]) == 0) {
				fprintf(stderr, "La branche n'existe pas !\nUtilisez './myGit help' pour plus d'informations !\n");

				return 1;
			}
			else {
				printf("Branche %s\n", argv[2]);
				printBranch(argv[2]);

				return 0;
			}
		} else {
			fprintf(stderr, "Veuillez renseigner le nom de la branche.\nUtilisez './myGit help' pour plus d'informations !\n");

			return 1;
		}
	}


	if (argc > 1 && strcmp(argv[1], "checkout-branch") == 0) {
		/*On verifie la presence d'un nom de branche valide*/
		if (argc > 2 && branchExists(argv[2]) == 1) {
			myGitCheckoutBranch(argv[2]);

			return 0;
		}
		else {
			fprintf(stderr, "Veuillez renseigner le nom d'une branche existante.\nUtilisez './myGit help' pour plus d'informations !\n");

			return 1;
		}
	}


	if (argc > 1 && strcmp(argv[1], "checkout-commit") == 0) {
		/*On verifie la presence d'un parametre*/
		if (argc > 2) {
			/*La fonction fait le traitement toute seule*/
			myGitCheckoutCommit(argv[2]);

			return 0;
		}
		else {
			fprintf(stderr, "Veuillez renseigner le nom du commit.\nUtilisez './myGit help' pour plus d'informations !\n");

			return 1;
		}
	}


	if (argc > 1 && strcmp(argv[1], "merge") == 0) {
		/*On verifie la presence d'un parametre*/
		if (argc > 2) {
			/*On verifie que ce parametre correspond a une branche valide*/
			if (branchExists(argv[2]) != 1) {
				fprintf(stderr, "La branche %s n'existe pas !\n", argv[2]);
				return 1;
			}

			/*On verifie que la branche ne soit pas la meme que la branche courante*/
			char* current = getCurrentBranch();
			if (strcmp(argv[2], current) == 0) {
				printf("%s est deja la branche courante, il n'y a rien a fusionner !\n", argv[2]);
				free(current);
				return 0;
			}

			/*On tente d'effectuer la fusion et on recupere la liste des conflits si il y a des collisions*/
			List* conflicts;
			if (argc > 3) {
				conflicts = merge(argv[2], argv[3]);
			} else {
				conflicts = merge(argv[2], NULL);
			}
			if (conflicts == NULL) {
				free(current);
				return 0;
			}

			/*On s'occupe des collisions*/
			int sortie = 0;
			int choix_collisions = 0;
			List* liste1 = NULL;
			List* liste2 = NULL;
			while (sortie != 1) {
				printf("Il y a des collisions ! Veuillez choisir une façon de les gerer :\n");
				printf("1 - Garder les fichiers de la branche courante %s\n", current);
				printf("2 - Garder les fichiers de la branche distante %s\n", argv[2]);
				printf("3 - Resoudre les conflits manuellement\n");
				printf("4 - Quitter le programme\n");
				scanf(" %d", &choix_collisions);

				switch (choix_collisions) {
					case 1:
						/*On creer le Commit de suppression pour la branche distante*/
						createDeletionCommit(argv[2], conflicts, "Suppression pour merge");

						/*On fait la fusion*/
						if (argc > 3)
							merge(argv[2], argv[3]);
						else
							merge(argv[2], NULL);

						sortie = 1;
						break;

					case 2:
						/*On creer le Commit de suppression pour la branche courante*/
						createDeletionCommit(current, conflicts, "Suppression pour merge");

						if (argc > 3)
							merge(argv[2], argv[3]);
						else
							merge(argv[2], NULL);

						sortie = 1;
						break;

					case 3:
						/*On cherche a savoir quel fichier garder sur quelle branche*/
						liste1 = initList();
						liste2 = initList();
						int choix_branche;
						printf("Veuillez selectionner une option pour chaque conflit :\n");
						printf("1 - Garder le fichier sur la branche courante %s\n", current);
						printf("2 - Garder le fichier sur la branche distante %s\n", argv[2]);

						/*Parcours de la liste des conflits*/
						for (Cell *cell = *conflicts; cell != NULL; cell = cell -> next) {
							choix_branche = 0;
							int sortie_branche = 0;
							printf("Conflit : %s\n", cell -> data);
							/*Tant que la valeur rentree n'est pas bonne on demande une option a l'utilisateur (attention a autre chose qu'un int -> boucle infinie)*/
							while (sortie_branche != 1) {
								scanf(" %d", &choix_branche);
								if (choix_branche == 1 || choix_branche == 2)
									sortie_branche = 1;
								else
									printf("Veuillez rentrer une valeur correcte !\n");
							}

							/*liste1 pour garder sur la branche courante*/
							if (choix_branche == 1)
								insertFirst(liste1, buildCell(cell->data));
							/*liste2 pour garder sur la branche distante*/
							else
								insertFirst(liste2, buildCell(cell->data));
						}

						/*Creation des Commits de suppression (tous les elements qui ne sont pas dans la liste donnee en parametre sont gardes)*/
						createDeletionCommit(argv[2], liste1, "Suppression pour merge");
						createDeletionCommit(current, liste2, "Suppression pour merge");

						if (argc > 3)
							merge(argv[2], argv[3]);
						else
							merge(argv[2], NULL);

						sortie = 1;
						break;

					case 4:
						/*Sortie du programme*/
						sortie = 1;
						break;

					default:
						break;
				}
			}

			/*Liberation des ressources*/
			free(current);
			freeList(conflicts);
			if (liste1 != NULL)
				freeList(liste1);
			if (liste2 != NULL)
				freeList(liste2);

			return 0;
		} else {
			fprintf(stderr, "Usage : ./myGit merge <nom_branche_distante> [message]\nUtilisez './myGit help' pour plus d'informations !\n");
			
			return 1;
		}
	}


	if (argc > 1 && strcmp(argv[1], "help") == 0) {
		printf("A FAIRE EN SPEED !\n");
		
		return 0;
	}

	printf("Utilisez './myGit help' pour avoir des informations sur comment utiliser le programme !\n");

	return 0;
}
