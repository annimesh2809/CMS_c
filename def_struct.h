#define EMAIL_LENGTH 20
#define PASS_LENGTH 20
#define USER_FILE "users.dat"

extern int NET_USERS;

struct _AReviewer
{
	char name[30];
	char pass[PASS_LENGTH];
};

struct ReviewInfo
{
	int RID;
	int rating;
	char *details;
	int isaccepted;
};

struct Paper
{
	int PID;
	int AID;
	struct ReviewInfo *reviews;
	int isreviewed;
	int isaccepted;
};

struct Conference
{
	int CONFID;
	int OCID;
	int PCID;
	int RID;
	struct _AReviewer *AReviewer;
	int AID;
	struct Paper *papers;
	int ReviewsCompleted;
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
