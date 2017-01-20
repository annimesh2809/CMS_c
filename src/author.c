#include "def_struct.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Conference* getConf(int);
int getchoice(char* ,int);
void displayConf(struct Conference *);
struct UserF *getUserByID(int);
void updateConfF(struct Conference);
void main_login_portal();

void welcome_author(){
	CLEAR;
	printf("Welcome to author portal\n");
}

int isAuthor(int UID,int ConfID){
	struct Conference *conf = getConf(ConfID);
	for(int i=0;i<conf->noa;i++)
	{
		if(conf->AID[i] == UID)
			return 1;
	}
	return 0;
}

void displayReview(struct ReviewInfo *review){
	struct UserF *p = getUserByID(review->RID);
	printf("Reviewer: %s\n",p->name);
	free(p);
	if(review->isaccepted>=0){
		printf("Rating: (%d) / 10\n",review->rating);
		printf("Details: %s\n", review->details);
		printf("Response: %s\n", (review->isaccepted)?"Accepted":"Declined");
	}
	else{
		printf("Not yet rated\n");
	}
}

void displayPaper(char *src){
	char CMD[PATH_LENGTH*2];
	strcpy(CMD,"f='");
	strcat(CMD,src);
	strcat(CMD,"'; if [ ! -f $f ]; then echo 'File does not exist'; else cat $f; fi;");
	printf("Book at loaction: %s\n",src);
	printf("*************************************\n");
	system(CMD);
	printf("*************************************\n");
}

void displayPaperDetails(struct Paper p)
{
	printf("Paper ID: %d\n",p.PID);
	struct UserF *u = getUserByID(p.AID);
	printf("Author: %s\n",u->name);
	free(u);
	if(p.isallowed == 1){
		printf("Accpeted for publication: %s\n",(p.isaccepted >=0) ? (p.isaccepted?"True":"False") : "Waiting");
		printf("Assigned Reviewers: \n");
		for(int i=0;i<p.noar;i++)
			printf("%d) %s\n",i+1,p.AReviewer[i].email);
		printf("Reviews: \n");
		for(int i=0;i<p.nor;i++)
		{
			displayReview(&p.reviews[i]);
		}
	}
	else if(p.isallowed == -1){
		printf("Waiting for screening process\n");
	}
	else printf("Declined by program committee\n");
	printf("Location: %s\n",p.src);
}

struct Paper* getPapersByAuthor(int UID,int ConfID,int* len){
	struct Paper *p = (struct Paper *)malloc(MAX_PAPERS*sizeof(struct Paper));
	*len = 0;
	struct Conference *Conf = getConf(ConfID);
	for(int i=0;i<Conf->nop;i++)
	{
		if(Conf->papers[i].AID == UID)
		{
			p[(*len)++] = Conf->papers[i];
		}
	}
	free(Conf);
	return p;
}

struct Paper* getPaperByID(int PID,int ConfID){
	struct Conference *Conf = getConf(ConfID);
	struct Paper *p = (struct Paper *)malloc(sizeof(struct Paper));
	for(int i=0;i<Conf->nop;i++)
	{
		if(Conf->papers[i].PID == PID)
		{
			*p = (Conf->papers[i]);
			free(Conf);
			return p;
		}
	}
	free(Conf);
	return NULL;
}

int validAuthorPaper(int ConfID,int UID,int PID){
	struct Conference *conf = getConf(ConfID);
	for(int i=0;i<conf->nop;i++)
	{
		if(conf->papers[i].PID == PID)
			if(conf->papers[i].AID == UID)
			{
				free(conf);
				return 1;
			}
	}
	free(conf);
	return 0;
}

void editPaper(int PID,int ConfID)
{
	struct Paper *p = getPaperByID(PID,ConfID);
	while(1){
		displayPaperDetails(*p);
		int ch = getchoice("You can Add/Update the following:\n1) Title\n2) Abstract\n3) Affiliation\n4) Location of file\n5) Save and Exit\n",5);
		switch(ch){
			case 1:{
				printf("Enter title: \n");
				scanf("%s",p->title);
				break;
			}
			case 2:{
				printf("Enter abstract: \n");
				scanf("%s",p->abstract);
				break;
			}
			case 3:{
				printf("Enter affiliation: \n");
				scanf("%s",p->affiliation);
				break;
			}
			case 4:{
				printf("Enter location to file\n");
				scanf("%s",p->src);
				break;
			}
			case 5:{
				struct Conference *Conf = getConf(ConfID);
				int x;
				for(int i=0;i<Conf->nop;i++)
					if(Conf->papers[i].PID == PID){
						Conf->papers[i] = *p;
						printf("conf: %s, paper: %s\n",Conf->papers[PID].src,(*p).src);
						x= i;
						break;
					}
				printf("just before: %s\n", Conf->papers[x].src);
				updateConfF(*Conf);
				free(p);
				return;
			}
		}
	}
}

void removePaper(int ConfID,int PID){
	struct Conference *Conf = getConf(ConfID);
	for(int i=0;i<Conf->nop;i++){
		if(Conf->papers[i].PID == PID)
		{
			Conf->papers[i] = Conf->papers[--Conf->nop];
			break;
		}
	}
	updateConfF(*Conf);
	free(Conf);
}

void author_portal(int UID){
	welcome_author();
	printf("Enter the Conference ID: \n");
	int ConfID = getchoice("",NET_CONF);
	struct Conference *Conf = getConf(ConfID);
	displayConf(Conf);
	int len;
	struct Paper *_p = getPapersByAuthor(UID,ConfID,&len);
	if(len){
		printf("Your submissions:\n");
		for(int i=0;i<len;i++)
		{
			displayPaperDetails(_p[i]);
		}
	}
	else printf("You have not submitted any papers yet\n");
	free(_p);
	int ch = getchoice("1) Add a paper\n2) Edit a submitted paper\n3) Remove a paper\n4) Logout",4);
	switch(ch){
		case 1:{
			struct Paper p;
			p.PID = ++ Conf->nop;
			p.AID = UID;
			p.nor = p.noar = 0;
			int ispresent = 0;
			for(int i=0;i<Conf->noa;i++)
				if(Conf->AID[i] == UID)
					ispresent = 1;
			if(!ispresent)
				Conf->AID[Conf->noa ++] = UID;
			printf("UID: %d\n", UID);
			strcpy(p.src,"None");
			strcpy(p.title,"None");
			strcpy(p.abstract,"None");
			strcpy(p.affiliation,"None");
			p.isallowed = -1;
			p.isaccepted = -1;
			Conf->papers[Conf->nop -1] = p;
			updateConfF(*Conf);
			editPaper(p.PID,Conf->CONFID);
			break;
		}
		case 2:{
			int PID = getchoice("Paper ID to edit: ",Conf->nop);
			if(validAuthorPaper(ConfID,UID,PID))
				editPaper(PID,ConfID);
			else printf("You are not the author of this paper\n");
			break;
		}
		case 3:{
			int PID = getchoice("Paper ID to remove: ",Conf->nop);
			if(validAuthorPaper(ConfID,UID,PID))
				removePaper(ConfID,PID);
			else printf("You are not the author of this paper\n");
			break;
		}
		case 4:{
			printf("Successfully logged out!\n");
			CLEAR;
			main_login_portal();
			break;
		}
	}
	free(Conf);
	author_portal(UID);
}