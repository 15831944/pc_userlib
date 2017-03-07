#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "next_day.h"
#include "get_int.h"
#include "gen_fun.h"
#include "USERINFO.H"

/******************************************************************
******************************************************************/
double get_int(double amount, double rate, char* bdate, char* mdate,
				char* ind, char* roll, char acc)
{
	if(strncmp(ind,"30/360",6)== 0)
		return(get_int30_360(amount,rate,bdate,mdate,roll,acc)); 
	if(strncmp(ind,"A/365",5)== 0 )
		return(get_int365(amount,rate,bdate,mdate,roll));
	if(strncmp(ind,"A/360",5)== 0 )
		return(get_int360(amount,rate,bdate,mdate,roll));
	return 0;
}
/******************************************************************
******************************************************************/
int g30_360_accdays(char* fdate, char* ldate)
{
	int accdays=0, D1, M1, Y1, D2, M2, Y2;

	D1 = Day(fdate);
	M1 = Month(fdate); 
	Y1 = Year(fdate);
   
	D2 = Day(ldate);
	M2 = Month(ldate); 
	Y2 = Year(ldate);

	if(Y1 > Y2 || ( Y1==Y2 && M1 > M2))
		return(-1*g30_360_accdays(ldate,fdate));

	if(D2 == Days_in(2, Y2) && M2 == 2 && D1 == Days_in(2, Y1) && M1 == 2)
		D2 = 30;
	if(D1 == Days_in(2, Y1) && M1 == 2)
		D1 = 30;
   
	if(D2 == 31 && D1 >= 30)
		D2 = 30;

	if(D1 == 31) 
		D1=30;

	accdays = (Y2-Y1)*360+(M2-M1)*30+(D2-D1);
	return(accdays);
}
/******************************************************************
******************************************************************/
int g30_360U_accdays(char *fdate, char *ldate)
{
	int accdays=0, D1, M1, Y1, D2, M2, Y2;

	D1=Day(fdate);
	M1=Month(fdate); 
	Y1=Year(fdate);

	D2=Day(ldate);
	M2=Month(ldate); 
	Y2=Year(ldate);

	if(Y1 > Y2 || ( Y1==Y2 && M1>M2))
		return(-1*g30_360U_accdays(ldate,fdate));

	accdays = (Y2-Y1)*360+(M2-M1)*30+(D2-D1);
	return(accdays);
}
/******************************************************************
******************************************************************/
double get_int30_360(double amount, double rate, char* bdate, char* mdate, char* roll, char acc)
{
	int accdays=0, rev;
	char revdate[11];

	if(strncmp(roll,"MAT",3)==0)
	{
		rev = 0;
		SaveCopy(revdate, sizeof(revdate), bdate, 0);
		accdays = g30_360_accdays(revdate,mdate); 
	}
	else 
		if(strncmp(roll,"ME",2)==0)
		{
			if(Month(mdate)==Month(bdate) && Year(mdate)==Year(bdate))
				return((MIN(Day(mdate),30)-Day(bdate))*amount*10000*rate/360);
			else   
				return( (MIN(Day(mdate),30)-1)*amount*10000*rate/360);
		}
		else  
		{
			rev = atoi(roll);
			find_last_revdate(revdate,bdate,mdate,rev);
			if(Month(revdate)==Month(mdate) && Year(revdate) ==Year(mdate))
				find_last_revdate(revdate,bdate,revdate,rev);
			if(acc=='A')
				accdays = g30_360_accdays(revdate,mdate); 
			else              
			{
				int M,Y;
				
				M=Month(revdate)+1;
				Y=Year(revdate);
				if(M==13)
				{
					M=1;
					Y++;
				}
				
				accdays = 30 - MIN(Day(bdate),30) + 1;
				while(Month(mdate)>M || Year(mdate)>Y)
				{
					accdays +=30;
					M++;    
					if(M==13)
					{
						M=1;
						Y++;
					}
				}
				accdays += (MIN(Day(mdate),30) -1);
			}
		} 

	return(amount*rate*accdays*10000/360);
}
/******************************************************************
*******************************************************************/
double av_bal_acc(char *rate_ind, double rate, double amount, char *bdate, char *mdate,
				  int cur_month, int cur_year, char *roll, char acc)
{
	char revdate[11], last_day[11], first_day[11];

	int end_hols,i,days,basis,l,rev;
	double actions[31], acc_int;

	if(strncmp(roll,"ME",2)==0)
		return(0.0);

	for(i=0;i<31;i++)
		actions[i]=0.0;
	
	if(strncmp(rate_ind,"30/360",6)==0 || strncmp(rate_ind,"A/360",5)==0)
		basis = 360;
	else
		basis = 365;
	days = Days_in(cur_month,cur_year);

	sprintf_s(first_day, sizeof(first_day), "01 %02d %04d", cur_month, cur_year);

	if((i=(100*Year(bdate)+Month(bdate)-100*cur_year-cur_month))>0)
		return(0.0);
	else 
		if(i<0)
		{
			actions[0] += get_int(amount,rate,bdate,first_day,
			rate_ind,roll,acc);
		}

	sprintf_s(last_day, sizeof(last_day), "%02d %02d %04d", Days_in(cur_month,cur_year),cur_month,cur_year);

	for(end_hols = 0; ;end_hols++)
	{
		if(!is_holiday(last_day))
			break;
		prev_day(last_day,last_day);
	}

	if((i=(100*Year(mdate)+Month(mdate) - 100*cur_year-cur_month))<0)
		return(0.0);
	else 
		if(i==0)
		{
			l=Day(mdate);
			actions[l-1] -= get_int(amount,rate,bdate,mdate, rate_ind,roll,acc);
		}
		else 
			if(100*Year(bdate)+Month(bdate)<100*cur_year+cur_month
				&& strncmp(roll,"ME",2)!=0 && strncmp(roll,"MAT",3)!=0)
			{
				rev = atoi(roll);
				if(find_if_revdate(bdate,last_day,rev))
					SaveCopy(revdate, sizeof(revdate), last_day, 0);
				else    
					find_last_revdate(revdate,bdate,last_day,rev);
				if(Month(revdate)== cur_month && Year(revdate)== cur_year)
				{
					l= Day(revdate);
					if(strncmp(rate_ind,"30/360",6)==0)
						actions[l-1] -= g30_360_rev_int(amount,rate,rev,revdate,bdate,acc);
					else
						actions[l-1] -= actual_rev_int(amount,rate,bdate,revdate,rate_ind,rev);
				}
			}


	actions[days-1-end_hols] += 
			amount*rate*10000*calc_days(bdate,mdate,cur_month,cur_year,rate_ind)/basis;

	acc_int = actions[0];
	for(i=1;i<days;i++)
	{
		actions[i] += actions[i-1];
		acc_int += actions[i];
	}

	return(acc_int/days);
}

