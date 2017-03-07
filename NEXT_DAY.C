#include <stdio.h>
#include <string.h>
#include "gen_fun.h"
#include "next_day.h"
#include "get_hols.h"
#include "USERINFO.H"


/******************************************************************

******************************************************************/

char *next_day(char *dest, char *src)
{
	if(Day(src) < Days_in(Month(src),Year(src)))
		sprintf_s(dest, 11, "%02d %02d %04d",Day(src)+1,Month(src),Year(src));
	else 
		if(Month(src) != 12)
			sprintf_s(dest, 11, "01 %02d %04d",Month(src)+1,Year(src));
		else 
			sprintf_s(dest, 11, "01 01 %04d",Year(src)+1);

	return(dest);
}

/******************************************************************

******************************************************************/

char *prev_day(char *dest, char *src)
{
	if(Day(src) > 1)
		sprintf_s(dest, 11, "%02d %02d %04d",Day(src)-1,Month(src),Year(src));
	else 
		if(Month(src) != 1)
			sprintf_s(dest, 11, "%02d %02d %04d",Days_in(Month(src)-1,Year(src)), Month(src)-1,Year(src));
		else 
			sprintf_s(dest, 11, "31 12 %04d",Year(src)-1);
	
	return(dest);
}

/******************************************************************

******************************************************************/

char *add_to_date(char *dest, char *src, int days)
{
	int	i;
	char date[11];

	SaveCopy(date, sizeof(date), src, 0);

	if(days == 0)
		SaveCopy(dest, sizeof(date), date, 0);
	else
		if(days > 0)
			for(i=1;i<=days;i++)
			{
				next_day(dest, date);
				SaveCopy(date, sizeof(date), dest, 0);
			}
		else 
			for(i=1;i<=(-1)*days;i++)
			{
				prev_day(dest,date);
				SaveCopy(date, sizeof(date), dest, 0);
			}
	
	return(dest);
}

/******************************************************************

******************************************************************/

int	is_holiday(char *date)
{
	int	i;
	
	if(Day_of_week(date)==6 || Day_of_week(date)==0)
		return(1);

	for(i=0; strcmp(holidays[i].h,"END"); i++)
		if(strcmp(holidays[i].h,date) == 0)
			return(1);

	return(0);
}

/******************************************************************

******************************************************************/

char *next_work_day(char *out, char *in)
{
	char date[11];
	
	SaveCopy(date, sizeof(date), in, 0);
	while(is_holiday(next_day(out,date)))
		SaveCopy(date, sizeof(date), out, 0);

	return(out);
}

/******************************************************************

******************************************************************/

int	eom(char *date)
{
	char next[11];
	if(Month(date) == Month(next_work_day(next,date)))
		return(0);

	return(1);
}

/******************************************************************

******************************************************************/

char *prev_work_day(char *out, char *in)
{
	char date[11];

	SaveCopy(date, sizeof(date), in, 0);
	while(is_holiday(prev_day(out,date)))
		SaveCopy(date, sizeof(date), out, 0);
	return(out);
}

/******************************************************************

******************************************************************/

char *rev_date(char *rev, char *book, int mos)
{
	int	correct;
	char date[11];

	sprintf_s(rev, 11, "%02d %02d %04d", Day(book), (Month(book)+mos-1)%12+1,
			Year(book)+(int)((Month(book)+mos-1)/12));
	correct=Month(rev);

	if(Day(rev)>Days_in(Month(rev),Year(rev)))
		sprintf_s(rev, 11, "%02d %02d %04d", Days_in(Month(rev),Year(rev)), Month(rev),
	
	Year(rev));
	SaveCopy(date, sizeof(date),rev, 0);

	if(is_holiday(rev))
	{
		next_work_day(rev, date);
		if(Month(rev) != correct)
			prev_work_day(rev,date);
	}

	return(rev);
}

/******************************************************************

******************************************************************/
char *adjust(char *adjdate, char *the_date)
{
	if(is_holiday(the_date))
	{
		next_work_day(adjdate,the_date);
		if(Month(adjdate) != Month(the_date))
			prev_work_day(adjdate,the_date);
	}
	else 
		strncpy_s(adjdate, 12,the_date,11);
	
	return(adjdate);
}

