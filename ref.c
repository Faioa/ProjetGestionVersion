#include "ref.h"
#include "file_utility.h"
void initRefs(){
	char cmd[256];
	sprintf(cmd,"mkdir -p .refs && touch .refs/{master,HEAD}");
	system(cmd);

}

void createUpdateRef(char* ref_name, char* hash){
	
	char cmd[256], *name = baseName(ref_name);
	sprintf(cmd,"echo %s > .refs/%s", hash, name);
	system(cmd);
	
	free(name);
}

void deleteRef(char* ref_name){
	char cmd[256], *name = baseName(ref_name);
	sprintf(cmd,"rm -f .refs/%s", name);
	system(cmd);
	
	free(name);
}
char* getRef(char* ref_name){
	if(file_exists(ref_name)==0)return NULL;

	char*buffer=malloc(sizeof(char)*256);

	char cmd[256];
	sprintf(cmd,"cat %s > %s",ref_name,buffer);
	system(cmd);

	return buffer;
}
