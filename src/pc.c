#include "def_struct.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Conference* getConf(int);
int getchoice(char* ,int);
void displayPaperDetails(struct Paper);
void updateConfF(struct Conference);
void pc_conf_portal(int);
void displayPaper(char *);

void welcome_pc(){
	printf("Welcome to Program Committee Portal\n");
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

void ScreenPaper(int ConfID,int i){
	struct Conference *Conf = getConf(ConfID);
	displayPaperDetails(Conf->papers[i]);
	int c = getchoice("1) Open Document\n2) Accept / Decline\n3) Add Reviewers\n4) Back",4);
	switch(c){
		case 1:{
			displayPaper(Conf->papers[i].src);
			break;
		}
		case 2:{
			char g;
			printf("Accept? (Y/N): ");
			scanf("%c ",&g);
			if(g == 'Y'|| g == 'y')
				Conf->papers[i].isallowed = 1;
			else Conf->papers[i].isallowed = 0;
			break;
		}
		case 3:{
			printf("Enter reviewers email: \n");
			scanf("%s",Conf->papers[i].AReviewer[Conf->papers[i].noar].email);
			printf("Enter reviewers password: \n");
			scanf("%s",Conf->papers[i].AReviewer[Conf->papers[i].noar].pass);
			Conf->papers[i].noar++;	
			break;
		}
		case 4:{
			pc_conf_portal(ConfID);
			break;
		}
	}
	updateConfF(*Conf);
	free(Conf);
	ScreenPaper(ConfID,i);
}

void RReviewsCompleted(int ConfID){
	struct Conference *Conf = getConf(ConfID);
	int flag = 1;
	if(Conf->nor>0)
	{
		for(int i=0;i<Conf->nop;i++)
		{
			if(Conf->papers[i].isallowed)
			{
				for(int j=0;j<Conf->papers[i].nor;j++)
				{
					if(Conf->papers[i].reviews[j].isaccepted == -1)
						flag = 0;
				}
			}
			else if(Conf->papers[i].isallowed == -1)
				flag = 0;
		}
	}
	Conf->ReviewsCompleted = flag;
	updateConfF(*Conf);
	free(Conf);
}

void pc_conf_portal(int ConfID){
	struct Conference *Conf = getConf(ConfID);
	if(Conf->ReviewsCompleted&&Conf->nop){
		for(int i=0;i<Conf->nop;i++){
			if(Conf->papers[i].isallowed)
			{
				displayPaperDetails(Conf->papers[i]);
				printf("Publish status of paper: (Y/N)\n");
				char ch;
				scanf("%c ",&ch);
				if(ch)
					Conf->papers[i].isaccepted = 1;
				else Conf->papers[i].isaccepted = 0;
			}
		}
		updateConfF(*Conf);
		printf("Successfully screened and rated everyone... Exiting... \n");
		exit(0);
	}
	else{
		printf("Screening papers and select Reviewers: \n");
		if(Conf->nop){
			for(int i=0;i<Conf->nop;i++)
				displayPaperDetails(Conf->papers[i]);
			int ch = getchoice("Paper ID: ",Conf->nop);
			int i;
			for(i=0;i<Conf->nop;i++)
				if(Conf->papers[i].PID == ch)
					break;
			ScreenPaper(ConfID,i);
			RReviewsCompleted(ConfID);
		}
		else{
			printf("No tasks available at the moment\n");
			exit(0);
		}
	}
	free(Conf);
	pc_conf_portal(ConfID);
}

void pc_portal(int UID){
	welcome_pc();
	printf("Enter the Conference ID: \n");
	int ConfID = getchoice("",NET_CONF);
	if(isPCmember(UID,ConfID)){
		pc_conf_portal(ConfID);
	}
	else{
		printf("You are not a program comittee member of this conference\nContact the organising committee member for details.");
		pc_portal(UID);
	}
}