/******************************************************************

******************************************************************/

char *actual_revdate(char *rev, char *book, int mos)
{
	sprintf_s(rev, 11, "%02d %02d %04d",Day(book), (Month(book)+mos-1)%12+1,
			Year(book)+(int)((Month(book)+mos-1)/12));

	if(Day(rev)>Days_in(Month(rev),Year(rev)))
		sprintf_s(rev, 11, "%02d %02d %04d", Days_in(Month(rev),Year(rev)), Month(rev),
	
	Year(rev));
	return(rev);
}

/******************************************************************

******************************************************************/

int cmp_dates(char *date1, char *date2)
{
	int Y1,Y2,M1,M2,D1,D2;

	if(strncmp(date1,date2,10)==0)
		return(0);
	Y1=Year(date1);
	Y2=Year(date2);
	if(Y1>Y2)
		return(1);
	if(Y2>Y1)
		return(-1);
	
	M1=Month(date1);
	M2=Month(date2);
	if(M1>M2)
		return(1);
	if(M2>M1)
		return(-1);
	
	D1=Day(date1);
	D2=Day(date2);
	if(D1>D2)
		return(1);
	return(-1);
}

/******************************************************************

******************************************************************/

char *find_next_revdate(char *rev, char *book, char *curr, int period)
{
	int i;

	for (i=1;;i++)
	{
		rev_date(rev,book,i*period);
		if(cmp_dates(rev,curr)==1)
			return(rev);
	}

	return NULL;
}

/******************************************************************

******************************************************************/

char *find_last_actual_revdate(char *rev, char *book, char *curr, int period)
{
	char date[11];
	int i;

	SaveCopy(date, sizeof(date), book, 0);
	for (i=1;;i++)
	{
		SaveCopy(rev, 11, date, 0);
		actual_revdate(date,book,i*period);
		if(cmp_dates(date,curr)>=0)
		return(rev);
	}

	return NULL;
}

/******************************************************************

******************************************************************/

char *find_last_revdate(char *rev, char *book, char *curr, int period)
{
	char date[11];
	int i;

	SaveCopy(date, sizeof(date), book, 0);
	for (i=1;;i++)
	{
		SaveCopy(rev, sizeof(date), date, 0);
		rev_date(date,book,i*period);
		if(cmp_dates(date,curr)>=0)return(rev);
	}
}

/******************************************************************

******************************************************************/

int	find_if_revdate(char *book, char *curr, int period)
{
	int i;
	char rev[11];

	for (i=1;;i++)
	{
		rev_date(rev,book,i*period);
		if(cmp_dates(rev,curr)==0)
			return(1);
		if(cmp_dates(rev,curr)==1)
			return(0);
	}
}
/******************************************************************

******************************************************************/

int	find_if_actual_revdate(char *book, char *curr, int period)
{
	int i;
	char rev[11];

	for (i=1;;i++)
	{
		actual_revdate(rev,book,i*period);
		if(cmp_dates(rev,curr)==0)
			return(1);
		if(cmp_dates(rev,curr)==1)
			return(0);
	}
}

/*****************************************************************************
FUNCTION TO CALCULATE WORK DAYS BETWEEN  TWO DATES ;

input		: char   *after, *before

Global variables
used		: None
variables 
affected	: none

functions used  : Days_in_Month(), Month(), Year(), Day(),is_holiday();

output		: returns an integer value for the number of days.


******************************************************************************/

int	Work_days_between(char *after, char *before)
{
	int work_days=0;
	char curr[11];

	if(cmp_dates(after,before)==0)
		return(0);
	else 
		if(cmp_dates(after,before)<0)
			return(-1*Work_days_between(before,after));

	SaveCopy(curr, sizeof(curr), before, 0);
	while(cmp_dates(after,curr)>0)
	{
		next_day(curr,curr);
		if(is_holiday(curr)!=1)
			work_days++;
	}
	return(work_days);
}
