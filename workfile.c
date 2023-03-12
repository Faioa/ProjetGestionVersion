#include "workfile.h"

WorkFile * createWorkFile(char * name){
	WorkFile * w=malloc(sizeof(WorkFile));
	w->name=strdup(name);
	w->hash=NULL;
	w->mode=0;
}

char* wfts(WorkFile* wf){
	
	char * buffer=malloc(sizeof(char)*256);
	
	sprintf(buffer,"%s\t%s\t%d",wf->name,wf->hash,wf->mode);
	
	return buffer;

}

WorkFile* stwf(char* ch){

 char name[256];
 
 char hash[256];
 
 int mode;
 
 
 sscanf(ch,"%s\t%s\t%d",name,hash,&mode);
 
 WorkFile*w=createWorkFile(name);
 w->hash=strdup(hash);
 w->mode=mode;
 
 return w;
	


}
