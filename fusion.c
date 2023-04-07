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
    char*current_branch=getCurrentBranch();
    WorkTree*wt1=ftwt(current_branch);
    WorkTree*wt2=ftwt(remote_branch);
    List*conflicts=initList();

    WorkTree*wt=mergeWorkTrees(wt1,wt2,&conflicts);
    if((*conflicts) == NULL){
        //creer le commit associé à ce nouveau worktree
        char *wt_hash=saveWorkTree(wt,".");
        Commit * c=createCommit(wt_hash);
        char*commit_pred_hash=getRef(remote_branch);
	    //Commit * c_pred=ftc(hashToPathCommit(commit_pred_hash));
        commitSet(c,"message",message);
        commitSet(c,"predecessor",commit_pred_hash);
        //enregistrement du nouveau commit
        char * commit_hash=blobCommit(c);
        //ajout du commit à la branche courante
        //mise à jour référence branche courante et HEAD
        createUpdateRef(current_branch,commit_hash);
        createUpdateRef("HEAD",commit_hash);
        //supprimer la référence de la branche passée en paramètre
        deleteRef(remote_branch);
        //restorer le projet correspondant au worktree de fusion
        restoreWorkTree(wt,".");

    }
    else{
        //On ne peut pas faire le merge
        //On supprime les worktrees
        freeWorkTree(wt1);
        freeWorkTree(wt2);
        freeWorkTree(wt);
        return conflicts;
    }

    return NULL;

}

void createDeletionCommit(char* branch, List* conflicts, char* message){
    char * current_branch=getCurrentBranch();
    myGitCheckoutBranch(branch);
    //recupérer le fichier contenant la référence(le hash) du commit
	char*commit_hash=getRef(branch);
	//conversion en commit
	Commit * c=ftc(hashToPathCommit(commit_hash));
    //recupérer le worktree associé
    WorkTree*wt=ftwt(hashToPath(commitGet(c,"tree")));
    //vider la zone de préparation
    char cmd[256];
    sprintf(cmd,"cat /dev/null > .add");
    system(cmd);
    //ajouter les fichiers/repertoires du worktree qui ne font pas partie de la liste des conflits
    for(int i=0;i<wt->n;i++){
        if(searchList(conflicts,wt->tab[i].name) == NULL){
            myGitAdd(wt->tab[i].name);
        } 
    }

    myGitCommit(branch,"suppresion");

    //revenir sur la branche de départ
    myGitCheckoutBranch(current_branch);



}





