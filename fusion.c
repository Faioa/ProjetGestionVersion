#include "fusion.h"

WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts){

    WorkTree*wt=initWorkTree();
    int test;//testeur de fichiers conflictuels
    for(int i=0;i<wt1->n;i++){
        test=0;
        WorkFile*wf1=&(wt1->tab[i]);
        for(int j=0;j<wt2->n;j++){
            WorkFile*wf2=&(wt2->tab[j]);
            //Si les 2 fichiers ont le même nom mais des hash différents
            if(strcmp(wf1->name,wf2->name)==0 && strcmp(wf1->hash,wf2->hash)!=0){
                insertFirst(*conflicts,buildCell(wf1->name));
                insertFirst(*conflicts,buildCell(wf2->name));
                test=1;
            }
        }
        if(test == 0){//le fichier/rep n'est en conflit avec aucun autre fichier/rep
            appendWorkTree(wt,wf1->name,wf1->hash,wf1->mode);
        }
    }

    return wt;

}
List* merge(char* remote_branch,char* message){
    char*current_branch=getCurrentBranch()
    WorkTree*wt1=ftwt(current_branch);
    WorkTree*wt2=ftwt(remote_branch);
    List*conflicts=initList();

    WorkTree*wt=mergeWorkTrees(wt1,wt2,&conflicts);

    





    return NULL;
}