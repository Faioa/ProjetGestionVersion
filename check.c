#include "check.h"

/*Restaure le commit correspondant au hash donne en parametre*/
void restoreCommit(char *hash_commit){

    if (hash_commit == NULL || strlen(hash_commit) == 0) {
        return;
    }

    /*Recuperation du chemin*/
    char buffer[1000];
    char*path=hashToPathCommit(hash_commit);

    /*Conversion en commit*/
    Commit*c=ftc(path);

    /*Recuperation du WorkTree associe*/
    char* tab=commitGet(c,"tree");
    char* path_commit = hashToPath(tab);
    sprintf(buffer,"%s.t", path_commit);
	WorkTree*wt=ftwt(buffer);

    /*Restauration des fichiers*/
    restoreWorkTree(wt,".");

    /*Liberation des ressources*/
    free(path);
    freeWorkTree(wt);
    freeCommit(c);
    free(path_commit);
}

/*Effectue le changement de branche et restaure le dernier Commit de la branche*/
void myGitCheckoutBranch(char * branch){
    /*Change la branche pointee par .current_branch*/
    FILE*f=fopen(".current_branch","w");
    fprintf(f,"%s",branch);
    fclose(f);

    /*Restaure le dernier Commit de la branche*/
    char* ref = getRef(branch);
    createUpdateRef("HEAD",ref);
    restoreCommit(ref);

    printf("Le changement de branche vers %s et la restauration de son dernier commit s'est effectue avec succes !\n", branch);

    free(ref);
}

/*Renvoie la liste des elements de la liste passee en parametre qui commencent par pattern*/
List* filterList(List * L, char * pattern){
    List* result = initList();
    Cell* tmp = *L;

    /*Parcours de la liste et comparaison avec pattern*/
    while(tmp){
        if(strncmp(tmp->data,pattern,strlen(pattern)) == 0){
            insertFirst(result,buildCell(tmp->data));
        }
        tmp=tmp->next;
    }

    return result;
}

/*Restaure le commit qui match avec le pattern donne en parametre*/
void myGitCheckoutCommit(char * pattern){
    /*Recuperation de tous les commits de toutes les branches*/
    List* l_commit = getAllCommits();

    /*Recuperation de la liste filtree*/
    List* l_pattern = filterList(l_commit, pattern);

    /*Parcours de la liste filtree*/
    Cell* c_pattern = *l_pattern;
    if(c_pattern == NULL){
        /*Aucun commit ne match*/
        fprintf(stderr,"Aucun hash de commit ne correspond a ce pattern !\n");
    }else{
        /*Il y a au moins un commit*/
        if(c_pattern->next == NULL){
            /*Il n'y a qu'un commit, c'est le bon*/
            createUpdateRef("HEAD", c_pattern->data);
            restoreCommit(c_pattern->data);
            printf("Le changement vers le commit %s s'est effectue avec succes !\n", c_pattern->data);

            /*On met a jour current_branch pour match avec la reference de HEAD*/
            List* refs = listdir(".refs");
            for (Cell* cell = *refs; cell != NULL; cell = cell -> next) {
                if (strcmp(cell->data, "HEAD") == 0)
                    continue;

                /*Quand on trouve la bonne branche on l'actualiser*/
                char* ref = getRef(cell -> data);
                if (strcmp(ref, c_pattern -> data) == 0) {
                    createUpdateRef(cell->data, ref);
                }

                if (ref != NULL)
                    free(ref);
            }

            freeList(refs);
        } else {
            /*Il y a plusieurs commit qui matchent*/
            printf("Veuillez precicez votre requete !\n");
            afficheList(l_pattern);
        }
    }
    freeList(l_commit);
    freeList(l_pattern);
}