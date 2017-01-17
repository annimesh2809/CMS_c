#define EMAIL_LENGTH 20
#define PASS_LENGTH 20
#define MAX_PAPERS 20
#define PATH_LENGTH 100
#define _SZ 10
#define USER_FILE "build/users.dat"
#define CONF_FILE "build/conf.dat"

extern int NET_USERS;
extern int NET_CONF;

void oc_portal(int);
void pc_portal(int);
void reviewer_portal(int);
void author_portal(int);
struct UserF* getUserByID(int);
int getchoice(char *,int);

struct _AReviewer
{
	char email[30];
	char pass[PASS_LENGTH];
};

struct ReviewInfo
{
	int RID;
	int rating;			//out of 10
	char details[50];
	int isaccepted;
};

struct Paper
{
	int PID;
	int AID;
	int nor,noar;
	char title[50], abstract[100], affiliation[100];
	struct ReviewInfo reviews[10];
	struct _AReviewer AReviewer[_SZ];
	char src[PATH_LENGTH];
	int isaccepted;
};

struct Conference
{
	int CONFID;
	int nooc,nopc,nor,noa,nop;
	int OCID[10];
	int PCID[10];
	int RID[20];
	int AID[20];
	struct Paper papers[MAX_PAPERS];
	int ReviewsCompleted;
	char title[50], date[10], venue[20], tc[30], dl[30];
};

enum UType {NONE = 0, OC = 1, PC = 2, REVIEWER = 3, AUTHOR = 4};

struct UserF
{
	int UID;
	enum UType type;
	char name[30];
	char email[EMAIL_LENGTH];
	char pass[PASS_LENGTH];
	char affl[30];
	char expertise[20];
};
