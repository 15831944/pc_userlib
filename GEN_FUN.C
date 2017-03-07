#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gen_fun.h"
#include "USERINFO.H"

void spri_var(char *, int size, char *,int,double);
char *months[]= {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
char *days[]= {"SUN","MON","TUE","WED","THU","FRI","SAT"};

/* **************************************************************************

	Function to extract Day from Date
	Takes Character String date of format "dd mm yyyy"
	(Null terminated)
	Returns int Day

	Written May 16,1989
	Programmer EGH
*****************************************************************************/
int Day(char *date)
{
	char  D[3];

	strncpy_s(D, sizeof(D), date, 2);
	D[2] = 0;
	return(atoi(D));
}
/* **************************************************************************

	Function to extract Month from Date
	Takes Character String date of format "dd mm yyyy"
	(Null terminated)
	Returns int month

	Written May 16,1989
	Programmer EGH
*****************************************************************************/
int Month(char *date)
{
	char M[3];

	strncpy_s(M, sizeof(M), &date[3], 2);
	M[2] = 0;
	return(atoi(M));
}
/* **************************************************************************

	Function to extract Year from Date
	Takes Character String date of format "dd mm yyyy"
	(Null terminated)
	Returns int year

	Written May 16,1989
	Programmer EGH
*****************************************************************************/
int Year(char *date)
{
	char Y[5];

	SaveCopy(Y, sizeof(Y), &date[6], 0);
	return(atoi(Y));
}

/* **************************************************************************

	Function to calculate Days in any given month 
	Takes Integer month, Integer year
	Returns int Days

	Makes call to function leap()

	Written May 16,1989
	Programmer EGH
*****************************************************************************/
int Days_in(int month, int year)
{
	switch (month) 
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return (31);
		case 4:
		case 6:
		case 9:
		case 11:
			return (30);
		case 2:
			if(leap(year))
				return(29);
			else 
				return(28);
		default:
			return 0;
	}
}
/* **************************************************************************

	Function to check for leap year
	Takes integer year
	
	Returns 1 if leap , 0 if not.

	Written May 16,1989
	Programmer EGH
*****************************************************************************/
int leap(int year)
{
	if (year % 4 != 0)
		return(0);
	else 
		if(year % 100 == 0 && year % 400 != 0)
			return(0);
	else 
		return(1);
}
/* **************************************************************************

	Function to validate Date input
	Takes Character String date of format "dd mm yyyy"
	(Null terminated)
	Returns 1 if not valid, 0 if valid

	Makes use of functions
	Day(),Month(),Year(),Days_in()

	Written May 16,1989
	Programmer EGH
*****************************************************************************/
int not_valid_date(char *date)
{
	if(Day(date)<1 || Month(date)<1 || Year(date) < 1000 || Month(date)>12 )
		return(1);
	if( Day(date) > Days_in ( Month(date) , Year(date) ))
		return(1);
	if(date[2]!=' ' || date[5]!=' ') 
		return(1);
	
	return(0);
}

/* **************************************************************************

	Function to get number of digits in integer
	Takes integer i 
	
	Returns size

	Written May 20,1989
	Programmer EGH
*****************************************************************************/
int int_size(int i)
{
	int cntr;
	
	for(cntr=0; i>0 ;++cntr)
		i=i/10;

	return(cntr);
}
/******************************************************************************

FUNCTION TO FIND DAYS OF WEEK;

input		: char   *date

Global variables
used		: None
variables 
affected	: none

functions used  : Days_between();
output		: returns a integer value for the day of week.

******************************************************************************/
int Day_of_week(char *date)
{
	int num,res;

	num = Days_between(date,"31 12 1989");
	res = num % 7;
	if(res < 0) 
		res += 7;
	return(res);
}

/*****************************************************************************
FUNCTION TO CALCULATE DAYS BETWEEN  TWO DATES ;

input		: char   *after, *before

Global variables
used		: None
variables 
affected	: none

functions used  : Days_in_Month(), Month(), Year(), Day()

output		: returns an integer value for the number of days.


******************************************************************************/
int	Days_between(char *after, char *before)
{
	int days=0,M,Y;

	if((Year(after)<Year(before)) || 
		(Year(after)==Year(before) && Month(after)<Month(before)) ||
		(Year(after)==Year(before) && Month(after)==Month(before) && Day(after)<Day(before)))
		return(-1*Days_between(before,after));

	if(Month(after)==Month(before) && Year(after)==Year(before))
		return(Day(after)-Day(before));

	days = Days_in(Month(before),Year(before))-Day(before);
	M=Month(before);
	Y=Year(before);
	while(Y<Year(after) || M<(Month(after)-1))
	{
		if (Year(after)==Y+1 && M==12 && Month(after)== 1)
			break;
		M++;
		if(M>12)
		{
			M=1;
			Y++;
		}
		days += Days_in(M,Y);

	}
	
	days += Day(after);
	return(days);
}

