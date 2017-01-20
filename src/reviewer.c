#include "def_struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void displayPaperDetails(struct Paper);

void welcome_reviewer(){
	printf("Welcome to Reviewer Portal\n");
}

void reviewer_login_portal(int UID,int ConfID){
	int logout = 0;
	do{
		int ch = getchoice("1) Papers you have been assigned\n2) Review accepted papers\n3) Logout",3);
		switch(ch){
			case 1:{
				printf("You have been assigned reviewer for the following papers: \n");
				struct Conference *Conf = getConf(ConfID);
				struct UserF *u = getUserByID(UID);
				int isPaper[MAX_PAPERS];
				int isReviewer[MAX_PAPERS];
				for(int i=0;i<MAX_PAPERS;i++)
					isReviewer[i] = -1;
				for(int i=0;i<Conf->nop;i++){
					for(int j=0;j<Conf->papers[i].noar;j++)
					{
						if(!strcmp(Conf->papers[i].AReviewer[j].email,u->email))
						{
							displayPaperDetails(Conf->papers[i]);
							isReviewer[Conf->papers[i].PID] = j;
							break;
						}
					}
					isPaper[Conf->papers[i].PID] = i;
				}
				printf("Enter Paper ID to approve to review or decline: \n");
				int PID;
				scanf("%d",&PID);
				if(isReviewer[PID]!=-1)
				{
					char ch = 'N';
					printf("Approve to review? (Y/N)\n");
					scanf("%c ",&ch);
					if(ch=='Y'||ch=='y')
					{
						Conf->papers[isPaper[PID]].reviews[Conf->papers[isPaper[PID]].nor ++].RID = UID;
						Conf->papers[isPaper[PID]].reviews[Conf->papers[isPaper[PID]].nor -1].rating = 0;
						strcpy(Conf->papers[isPaper[PID]].reviews[Conf->papers[isPaper[PID]].nor -1].details,"Not yet rated");
						Conf->papers[isPaper[PID]].reviews[Conf->papers[isPaper[PID]].nor -1].isaccepted = -1;
						int ispresent = 0;
						for(int i=0;i<Conf->nor;i++)
						{
							if(Conf->RID[i] == UID)
							{
								ispresent = 1;
							}
						}
						if(!ispresent){
							Conf->RID[Conf->nor++] = UID;
						}
					}
					Conf->papers[PID].AReviewer[isReviewer[PID]] = Conf->papers[PID].AReviewer[--Conf->papers[PID].noar];
				}
				else{
					printf("You ae not assigned a reviewer to this paper\n"); 
					reviewer_login_portal(UID,ConfID);
				}
				updateConfF(*Conf);
				free(u);
				free(Conf);
				break;
			}
			case 2:{
				struct Conference *Conf = getConf(ConfID);
				printf("The following papers have been accepted by you for review:\n");
				int isReviewer[MAX_PAPERS];
				for(int i=0;i<MAX_PAPERS;i++)
					isReviewer[i] = -1;
				for(int i=0;i<Conf->nop;i++)
				{
					for(int j=0;j<Conf->papers[i].nor;j++)
					{
						if(Conf->papers[i].reviews[j].RID == UID)
						{
							displayPaperDetails(Conf->papers[i]);
							isReviewer[Conf->papers[i].PID] = j;
							break;
						}
					}
				}
				printf("Enter paper Id to review:\n");
				int PID;
				scanf("%d",&PID);
				if(isReviewer[PID]!=-1)
				{
					int back = 0;
					do{
						displayPaperDetails(Conf->papers[PID]);
						int g = getchoice("1) Rating\n2) Details\n3) Accept/Decline\n4) Save and Go Back",4);
						switch(g){
							case 1:{
								Conf->papers[PID].reviews[isReviewer[PID]].rating = getchoice("Enter rating",10);
								break;
							}
							case 2:{
								printf("Enter review details:\n");
								scanf("%s",Conf->papers[PID].reviews[isReviewer[PID]].details);
								break;
							}
							case 3:{
								char c;
								printf("Accept this paper? (Y/N)\n");
								scanf("%c ",&c);
								if(c=='Y'||c=='y')
									Conf->papers[PID].reviews[isReviewer[PID]].isaccepted = 1;
								else Conf->papers[PID].reviews[isReviewer[PID]].isaccepted = 0;
								break;
							}
							case 4:{
								back = 1;
								break;
							}
						}
					}while(!back);
				}
				else{
					printf("You are not assigned a reviewer to this paper\n"); 
					reviewer_portal(UID);
				}
				updateConfF(*Conf);
				free(Conf);
				break;
			}
			case 3:{
				printf("Successfully logged out\n");
				logout = 1;
				CLEAR;
				break;
			}
		}
	}while(!logout);
}

void reviewer_portal(int UID){
	welcome_reviewer();
	int ConfID = getchoice("Enter Conference ID: ",NET_CONF);
	reviewer_login_portal(UID,ConfID);
}