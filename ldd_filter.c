#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	char *cc;
	char *ccc;
	char c[1024];
	c[0]=0;
	fgets(c ,1021,stdin);
	c[1022]=0;
	cc=strstr(c,"/");
	if(cc!=NULL){
		ccc=strstr(cc," ");
		if(ccc!=NULL){
			ccc[0]=0;
			printf("%s\n",cc);
		}else{
			printf("\n");
		}
	}else{	
		printf("\n");
	}
	return 0;
}
