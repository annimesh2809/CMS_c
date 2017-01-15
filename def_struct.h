#define EMAIL_LENGTH 20
#define PASS_LENGTH 20
#define MAX_PAPERS 20
#define _SZ 10
#define USER_FILE "users.dat"
#define CONF_FILE "conf.dat"

extern int NET_USERS;
extern int NET_CONF;

struct _AReviewer
{
	char name[30];
	char pass[PASS_LENGTH];
};

struct ReviewInfo
{
	int RID;
	int rating;
	char details[50];
	int isaccepted;
};

struct Paper
{
	int PID;
	int AID;
	struct ReviewInfo reviews[10];
	int isaccepted;
};

struct Conference
{
	int CONFID;
	int OCID[10];
	int PCID[10];
	int RID[20];
	struct _AReviewer AReviewer[_SZ];
	int AID[20];
	struct Paper papers[MAX_PAPERS];
	int ReviewsCompleted;
	char title[50], date[10], venue[20], tc[30], dl[30];
};

enum UType {NONE, OC, PC, AUTHOR, REVIEWER};

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