/* **************************************************************************

	Function to format float output with commas
	Takes 	double d
		int    len
		int    dec
		char   *output
	Returns  Pointer to formated string

	Written July 20,1989
	Programmer EGH
	Modified  Feb 28 1990 to round instead of truncating
*****************************************************************************/
char *commas(double d, int len, int dec, char *output, int out_size)
{
	char temp[30];
	int i,flag=0;
	double decimal,nondec, digit;

	if (d<0) 
	{
		flag=1;
		d *= -1;
	}
	
	output[0] = 0;
	if (len < dec+2 || len >30 || dec > 9||dec < 0)
		return("Wrong format");

	if(d == 0 || round(d, dec) == 0) // if zero
	{
		sprintf_s(temp, 30, "%%0.%dlf", dec);
		sprintf_s(output, out_size, temp, d);
		return output;
	}

	decimal = modf(d, &nondec);
	if(dec) 
	{
		if(round(decimal,dec)>=1)
		{
			nondec++;
			decimal = 0.0;
		}	
	}
	else
		decimal = 0.0;

	if(dec > 0)
	{
		sprintf_s(temp, sizeof(temp), "%%0.%dlf", dec);
		sprintf_s(output, out_size, temp, decimal);
		if(nondec < 1) // if decimal only
			return output;
		SaveCopy(output, out_size, output + 1, 0); // remove zero
	}
    
	for (i = 1; i <= len && nondec >= 1;i++)
	{		
		digit = 10*modf(nondec/10, &nondec);
		if(i !=1 && i%3==1)
			sprintf_s(temp, sizeof(temp), "%1.0lf,%s", digit, output);
		else
			sprintf_s(temp, sizeof(temp), "%1.0lf%s", digit, output);		
		SaveCopy(output, out_size, temp, 0);
	}
	
	if(flag)
	{
		SaveCopy(temp, sizeof(temp), "-", 0);
		SaveConcatenate(temp, sizeof(temp), output);
		SaveCopy(output, out_size, temp, 0);
	}

	return(output);
}
/* **************************************************************************

	Function to round floating point numbers
	Takes 	double number
		int    perci
	Returns double

	Written Feb 28,1990
	Programmer EGH

	Modified Jun 28,1990
	To round negative numbers.
*****************************************************************************/
double round(double number, int perci)
{
	double num1=0,num2=0;

	if(number < 0.0)
		return(-1*round((-1.0*number),perci));

	num1 = number * pow(10,perci);
	if (modf(num1,&num2) >= .5)
		num2++;
	return(num2 / pow(10,perci));
}

/* **************************************************************************

	Function to truncate floating point numbers
	Takes 	double number
		int    perci
	Returns double

*****************************************************************************/
double trunc(double number, int perci)
{
	double num1=0,num2=0;
	
	if(number < 0.0)
		return(-1*round((-1.0*number),perci));
	num1 = number * pow(10,perci);
	modf(num1,&num2);
	return(num2 / pow(10,perci));
}

/****************************************************************************



*****************************************************************************/
void spri_var(char *string, int s_size, char *insert, int i, double num)
{
	switch (i)
	{
		case 1:
			sprintf_s(string,s_size,"%s%01.0f",insert,num);
			break;
		case 2:
			sprintf_s(string,s_size,"%s%02.0f",insert,num);
			break;
		case 3:
			sprintf_s(string,s_size,"%s%03.0f",insert,num);
			break;
		case 4:
			sprintf_s(string,s_size,"%s%04.0f",insert,num);
			break;
		case 5:
			sprintf_s(string,s_size,"%s%05.0f",insert,num);
			break;
		case 6:
			sprintf_s(string,s_size,"%s%06.0f",insert,num);
			break;
		case 7:
			sprintf_s(string,s_size,"%s%07.0f",insert,num);
			break;
		case 8:
			sprintf_s(string,s_size,"%s%08.0f",insert,num);
			break;
		case 9:
			sprintf_s(string,s_size,"%s%09.0f",insert,num);
			break;
	}
}

