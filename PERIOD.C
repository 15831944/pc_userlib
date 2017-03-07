#include <stdio.h>
#include "gen_fun.h"
#include "next_day.h"

/******************************************
******************************************/

char *add_months(char *next, char *this_date, int mos)
{
	int D,M,Y,i, end_of_month=0;

	D = Day(this_date);
	M = Month(this_date);
	Y = Year(this_date);

	if(D == Days_in(M,Y))
		end_of_month = 1;

	for(i=0;i<mos;i++){
		M++;
		if(M==13){
			Y++;
			M=1;
		}
	}

	if(end_of_month || D >= Days_in(M,Y))
		sprintf_s(next, 11, "%02d %02d %04d",Days_in(M,Y),M,Y);
	else
		sprintf_s(next, 11, "%02d %02d %04d",D,M,Y);
	
	prev_day(next, next);
	return(next);
}

/******************************************
******************************************/
char *add_years(char *next, char *this_date, int yrs)
{
	int D,M,Y, end_of_month=0;

	D = Day(this_date);
	M = Month(this_date);
	Y = Year(this_date);

	if(D==Days_in(M,Y))
		end_of_month = 1;

	Y+=yrs;

	if(end_of_month || D >= Days_in(M,Y))
		sprintf_s(next, 11, "%02d %02d %04d",Days_in(M,Y),M,Y);
	else
		sprintf_s(next, 11, "%02d %02d %04d",D,M,Y);

	prev_day(next,next);
	return(next);
}

/******************************************
******************************************/
int	trans_days_in_period(char *begin, char *end, char *book, char *matures)
{
	int	period,first,last;

	if(cmp_dates(book,end)>0)
		return(0);
	if(cmp_dates(begin,matures)>=0)
		return(0);
	
	period = Days_between(end,begin)+1;
	
	first = Days_between(book,begin);
	if(first <= 0)
		first = 0;

	last = Days_between(matures,begin);
	if(last > period)
		last = period;
	return(last - first);
}
