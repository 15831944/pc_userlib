#ifndef _LOGIN_DEFINED
#define NOECHO  0
#define DISPLAY 1

extern char sLoginCommand[100];
void getString(char *, int);
int login(const char *db);
int logon(const char *user, const char *password, const char *db);
#define _LOGIN_DEFINED			1
#endif
