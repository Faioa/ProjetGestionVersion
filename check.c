#include "check.h"

void restoreCommit(char *hash_commit){

    if (hash_commit == NULL || strlen(hash_commit) == 0) {
        return;
    }

    char buffer[1000];
    char*path=hashToPathCommit(hash_commit);
    Commit*c=ftc(path);
    char* tab=commitGet(c,"tree");
    char* path_commit = hashToPath(tab);
    sprintf(buffer,"%s.t", path_commit);
	WorkTree*wt=ftwt(buffer);
    restoreWorkTree(wt,".");
    free(path);
    freeWorkTree(wt);
    freeCommit(c);
    free(path_commit);
}

void myGitCheckoutBranch(char * branch){
    FILE*f=fopen(".current_branch","w");
    fprintf(f,"%s",branch);
    fclose(f);

    char* ref = getRef(branch);
    createUpdateRef("HEAD",ref);
    restoreCommit(ref);

    printf("Le changement de branche vers %s et la restoration de son dernier commit s'est effectue avec succes !\n", branch);

    free(ref);
}

List* filterList(List * L, char * pattern){
    List* result = initList();
    Cell* tmp = *L;

    while(tmp){
        if(strncmp(tmp->data,pattern,strlen(pattern)) == 0){
            insertFirst(result,buildCell(tmp->data));
        }
        tmp=tmp->next;
    }

    return result;
}

void myGitCheckoutCommit(char * pattern){
    List* l_commit = getAllCommits();
    List* l_pattern = filterList(l_commit, pattern);
    Cell* c_pattern = *l_pattern;
    if(c_pattern == NULL){
        fprintf(stderr,"Aucun hash de commit ne correspond a ce pattern !\n");
    }else{
        if(c_pattern->next == NULL){
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
            printf("Veuillez precicez votre requete !\n");
            afficheList(l_pattern);
        }
    }
    freeList(l_commit);
    freeList(l_pattern);
}