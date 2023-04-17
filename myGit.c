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
 		initRefs();
 		initBranch();
	}

	if (argc > 1 && strcmp(argv[1], "refs-list") == 0) {
		if (file_exists(".refs")) {
			List* L = listdir(".refs");
			printf("REFS :\n");
			for (Cell* ptr = *L; ptr != NULL; ptr = ptr -> next) {
				if ( ptr -> data[0] == '.' )
					continue;
				char* content = getRef(ptr -> data);
				printf("−%s\t%s\n" , ptr -> data, content);
				free(content);
			}
			freeList(L);
		} else {
			printf("Il n'y a pas de reference.\n");
		}
	}

	if (argc > 1 && strcmp(argv[1], "create-refs") == 0) {
		if (argc > 3)
			createUpdateRef(argv[2], argv[3]);
		else
			printf("Veuillez renseigner le nom de la reference et son hash.\n");
	}
	
	if (argc > 1 && strcmp(argv[1], "delete-ref") == 0) {
		if (argc > 2)
			deleteRef(argv[2]);
		else
			printf("Veuillez renseigner le nom de la reference.\n");
	}

	if (argc > 1 &&  strcmp(argv[1], "add") == 0) {
		if (argc > 2) {
			for (int i = 2; i < argc; i ++) {
				myGitAdd(argv[i]);
			}
		}
		else {
			printf("Veuillez renseigner le(s) fichier(s) a ajouter.\n");
		}
	}

	if (argc > 1 &&  strcmp(argv[1], "clear-add") == 0) {
		system("rm .add");
	}

	if (argc > 1 && strcmp(argv[1], "add-list") ==0) {
		printf("Zone de preparation : \n");
		if (file_exists(".add")) {
			WorkTree* wt = ftwt(".add");
			char* swt = wtts(wt);
			printf("%s\n", swt);
			freeWorkTree(wt);
			free(swt);
		}
	}

	if (argc > 1 && strcmp(argv[1], "commit") == 0) {
			if (argc > 4 && strcmp(argv[3], "-m") == 0) {
				if (branchExists(argv[2]) != 1) {
					fprintf(stderr, "La branche %s n'est pas valide !\n", argv[2]);
				} else {
					myGitCommit(argv[2], argv[4]);
				}
			} else if (argc > 2) {
				if (branchExists(argv[2]) != 1) {
					fprintf(stderr, "La branche %s n'est pas valide !\n", argv[2]);
				} else {
					myGitCommit(argv[2], NULL);
				}
			} else {
			printf("Veuillez renseigner le nom du commit [et le message].\n");
			}
	}

	if (argc > 1 && strcmp(argv[1], "get-current-branch") == 0) {
		char* branch = getCurrentBranch();
		if (branch == NULL) {
			printf("Erreur lors de l'ouverture du fichier current_branch !\n");
		} else {
			printf("%s\n", branch);
			free(branch);
		}
	}

	if (argc > 1 && strcmp(argv[1], "branch") == 0) {
		if (argc >2) {
			if (!branchExists(argv[2])) {
				createBranch(argv[2]);
			}
			else {
				printf("La branche existe deja.\n");
			}
		}
	}

	if (argc > 1 && strcmp(argv[1], "branch-print") == 0) {
		if (argc > 2) {
			if (branchExists(argv[2]) == 0) {
				printf("La branche n'existe pas.\n");
			}
			else {
				printBranch(argv[2]);
			}
		} else {
			printf("Veuillez renseigner le nom de la branche.\n");
		}
	}

	if (argc > 1 && strcmp(argv[1], "checkout-branch") == 0) {
		if (argc > 2 && branchExists(argv[2]) == 1) {
			myGitCheckoutBranch(argv[2]);
		}
		else {
			printf("Veuillez renseigner le nom d'une branche existante.\n");
		}
	}

	if (argc > 1 && strcmp(argv[1], "checkout-commit") == 0) {
		if (argc > 2) {
			myGitCheckoutCommit(argv[2]);
		}
		else {
			printf("Veuillez renseigner le nom du commit.\n");
		}
	}

	if (argc > 1 && strcmp(argv[1], "merge") == 0) {
		if (argc > 2) {
			if (branchExists(argv[2]) != 1) {
				fprintf(stderr, "La branche %s n'existe pas !\n", argv[2]);
				return 1;
			}

			char* current = getCurrentBranch();
			if (strcmp(argv[2], current) == 0) {
				printf("%s est deja la branche courante, il n'y a rien a fusionner !\n", argv[2]);
				free(current);
				return 0;
			}

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
						sortie = 1;
						break;

					default:
						break;
				}
			}

			free(current);
			freeList(conflicts);
			if (liste1 != NULL)
				freeList(liste1);
			if (liste2 != NULL)
				freeList(liste2);
		} else {
			fprintf(stderr, "Usage : ./myGit merge <nom_branche_distante> [message]\n");
			return 1;
		}
	}

	return 0;
}