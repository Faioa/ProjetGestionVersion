#include "fusion.h"

WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts){
    if (wt1 == NULL) {
        return wt2;
    }
    if (wt2 == NULL) {
        return wt1;
    }

    if (*conflicts == NULL) {
        *conflicts = initList();
    }

    WorkTree* wt=initWorkTree();
    int i;
    
    /*On parcourt le premier WorkTree*/
    for(i = 0; i < wt1 -> n; i++){
        /*On verifie que le fichier n'est pas dans le second WorkTree*/
        int position = inWorkTree(wt2, wt1->tab[i].name);

        if (position != -1) {
            if(strcmp(wt1->tab[i].hash, wt2->tab[position].hash) != 0){
                /*Il y a un conflit*/
                insertFirst(*conflicts, buildCell(wt1->tab[i].name));
            } else {
                /*Il n'y a pas de conflit*/
                appendWorkTree(wt, wt1->tab[i].name, wt1->tab[i].hash, wt1->tab[i].mode);
            }
        }
    }

    /*On parcourt le second WorkTree*/
    for(i = 0; i < wt2 -> n; i++){
        /*On verifie que le fichier de soit pas deja dans la liste des conflits pour economiser du temps de calcul*/
        Cell* cell;
        if ((cell = searchList(*conflicts, wt2->tab[i].name)) == NULL) {
            appendWorkTree(wt, wt2->tab[i].name, wt2->tab[i].hash, wt2->tab[i].mode);
        }
    }

    return wt;
}

List* merge(char* remote_branch, char* message){

    if (branchExists(remote_branch) != 1) {
        fprintf(stderr, "La branche %s n'existe pas, impossible de fusionner a la branche courante !\n", remote_branch);
        exit(1);
    }

    /*Recuperation des references*/
    char* current_branch = getCurrentBranch();
    char* current_hash = getRef(current_branch);
    char* remote_hash = getRef(remote_branch);


    /*Recuperation des Commits*/
    char* current_path_commit = hashToPathCommit(current_hash);
    Commit* current_commit = ftc(current_path_commit);
    free(current_path_commit);

    char* remote_path_commit = hashToPathCommit(remote_hash);
    Commit* remote_commit = ftc(remote_path_commit);  
    free(remote_path_commit);


    /*Recuperation des hashs des WorkTrees*/
    char* current_wt_hash = commitGet(current_commit, "tree");

    char* remote_wt_hash = commitGet(remote_commit, "tree");


    /*Recuperation des chemins vers les WorkTrees et restoration de ces derniers*/
    char* current_wt_path = hashToPath(current_wt_hash);
    char buffer1[500];
    memset(buffer1, 0, 500);
    sprintf(buffer1,"%s.t", current_wt_path);
    WorkTree* wt1 = ftwt(buffer1);
    free(current_wt_path);
    freeCommit(current_commit);

    char* remote_wt_path = hashToPath(remote_wt_hash);
    char buffer2[500];
    memset(buffer2, 0, 500);
    sprintf(buffer2,"%s.t", remote_wt_path);
    WorkTree* wt2 = ftwt(buffer2);
    free(remote_wt_path);
    freeCommit(remote_commit);


    /*Creation du WorkTree de fusion si il n'y a pas de conflits*/
    List* conflicts = initList();

    WorkTree* wt = mergeWorkTrees(wt1, wt2, &conflicts);
    freeWorkTree(wt1);
    freeWorkTree(wt2);

    if((*conflicts) == NULL){
        /*Enregistrement instantane du WorkTree de la fusion*/
        char* wt_hash = blobWorkTree(wt);

        /*Creer le Commit associe a ce nouveau WorkTree*/
        Commit* c = createCommit(wt_hash);
        free(wt_hash);

        /*On initialise les champs du Commit*/
        commitSet(c,"message", message);
        commitSet(c,"predecessor", current_hash);
        commitSet(c, "merged_predecessor", remote_hash);
        free(current_hash);
        free(remote_hash);

        /*Enregistrement instantane du nouveau Commit*/
        char* commit_hash = blobCommit(c);
        freeCommit(c);

        /*Mise a jour des references*/
        createUpdateRef(current_branch, commit_hash);
        createUpdateRef("HEAD", commit_hash);
        free(commit_hash);

        /*Supprimer la reference de la branche passee en parametre*/
        deleteRef(remote_branch);

        /*Restorer le projet correspondant au WorkTree de fusion*/
        restoreWorkTree(wt, ".");

        printf("La fusion des branches %s et %s vers %s s'est bien passee !\n", current_branch, remote_branch, current_branch);
        
        free(current_branch);
        freeWorkTree(wt);
        freeList(conflicts);

        return NULL;
    }
    else{
        /*On ne peut pas faire la fusion*/
        freeWorkTree(wt);
        free(current_branch);
        free(current_hash);
        free(remote_hash);
        return conflicts;
    }
}

void createDeletionCommit(char* branch, List* conflicts, char* message){
    if (branchExists(branch) != 1) {
        fprintf(stderr, "La branche %s n'existe pas !\n", branch);
        exit(1);
    }

    char* current_branch = getCurrentBranch();

    /*Changement de branche*/
    myGitCheckoutBranch(branch);

    /*Recupérer le fichier contenant la référence(le hash) du commit*/
	char* commit_hash = getRef(branch);

	/*Recuperation du chemin vers le Commit et restoration*/
    char* path_commit_hash = hashToPathCommit(commit_hash);
	Commit* c = ftc(path_commit_hash);
    free(commit_hash);
    free(path_commit_hash);

    /*Recuperer le WorkTree associe*/
    char* wt_hash = commitGet(c,"tree");
    char* path_hash = hashToPath(wt_hash);
    char wt_path[500];
    memset(wt_path, 0, 500);
    sprintf(wt_path, "%s.t", path_hash);
    WorkTree* wt = ftwt(wt_path);
    free(path_hash);

    /*On vide la zone de préparation*/
    char cmd[256];
    sprintf(cmd,"cat /dev/null > .add");
    system(cmd);

    /*Ajouter les fichiers/repertoires du WorkTree qui ne font pas partie de la liste des conflits*/
    for(int i=0; i < wt -> n; i++){
        if(searchList(conflicts, wt->tab[i].name) == NULL){
            myGitAdd(wt->tab[i].name);
        }
    }

    freeWorkTree(wt);
    freeCommit(c);

    /*Enregistrement du Commit de suppression*/
    myGitCommit(branch, message);

    /*Revenir sur la branche de depart*/
    myGitCheckoutBranch(current_branch);
    free(current_branch);
}