/* **************************************************************************

	Function convert a character string to Upper Case Letters.
	+++ String must be Null Terminated.
	Takes char* (pointer to the string)
		
	Returns void 

	Written June, 1990
	Programmer EGH
*****************************************************************************/
void upperize(char *fraze)
{
	int i;

	for(i = 0; fraze[i] != 0; i++)
		fraze[i]=toupper(fraze[i]);
}
/*****************************************************************************/
void lower(char *fraze)
{
	int i;

	for(i = 0; fraze[i] != 0; i++)
		fraze[i]=tolower(fraze[i]);
}
/* **************************************************************************

	Function convert a character date string from 'dd mm yyyy'
	fromat to 'dd-mon-yy' oracle default format.
	+++ String must be Null Terminated.
	Takes 	dest	char* (pointer to the string)
		orig	char*
	Returns pointer to destination string.

	Written June ,1990
	Programmer EGH
*****************************************************************************/
char *conv_date(char *dest, char *orig)
{
	if(not_valid_date(orig))
	{
		printf("\nInvalid Date Format.");
		return(NULL);
	}

	sprintf_s(dest,10,"%02d-%s-%02d",Day(orig),months[Month(orig)-1],Year(orig)%100);
	return(dest);
}
/* **************************************************************************

	Function to return the day of month of
	month mon ,year yr that is the num th 
	"day" of week (0=Sunday ... 6=Saturday)

	Written Aug ,1990
	Programmer EGH
*****************************************************************************/
int	find_day_in_month(int num, int day, int mon, int yr)
{
	char the_date[11];
	int i,count=0;

	for(i=1; i<32 ;i++)
	{
		sprintf_s(the_date, 11, "%02d %02d %04d",i,mon,yr);
		if(Day_of_week(the_date) == day) 
			count++;
		if(count == num)
			return(Day(the_date));
	}

	return 0;
}
/***************************************************************************

	Function convert a character date string from 'dd-mon-yy'
	fromat to 'dd mm yyyy' format.
	+++ String must be Null Terminated.
	Takes 	dest	char* (pointer to the string)
		orig	char*
	Returns pointer to destination string.

	Written Oct ,1990
	Programmer EGH
*****************************************************************************/
char *conv_back_date(char *dest, char *orig)
{
	char mon[4];
	int	i,yr;

	sprintf_s(mon, 4, "%.3s",&orig[3]);
	yr=atoi(&orig[7]);
	for(i=0;i<12;i++)
		if(strncmp(mon,months[i],3)==0)
			break;

	if(yr<30)
		yr+=2000;
	else
		if(yr<100)
			yr+=1900;

	sprintf_s(dest, 11, "%02d %02d %04d",Day(orig),i+1,yr);
	return(dest);
}


char* strrtrim(char* str)
{
	int i;

	i = strlen(str) ? strlen(str) - 1 : 0;
	while(*(str + i) == ' ')
		i--;

	if(i < 0)
		str[0] = 0;
	else
		str[i+1] = 0;

	return(str);
}
/*********************************************************
*********************************************************/
char* strltrim(char* str)
{
	int i = 0;

	while(*(str + i) == ' ')
		i++;

	memmove(str, str+i, strlen(str) - i + 1);

	return(str);
}
/*********************************************************
*********************************************************/
char* strtrim(char* str)
{
	return strltrim(strrtrim(str));
}

/*************************************************************************
*************************************************************************/
char* StrRTrim(char* str, char TrimedChar)
{
	int i;

	i = strlen(str) ? strlen(str) - 1 : 0;
	while(*(str + i) == TrimedChar)
		i--;

	if(i < 0)
		str[0] = 0;
	else
		str[i+1] = 0;

	return(str);
}
/*********************************************************
*********************************************************/
char* StrLTrim(char* str, char TrimedChar)
{
	int i = 0;

	while(*(str + i) == TrimedChar)
		i++;

	memmove(str, str+i, strlen(str) - i + 1);

	return(str);
}

