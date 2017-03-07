#ifndef _NEXT_DAY_DEFINED

/*************************************************
next_day.h  Header File
*************************************************/
char *next_day(char*, char*);
char *prev_day(char*, char*);
char *add_to_date(char*, char*, int);
char *find_next_revdate(char*,char*,char*,int);
char *find_last_actual_revdate(char*,char*,char*,int);
int	find_if_revdate(char*,char*,int);
int	find_if_actual_revdate(char*,char*,int);
char *find_last_revdate(char*, char*, char*, int);
int	is_holiday(char*);
int cmp_dates(char*, char*);
int	eom(char*);
char	*next_work_day(char*,char*);
char	*prev_work_day(char*,char*);
char	*adjust(char*,char*);
char	*rev_date(char*,char*,int);
char	*actual_revdate(char*,char*,int);
int	Work_days_between(char*,char*);

#define _NEXT_DAY_DEFINED
#endif