/**************************************************************

***************************************************************/
int calc_days(char *book_date, char *settle_date, int month, int year, char *rate_ind)
{
	int start,end;

	if(Year(book_date)*100+Month(book_date)>year*100+month)
		return(0);
	if(Year(settle_date)*100+Month(settle_date)<year*100+month)
		return(0);

	if (month == Month(book_date) && year == Year(book_date))
		start = Day(book_date)-1;
	else 
		start = 0;

	if (strncmp(rate_ind,"30/360",6)==0)
	{
		if(month == Month(settle_date)&& year == Year(settle_date)) 
			end = MIN(Day(settle_date),30)-1;
		else 
			end = 30;
	}
	else
	{
		if(month == Month(settle_date)&& year == Year(settle_date)) 
			end = Day(settle_date)-1;
		else 
			end = Days_in(month,year);
	}
	
	return (end-start);
}
/**************************************************************

***************************************************************/
void get_month_avs(double amount, double rate, char *rate_ind, char *bdate, char *mdate,
				   int cmonth, int cyear, char *roll, char acc, double *intrev, double *avacc)
{
	int basis;

	*intrev = 0.0;
	*avacc = 0.0;

	if(strncmp(rate_ind,"30/360",6)==0 || strncmp(rate_ind,"A/360",5)==0)
		basis = 360;
	else 
		basis = 365;

	*intrev = amount*rate*10000*calc_days(bdate,mdate,cmonth,cyear,rate_ind)/basis;
	*avacc = av_bal_acc(rate_ind,rate, amount,bdate,mdate,cmonth,cyear,roll,acc);
}
/**************************************************


**************************************************/

