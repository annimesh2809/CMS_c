#include <stdio.h>
#include "def_struct.h"

void main_login_portal();
int getchoice(char* ,int);
void FileError(const char *);

void welcome_oc(){
	printf("*** Welcome to the Organising Committee Portal ***\n");
}

void writeConfF(struct Conference conf)
{
	FILE *cf = fopen(CONF_FILE,"a");
	if(fp){
		fwrite(&conf,sizeof(conf),cf);
		fclose(cf);
	}
	else FileError(CONF_FILE);
}

struct Conference getConf(int ConfID){
	
}

void createConference(){
	struct Conference conf;
	printf("Enter conference title: \n");
	scanf("%s",conf.title);
	printf("Enter conference date (DD/MM/YYYY): \n");
	scanf("%s",conf.date);
	printf("Enter venue: \n");
	scanf("%s",conf.venue);
	printf("Enter topics covered: \n");
	scanf("%s",conf.tc);
	printf("Enter deadlines (optional): \n");
	scanf("%s",conf.dl);
	conf.CONFID = ++NET_CONF;
	writeConfF(conf);
	editConference(conf.CONFID);
}

void editConference(int confID){
	struct Conference conf = getConf(confID);
	int ch = getchoice("1) Change title\n2) Change Date\n3) Change venue\n4) Change topics covered\n5) Change deadlines\n6)
					Add/Update organising members\n7) Add/Update program committee members\n8) Back to main menu",8);
	switch(ch){
		case 1:{
			printf("Enter new title\n");
			break;
		}
		case 2:{
			break;
		}
		case 3:{
			break;
		}
		case 4:{
			break;
		}
		case 5:{
			break;
		}
		case 6:{
			break;
		}
		case 7:{
			break;
		}
		case 8:{
			oc_portal();
			break;
		}
	}
}

void oc_portal(){
	welcome_oc();
	int ch = getchoice("1) Create entry for a new conference\n2) Edit an existing conference\n3)Logout\n",3);
	switch(ch){
		case 1:{
			createConference();
			break;
		}
		case 2:{
			printf("Enter the Conference ID you want to edit: \n");
			int confID = getchoice("",NET_CONF);
			editConference(confID);
			break;
		}
		case 3:{
			printf("Successfully logged out\n\n");
			main_login_portal();
			break;
		}
	}
}