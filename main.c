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

	if (strcmp(argv [1], "init") == 0) {
 		initRefs();
 		initBranch();
	}

	if (strcmp(argv[1], "refs-list") == 0) {
 		printf( "REFS : \n" );
		if (file_exists(".refs")) {
			List* L = listdir(".refs");
			for (Cell* ptr = *L; ptr != NULL; ptr = ptr -> next) {
				if ( ptr -> data[0] == '.' )
					continue;
				char* content = getRef( ptr -> data );
				printf ( "− %s \t %s \n" , ptr -> data , content );
			}
		}
	}

	if ( strcmp(argv[1] ,"create-refs") == 0) {
		createUpdateRef(argv[2], argv[3]);
	}
	
	if ( strcmp(argv[1] , "delete−ref") == 0) {
		deleteRef(argv[2]);
	}

	if ( strcmp(argv[1], "add") == 0) {
		for (int i = 2; i < argc; i ++) {
			myGitAdd(argv[i]);
		}
	}

	if ( strcmp(argv[1], "clear-add") == 0) {
		system("rm .add");
	}

	if ( strcmp(argv [2], "add-list") ==0) {
		printf("Zone de preparation : \n");
		if (file_exists(".add")) {
			WorkTree* wt = ftwt(".add");
			printf("%s\n", wtts(wt));
		}
	}

	if (strcmp(argv[1], "commit") == 0) {
		if (strcmp(argv[3], "-m") == 0) {
			myGitCommit(argv[2], argv[4]);
		} else {
			myGitCommit(argv[2], NULL);
		}
	}

	if (strcmp(argv[1], "get-current-branch") == 0) {
		printf ("%s", getCurrentBranch());
	}

	if (strcmp(argv[1], "branch") == 0) {
		if (!branchExists(argv[2])) {
			createBranch(argv[2]);
		}
		else {
			printf("La branche existe deja.\n");
		}
	}

	if (strcmp(argv[1], "branch-print") == 0) {
		if (!branchExists(argv[2]) == 0) {
			printf("La branche n'existe pas.\n");
		}
		else {
			printBranch(argv[2]);
		}
	}

	if (strcmp(argv[1], "checkout-branch") == 0) {
		printf("La branche n'existe pas.\n");
	}
	else {
		myGitCheckoutBranch(argv[2]);
	}

	if (strcmp(argv[1], "checkout-commit") == 0) {
		myGitCheckoutCommit(argv[2]);
	}

	return 0;
}