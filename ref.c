#include "ref.h"

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