double g30_360_rev_int(double amount, double rate, int rev, char *revdate, char *bdate, char acc)
{
	char prevrev[11];
	int accdays;

	if(acc == 'S')
		return(amount*rate*10000*30*rev/360);

	find_last_revdate(prevrev,bdate,revdate,rev);
	accdays = g30_360_accdays(prevrev,revdate);
	return(amount*rate*10000*accdays/360);
}
/**************************************************


**************************************************/
double get_rev(double amount, double rate, char *bdate, char *revdate, char *rate_ind, 
			   int rev, char acc)
{
	if(strncmp(rate_ind,"30/360",6)==0)
		return(g30_360_rev_int(amount,rate,rev,revdate,bdate,acc));
	return(actual_rev_int(amount,rate,bdate,revdate,rate_ind,rev));
}
/**************************************************


**************************************************/

double actual_rev_int(double amount, double rate, char *bdate, char *revdate, char *rate_ind, int rev)
{
	char prev_revdate[11];
	int basis;

	if( strncmp(rate_ind,"A/360",5)==0)
	     basis = 360;
	else 
		basis = 365;

	find_last_revdate(prev_revdate,bdate,revdate,rev);
 	return(amount*rate*10000*Days_between(revdate,prev_revdate)/basis);
}

/**************************************************


**************************************************/

double get_int365(double amount, double rate, char *bdate, char *mdate, char *roll)
{
	int accdays=0, rev;
	char revdate[11];

	if(strncmp(roll,"MAT",3)==0)
		return(amount*rate*Days_between(mdate,bdate)*10000/365);
	else 
		if(strncmp(roll,"ME",2)==0)
		{
			if(Month(mdate)==Month(bdate) && Year(mdate)==Year(bdate))
				return((Day(mdate)-Day(bdate))*amount*10000*rate/365);
			else
				return( (Day(mdate)-1)*amount*10000*rate/365);
		}
		else  
		{
			rev = atoi(roll);
			find_last_actual_revdate(revdate,bdate,mdate,rev);
		} 

	accdays = Days_between(mdate,revdate);
	return(amount*rate*accdays*1000/365);
}

/**************************************************


**************************************************/
double  get_int360(double amount, double rate, char *bdate, char *mdate, char *roll)
{
	int accdays=0, rev;
	char revdate[11];

	if(strncmp(roll,"MAT",3)==0)
		return(amount*rate*Days_between(mdate,bdate)*10000/360);
	else 
		if(strncmp(roll,"ME",3)==0)
		{
			if(Month(mdate)==Month(bdate) && Year(mdate)==Year(bdate))
				return((Day(mdate)-Day(bdate))*amount*10000*rate/360);
			else
				return( (Day(mdate)-1)*amount*10000*rate/360);
		}
		else  
		{
			rev = atoi(roll);
			find_last_actual_revdate(revdate,bdate,mdate,rev);
		} 
	
	accdays = Days_between(mdate,revdate);
	return(amount*rate*accdays*10000/360);
}


/***************************************************************************

***************************************************************************/

double get_me(double amount, double rate, char *bdate, char *ind, int M, int Y)
{
	int books_in_month=0, D;
	
	D=Day(bdate);
	if(Month(bdate)==M && Year(bdate)==Y)
		books_in_month = 1;
	if(books_in_month)
	{
		if(strncmp(ind,"A/360",5)==0)
			return((Days_in(M,Y)-D+1)*amount*rate*10000/360);
		if(strncmp(ind,"A/365",5)==0)
			return((Days_in(M,Y)-D+1)*amount*rate*10000/365);
		if(strncmp(ind,"30/360",6)==0)
			return((31-D)*amount*rate*10000/360);
	}
	else 
	{
		if(strncmp(ind,"A/360",5)==0)
			return(Days_in(M,Y)*amount*rate*10000/360);
		if(strncmp(ind,"A/365",5)==0)
			return(Days_in(M,Y)*amount*rate*10000/365);
		if(strncmp(ind,"30/360",6)==0)
			return(30*amount*rate*10000/360);
	}
	return 0;
}
