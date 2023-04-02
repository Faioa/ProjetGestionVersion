#include "check.h"

void restoreCommit(char *hash_commit){
    char buffer[1000];
    char*path=hashToPathCommit(hash_commit);
    Commit*c=ftc(path);
    char tab[256]=commitGet(c,"tree");
    sprintf(buff,"%s.t",hashToPath(tab));
	WorkTree*wt=ftwt(buffer);
    restoreWorkTree(wt,".");
    free(path);
    freeWorkTree(wt);
    freeCommit(c);
}

void myGitCheckoutBranch(char * branch){
    FILE*f=fopen(".current_branch","w");
    fprintf(f,"%s",branch);
    fclose(f);
    char*ref=getRef(branch);
    createUpdateRef("HEAD",ref);
    restoreCommit(ref);

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
        fprintf(stderr,"aucune branche ne correspond à ce pattern\n");
    }else{
        if(c_pattern->next == NULL){
            myGitCheckoutBranch(c_pattern->data);
        }else{
            fprintf(stdout,"Veuillez précicez votre requete\n");
            afficheList(l_pattern);
        }
    }

}