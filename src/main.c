#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def_struct.h"

int NET_USERS = 0;
int NET_CONF = 0;

void main_login_portal();

void welcome(){
	printf("***** Welcome to Conference Management System *****\n");
}

void InputError(){
	printf("Invalid input! Retry!\n");
}

void FileError(const char *s){
	printf("File %s could not open\n",s);
}

int getchoice(char *s,const int MAX)
{
	printf("** Enter choice (1 - %d)\n", MAX);
	printf("%s\n",s);
	int ch;
	do{
		scanf("%d",&ch);
		if(ch>MAX)
			InputError();
	}while(ch>MAX);
}

struct UserF* getUserByID(int UID){
	FILE *uf = fopen(USER_FILE,"rb");
	if(uf){
		struct UserF *user = (struct UserF *)malloc(sizeof(struct UserF));
		while(fread(user,sizeof(struct UserF),1,uf)){
			if(user!=NULL){
				if(user->UID == UID){
					fclose(uf);
					return user;
				}
			}
		}
		fclose(uf);
		return NULL;
	}
	else{
		FileError(USER_FILE);
		exit(100);
	}
}

struct UserF* getUser(char *email,char *pass){
	FILE *uf = fopen(USER_FILE,"rb");
	if(uf){
		struct UserF *user = (struct UserF *)malloc(sizeof(struct UserF));
		while(fread(user,sizeof(struct UserF),1,uf)){
			if(user!=NULL){
				if(!strcmp(user->email,email)&&!strcmp(user->pass,pass)){
					fclose(uf);
					return user;
				}
			}
		}
		fclose(uf);
		return NULL;
	}
	else{
		FileError(USER_FILE);
		exit(100);
	}
}

void login_portal(){
	char email[EMAIL_LENGTH],pass[PASS_LENGTH];
	printf("Enter email: \n");
	scanf("%s",email);
	printf("Enter password: \n");
	scanf("%s",pass);
	struct UserF* user = getUser(email,pass);
	enum UType type = user==NULL?NONE:user->type;
	switch(type){
		case NONE:{
			printf("Invalid email / password! Retry!\n");
			login_portal();
			break;
		}
		case OC:{
			oc_portal(user->UID);
			break;
		}
		case PC:{
			pc_portal(user->UID);
			break;
		}
		case AUTHOR:{
			author_portal(user->UID);
			break;
		}
		case REVIEWER:{
			reviewer_portal(user->UID);
			break;
		}
	}
}

void writeUserF(struct UserF p){
	FILE *uf = fopen(USER_FILE,"ab");
	if(uf){
		fwrite(&p,sizeof(p),1,uf);
		fclose(uf);
	}
	else FileError(USER_FILE);
}

int UserFfind(char *email)
{
	FILE *uf = fopen(USER_FILE,"rb");
	if(uf){
		struct UserF user;
		while(fread(&user,sizeof(struct UserF),1,uf)){
			if(&user!=NULL){
				if(!strcmp(user.email,email)){
					fclose(uf);
					return user.UID;
				}
			}
		}
		fclose(uf);
		return 0;
	}
	else{
		FileError(USER_FILE);
		exit(100);
	}
}

void register_portal(){
	struct UserF p;
	printf("Enter email: ");
	scanf("%s",p.email);
	if(UserFfind(p.email))
	{
		printf("** Email already registered! Try Loging in!\n");
		main_login_portal();
	}
	printf("Enter name: ");
	scanf("%s",p.name);
	printf("Enter password: ");
	scanf("%s",p.pass);
	printf("Enter affiliation: ");
	scanf("%s",p.affl);
	printf("Enter expertise: ");
	scanf("%s",p.expertise);
	printf("Enter membership type: \n");
	p.UID = ++NET_USERS;
	p.type = (enum UType)getchoice("1) Organizing Committee\n2) Program Committee\n3) Reviewer\n4) Author",4);
	writeUserF(p);
	printf("** Successfully registered\n");
}

void main_login_portal(){
	int ch = getchoice("\t1) Login\n\t2) Register\n\t3) Exit",3);
	switch(ch){
		case 1:{
			login_portal();
			break;
		}
		case 2:{
			register_portal();
			break;
		}
		case 3:{
			exit(0);
		}
	}
	main_login_portal();
}

void load_data(){
	//for NETUSERS
	FILE *uf = fopen(USER_FILE,"rb");
	if(uf){
		struct UserF user;
		while(fread(&user,sizeof(struct UserF),1,uf)){
			if(&user!=NULL){
				NET_USERS++;
			}
		}
		printf("** Total registered users: %d\n", NET_USERS);
		fclose(uf);
	}
	else{
		printf("** No existing user data found!\n");
		uf = fopen(USER_FILE,"wb");
		fclose(uf);
		printf("** Created new user data file\n");
		NET_USERS = 0;
	}

	//for NETCONF
	FILE *cf = fopen(CONF_FILE,"rb");
	if(cf){
		struct Conference conf;
		while(fread(&conf,sizeof(struct Conference),1,cf)){
			if(&conf!=NULL){
				NET_CONF++;
			}
		}
		printf("** Total registered conferences: %d\n", NET_CONF);
		fclose(cf);
	}
	else{
		printf("** No existing conferences data found!\n");
		cf = fopen(CONF_FILE,"wb");
		fclose(cf);
		printf("** Created new confernces data file\n");
		NET_CONF = 0;
	}
}

int main()
{
	welcome();
	load_data();
	main_login_portal();
	return 0;
}