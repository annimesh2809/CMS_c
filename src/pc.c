#include "def_struct.h"
#include <stdio.h>

struct Conference* getConf(int);
int getchoice(char* ,int);

void welcome_pc(){
	printf("Welcome to Program Comiittee Portal\n");
}

int isPCmember(int UID,int ConfID){
	struct Conference *conf = getConf(ConfID);
	for(int i=0;i<conf->nopc;i++)
	{
		if(conf->PCID[i] == UID)
			return 1;
	}
	return 0;
}

void pc_conf_portal(int UID,int ConfID){

}

void pc_portal(int UID){
	welcome_pc();
	printf("Enter the Conference ID: \n");
	int ConfID = getchoice("",NET_CONF);
	if(isPCmember(UID,ConfID)){
		pc_conf_portal(UID,ConfID);
	}
	else{
		printf("You are not a program comittee member of this conference\nContact the organising committee member for details.");
		pc_portal(UID);
	}
}