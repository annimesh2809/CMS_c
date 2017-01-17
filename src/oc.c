#include <stdio.h>
#include <stdlib.h>
#include "def_struct.h"
#include <string.h>

void main_login_portal();
int getchoice(char* ,int);
void FileError(const char *);
void oc_portal(int);
int UserFfind(char *);

void welcome_oc(){
	printf("*** Welcome to the Organising Committee Portal ***\n");
}

void updateConfF(struct Conference conf)
{
	FILE *cf = fopen(CONF_FILE,"rb+");
	struct Conference temp;
	if(cf){
		while(fread(&temp,sizeof(struct Conference),1,cf)){
			if(conf.CONFID == temp.CONFID){
				fseek(cf,-sizeof(struct Conference),SEEK_CUR);
				fwrite(&conf,sizeof(struct Conference),1,cf);
				fclose(cf);
				return;
			}
		}
		fseek(cf,0,SEEK_END);
		fwrite(&conf,sizeof(struct Conference),1,cf);
		fclose(cf);
	}
	else FileError(CONF_FILE);
}

struct Conference* getConf(int ConfID){
	FILE *cf = fopen(CONF_FILE,"rb");
	if(cf){
		struct Conference *conf = (struct Conference *)malloc(sizeof(struct Conference));
		while(fread(conf,sizeof(struct Conference),1,cf)){
			if(conf->CONFID == ConfID){
				fclose(cf);
				return conf;
			}
		}
		fclose(cf);
		return NULL;
	}
	else FileError(CONF_FILE);
}


void displayConf(struct Conference* conf){
	printf("************************** CONFERENCE DETAILS ***\n");
	printf("Conference ID: %d\n",conf->CONFID);
	printf("Conference Title: %s\n",conf->title);
	printf("Date: %s\tVenue: %s\n",conf->date,conf->venue);
	printf("Topics covered: %s\n", conf->tc);
	printf("Deadlines: %s\n", conf->dl);
	printf("*** Members Info ***\n");
	printf("Organising Committee members (%d): \n",conf->nooc);
	for(int i=0;i<conf->nooc;i++){
		struct UserF *u = getUserByID(conf->OCID[i]);
		printf("%d %d %s\n", i+1,u->UID,u->name);
		free(u);
	}
	printf("Program Committee members (%d): \n",conf->nopc);
	for(int i=0;i<conf->nopc;i++){
		struct UserF *u = getUserByID(conf->PCID[i]);
		printf("%d %d %s\n", i+1,u->UID,u->name);
		free(u);
	}
	printf("Assigned Reviewers (%d): \n",conf->nor);
	for(int i=0;i<conf->nor;i++){
		struct UserF *u = getUserByID(conf->RID[i]);
		printf("%d %d %s\n", i+1,u->UID,u->name);
		free(u);
	}
	printf("Authors (%d): \n",conf->noa);
	for(int i=0;i<conf->noa;i++){
		struct UserF *u = getUserByID(conf->PCID[i]);
		printf("%d %d %s\n", i+1,u->UID,u->name);
		free(u);
	}
}

void editConference(int UID,int confID){
	struct Conference *conf = getConf(confID);
	while(1){
		system("clear");
		displayConf(conf);
		int ch = getchoice("You can Add/Update the following entries:\n1) Title\n2) Date\n3) Venue\n4) Topics covered\n5) Deadlines\n6) Organising members\n7) Program committee members\n8) Save and exit\n",8);
		switch(ch){
			case 1:{
				printf("Enter title: ");
				scanf("%s",conf->title);
				break;
			}
			case 2:{
				printf("Enter date: ");
				scanf("%s",conf->date);
				break;
			}
			case 3:{
				printf("Enter venue: ");
				scanf("%s",conf->venue);
				break;
			}
			case 4:{
				printf("Enter topics covered: \n");
				scanf("%s",conf->tc);
				break;
			}
			case 5:{
				printf("Enter Deadlines\n");
				scanf("%s",conf->dl);
				break;
			}
			case 6:{
				printf("Enter new organising member's email\n");
				char t[EMAIL_LENGTH];
				scanf("%s",t);
				int c = UserFfind(t);
				if(!c)
					printf("Email not registered\n");
				else{
					struct UserF *u=getUserByID(c);
					if(u->type != OC)
						printf("Member is not a part of Organising Committee\n");
					else{
						conf->OCID[conf->nooc++] = c;
						printf("Successfully added\n");
					}
					free(u);
				}
				break;
			}
			case 7:{
				printf("Enter new program committee member's email\n");
				char t[EMAIL_LENGTH];
				scanf("%s",t);
				int c = UserFfind(t);
				if(!c)
					printf("Email not registered\n");
				else{
					struct UserF *u=getUserByID(c);
					if(u->type != PC)
						printf("Member is not a part of Program Committee\n");
					else{
						conf->PCID[conf->nopc++] = c;
						printf("Successfully added\n");
					}
					free(u);
				}
				break;
			}
			case 8:{
				updateConfF(*conf);
				return;
			}
		}
	}

}


void createConference(int UID){
	struct Conference conf;
	conf.CONFID = ++NET_CONF;
	conf.nooc = conf.nopc = conf.nor = conf.noa = conf.nop = 0;
	strcpy(conf.title,"None");
	strcpy(conf.date,"None");
	strcpy(conf.venue,"None");
	strcpy(conf.tc,"None");
	strcpy(conf.dl,"None");
	conf.OCID[conf.nooc ++] = UID;
	updateConfF(conf);
	editConference(UID,conf.CONFID);
}

int isOCmember(int UID,int ConfID){
	struct Conference *conf = getConf(ConfID);
	for(int i=0;i<conf->nooc;i++)
	{
		if(conf->OCID[i] == UID)
			return 1;
	}
	return 0;
}

void oc_portal(int UID){
	welcome_oc();
	int ch = getchoice("1) Create entry for a new conference\n2) Edit an existing conference\n3) Logout\n",3);
	switch(ch){
		case 1:{
			createConference(UID);
			break;
		}
		case 2:{
			printf("Enter the Conference ID you want to edit: \n");
			int confID = getchoice("",NET_CONF);
			if(isOCmember(UID,confID))
				editConference(UID,confID);
			else{ 
				printf("You do not have priveledges to modify this conference\n");
			}
			break;
		}
		case 3:{
			printf("Successfully logged out\n\n");
			main_login_portal();
			break;
		}
	}
	oc_portal(UID);
}