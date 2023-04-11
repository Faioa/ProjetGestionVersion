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
				myGitCommit(argv[2], argv[4]);
			} else if (argc > 2) {
				myGitCommit(argv[2], NULL);
			} else {
			printf("Veuillez renseigner le nom du commit [et le message].\n");
			}
	}

	if (argc > 1 && strcmp(argv[1], "get-current-branch") == 0) {
		char* branch = getCurrentBranch();
		if (branch == NULL) {
			printf("erreur lors de l'ouverture du fichier current_branch");
		} else {
			printf("%s", branch);
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
		if (argc > 0) {
			if (!branchExists(argv[2]) == 0) {
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

	return 0;
}