/*********************************************************
*********************************************************/
char* StrTrim(char* str, char TrimedChar)
{
	return StrLTrim(StrRTrim(str, TrimedChar), TrimedChar);
}
/***************************************************************************
	Function IsIn if str contain instr return 1 else return 0	

	Written Oct 19, 2001
	Programmer LX
*****************************************************************************/
int IsIn(const char *str, const char *instr)
{
	return strlen(str) == strlen(instr) && strstr(str, instr) ? 1 : 0;
}
/***************************************************************************
	Function get input string if the string is "EXIT" return 0 else return
	strlen of the string.

	Written Oct 19, 2001
	Programmer LX
*****************************************************************************/
int GetInput(const char *Prompt, char *Buf)
{
	printf("\n%s", Prompt);
	gets_s(Buf, 80);
	upperize(Buf);
	strtrim(Buf);
	if(IsIn(Buf, "EXIT"))
		return(0);
	return strlen(Buf);
}
/***************************************************************************
	Function get date input in 'dd mm yyyy' format and convert it to 'dd-mon-yy' format
	Returns 0 for fail and strlen of the date for success

	Written Sept 27, 2001
	Programmer LX
*****************************************************************************/
int GetDate(const char *Prompt, char *Date)
{
	char buf[100];

	for(;;)
	{
		if(GetInput(Prompt, buf) == 0)
			return 0;
		if(not_valid_date(buf))
			printf("\nInvalid Entry! Please enter DD MM YYYY eg. 01 12 1993 for Dec 1,93 ");
		else
			break;
	}

	conv_date(Date, buf);
	printf("\nYou have entered %s.", Date);
	return(strlen(Date));
}
/***************************************************************************
	Function get condition string input which end with "END"

	Written Sept 27, 2001
	Programmer LX
*****************************************************************************/
int GetConditions(const char *Prompt, char* conditions, int con_size)
{
	char buf[100];

	conditions[0] = 0;
	for(;;)
	{
		if(GetInput(Prompt, buf) == 0)
			return 0;

		if(IsIn(buf, "END"))
			return strlen(conditions);
		
		if(IsIn(buf, "ALL"))
		{
			SaveCopy(conditions, con_size, "ALL", 0);
			return strlen(conditions);
		}

		if(strlen(buf) > 0)
		{
			if(strlen(conditions) > 0)
				SaveConcatenate(conditions, con_size, ",");
			SaveConcatenate(conditions, con_size, "'");
			SaveConcatenate(conditions, con_size, buf);
			SaveConcatenate(conditions, con_size, "'");
		}
	}
}

void DateToYearMonthDay(const char *Date, int *nYear, int *nMonth, int *nDay)
// DD-MON-YY, DD-MOM-YYYY, YYYYMMDD
{
	int i = 0, j;
	char Buf[100], *p, *q;

	strcpy_s(Buf, sizeof(Buf), Date);
	p = strstr(Buf, "-");
	
	if(p) // Date in DD-MON-YY, DD-MOM-YYYY format
	{
		p = strtok_s(Buf, "-", &q);
		while(p)
		{
			switch(i)
			{
				case 0:
					*nDay = atoi(p);
					break;
				case 1:
					for(j = 0; j < 12; j++)
						if(strcmp(months[j], p) == 0)
							*nMonth = j + 1;
					break;
				case 2:
					*nYear = atoi(p);
					if(*nYear < 100)
						*nYear = *nYear + 2000;
					break;
				default:
					break;
			}

			p = strtok_s(NULL, "-", &q);
			i++;
		}
	}
	else
	{
		Buf[4] = 0;
		*nYear = atoi(Buf);
		strncpy_s(Buf, sizeof(Buf), Date + 4, 2);
		Buf[2] = 0;
		*nMonth = atoi(Buf);
		strncpy_s(Buf, sizeof(Buf), Date + 4, 2);
		Buf[2] = 0;
		*nDay = atoi(Buf);
	}
}

int GetJulianDays(int nYear, int nMonth, int nDay)
{
	char Buf[30];
	int  Mp, Yp, T;
	long Days = 1;

	if(nMonth <= 2)
	{
		Mp = 0;
		Yp = nYear - 1;
	}
	else
	{
		sprintf_s(Buf, sizeof(Buf), "%lf", 0.4*nMonth + 2.3);
		Mp = atoi(Buf);
		Yp = nYear;
	}

	T = Yp/4 - Yp/100 + Yp/400;
	Days = Days*365*nYear + 31*(nMonth - 1) + nDay + T - Mp;

	return Days;
}