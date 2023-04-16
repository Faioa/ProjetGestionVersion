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
    free(ref);
}

List* filterList(List * L, char * pattern){
    List*result=initList();
    Cell*tmp=*L;
    while(tmp){
        if(strncmp(tmp->data,pattern,strlen(pattern))){
            insertFirst(result,buildCell(tmp->data));
        }
        tmp=tmp->next;
    }
    return result;
}

void myGitCheckoutCommit(char * pattern){
    List*l_commit=getAllCommits();
    List*l_pattern=filterList(l_commit,pattern);
    Cell*c_pattern=*l_pattern;
    if(c_pattern == NULL){
        fprintf(stderr,"Aucune branche ne correspond a ce pattern !\n");
    }else{
        if(c_pattern->next == NULL){
            myGitCheckoutBranch(c_pattern->data);
        }else{
            printf("Veuillez precicez votre requete !\n");
            afficheList(l_pattern);
        }
    }

}