#include <stdio.h>
#include <process.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sys\types.h> 
#include <sys\stat.h> 
#include "userinfo.h"
#include <stdlib.h>
#include "asset.h"
#include "login.h"
#include "gen_fun.h"


/************************************************
************************************************/
int IsDateValid(const char* date)
{
	DateType FmsDate;
	int      MaxDays = 31;

	FmsDateGetDayMonthYear(&FmsDate, date);
	if(FmsDate.m_nMonth < 1 || FmsDate.m_nMonth > 12)
	{
		printf("Invalid month\n");
		return 0;
	}

	switch(FmsDate.m_nMonth)
	{
		case 4:
		case 6:
		case 9:
		case 11:
			MaxDays = 30;
			break;

		case 2:
			MaxDays = 28 + FmsDateIsLeapYear(&FmsDate);
			break;

		default:
			break;
	}

	if(FmsDate.m_nDay <= 0 || FmsDate.m_nDay > MaxDays)
	{
		printf("Invalid days per month\n");
		return 0;
	}

	if(FmsDate.m_nYear <= 1900)
	{
		printf("Invalid Year\n");
		return 0;
	}

	return 1;
}

char *GetFileName(char *FileName, int s_Size, const char *EnvVar, const char *FName)
{
	size_t requireSize;

	getenv_s(&requireSize, FileName, s_Size, EnvVar);  

	if(strlen(FileName) > 0)
		SaveConcatenate(FileName, s_Size, "\\");
	SaveConcatenate(FileName, s_Size, FName);

	return FileName;
}

FILE *OpenOutputFile(const char *FName, const char *EnvVar)
{
	FILE *fp;
	char FileName[200];

	GetFileName(FileName, sizeof(FileName), EnvVar, FName);
	fopen_s(&fp, FileName, "w+");
	if(!fp)
		printf("Fail to open %s\n", FileName);

	return fp;
}

FILE *OpenInputFile(const char *FName, const char *EnvVar)
{
	FILE *fp;
	char FileName[200];

	GetFileName(FileName, sizeof(FileName), EnvVar, FName);
	if(FileSize64(FileName) == 0)
		return 0;

	fopen_s(&fp, FileName, "r");
	if(!fp)
		printf("Fail to open %s\n", FileName);

	return fp;
}

void write_userinfo(const char* username, const char* password, const char* db)
{
   FILE* fp;
   unsigned i;

   fopen_s(&fp, "userinfo.txt", "w");
   if(!fp)
      return;

   fprintf(fp, "%s\n", username);
   for(i = 0; i < strlen(password); i++)
      fprintf(fp, "%d\n", (13*password[i] + 7)*3);
   fprintf(fp, "%s\n", db);
   
   fclose(fp);
}

int read_userinfo(char* username, char* password, char *db)
{
   FILE* fp;
   char  buf[80];
   int   i = 0;
   char* p;

   username[0] = 0;
   password[0] = 0;
   fopen_s(&fp, "userinfo.txt", "r");
   if(!fp)
      return 0;
   
   fgets(buf, sizeof(buf), fp);
   p = strchr(buf, '\r');
   if(p)
     p[0] = 0;
   p = strchr(buf, '\n');
   if(p)
     p[0] = 0;
   SaveCopy(username, strlen(buf) + 1, buf, 0);

   while (!feof(fp))
   {
      buf[0] = 0;
      fgets(buf, sizeof(buf), fp);
      p = strchr(buf, '\r');
      if(p)
         p[0] = 0;
      p = strchr(buf, '\n');
      if(p)
         p[0] = 0;
      
      if(strlen(buf) > 0)
      {
         password[i] = (atoi(buf)/3 - 7)/13;
         i++;
      }
   } 

   password[i] = 0;

   fgets(buf, sizeof(buf), fp);
   p = strchr(buf, '\r');
   if(p)
     p[0] = 0;
   p = strchr(buf, '\n');
   if(p)
     p[0] = 0;
   SaveCopy(db, strlen(buf) + 1, buf, 0);

   fclose(fp);
   return 1;
}

void GenerateSelList(int Max, char *AvailList[], char *SelList, int sSize, int Start)
{
	int i;

	SelList[0] = 0;

	for(i = Start; i < Max; i ++)
	{
		if(strlen(SelList) > 0)
			SaveConcatenate(SelList, sSize, ", ");
		sprintf_s(SelList + strlen(SelList), sSize, "'%s'", AvailList[i]);
	}
	
	upperize(SelList);
}


void MMDDYYYYToDDMONYY(const char *SrcDate, char *Date)
{
	char Buf[100], *Token, *p, *q;
	char *Months[] = {"", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", 
					"JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
	int i = 0, Day, Month, Year;

	if(!SrcDate || strlen(SrcDate) == 0)
		return;

	SaveCopy(Buf, sizeof(Buf), SrcDate, 0);
	
	Date[0] = 0;

	if(strstr(Buf, "/"))
		Token = "/";
	else
		if(strstr(Buf, "-"))
			Token = "-";
		else
			return;

	p = strtok_s(Buf, Token, &q);
	while(p)
	{
		switch(i)
		{
			case 0: // Month
				Month = atoi(p);
				break;
			case 1: // Day
				Day = atoi(p);
				break;
			case 2:
				Year = atoi(p);
				break;
			default:
				break;
		}
		i++;
		p = strtok_s(NULL, Token, &q);
	}

	if(Day > 0 && Day <= 31 && Month > 0 && Month <= 12 && Year >= 0)
		sprintf_s(Date, 10, "%02d-%s-%02d", Day, Months[Month], Year % 100);
}

void MMDDYYYYToDDMONYYYY(const char *SrcDate, char *Date)
{
	char Buf[100], *Token, *p, *q;
	char *Months[] = {"", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", 
					"JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
	int i = 0, Day, Month, Year;

	if(!SrcDate || strlen(SrcDate) == 0)
		return;

	SaveCopy(Buf, sizeof(Buf), SrcDate, 0);
	
	Date[0] = 0;

	if(strstr(Buf, "/"))
		Token = "/";
	else
		if(strstr(Buf, "-"))
			Token = "-";
		else
			return;

	p = strtok_s(Buf, Token, &q);
	while(p)
	{
		switch(i)
		{
			case 0: // Month
				Month = atoi(p);
				break;
			case 1: // Day
				Day = atoi(p);
				break;
			case 2:
				Year = atoi(p);
				break;
			default:
				break;
		}
		i++;
		p = strtok_s(NULL, Token, &q);
	}

	if(Day > 0 && Day <= 31 && Month > 0 && Month <= 12 && Year >= 0)
		sprintf_s(Date, 12, "%02d-%s-%04d", Day, Months[Month], Year % 10000);
}

int ProcessInputInfoBase(int argc, char *argv[])
// Support c:\Program
//		   c:\Program user password
//         c:\program user password dbstring
{
	switch (argc)
	{
		case 1:
			if(login(0))
				return 1;
			break;
		case 2:
			printf("Invalid number of parameters\n");
			break;
		case 3:
			if(logon(argv[1], argv[2], 0))
				return 1;
			break;
		case 4:
			if(logon(argv[1], argv[2], argv[3]))
				return 1;
			break;
		default:
			break;
	}

	return 0;
}

int ProcessInputInfo(int argc, char* argv[], char *PortList, int PortSize)
// Support C:\Program user password dbstring portfolio1 portfolio2 ...
{
	PortList[0] = 0;

	if(argc > 4)
	{
		GenerateSelList(argc, argv, PortList, PortSize, 4);
		argc = 4;
	}

	return ProcessInputInfoBase(argc, argv);
}

int ProcessInputInfo1(int argc, char* argv[], char *PortList, int PortSize, char *Date)
// Support C:\Program user password dbstring date portfolio1 portfolio2 ...
{
	PortList[0] = 0;
	Date[0] = 0;

	if(argc >= 5)
	{
		upperize(argv[4]);
		SaveCopy(Date, strlen(argv[4]) + 1, argv[4], 0);
		GenerateSelList(argc, argv, PortList, PortSize, 5);
		argc = 4;	
	}

	return ProcessInputInfoBase(argc, argv);
}

int ProcessInputInfo2(int argc, char* argv[], char *PortList, int PortSize, char *Date, char *Parm1)
// Support C:\Program user password dbstring date parm portfolio1 portfolio2 ...
{
	PortList[0] = 0;
	Date[0] = 0;
	Parm1[0] = 0;

	if(argc >= 6)
	{
		upperize(argv[4]);
		SaveCopy(Date, strlen(argv[4]) + 1, argv[4], 0);
		upperize(argv[5]);
		SaveCopy(Parm1, strlen(argv[5]) + 1, argv[5], 0);
		GenerateSelList(argc, argv, PortList, PortSize, 6);
		argc = 4;
	}
	else
		if(argc == 5)
			return ProcessInputInfo1(argc, argv, PortList, PortSize, Date);
	
	return ProcessInputInfoBase(argc, argv);
}

int ProcessInputInfo3(int argc, char* argv[], char *PortList, int PortSize, char *Date, char *Parm1, char *Parm2)
// Support C:\Program user password dbstring date parm1 parm2 portfolio1 portfolio2 ...
{
	PortList[0] = 0;
	Date[0] = 0;
	Parm1[0] = 0;
	Parm2[0] = 0;

	if(argc >= 7)
	{
		upperize(argv[4]);
		SaveCopy(Date, strlen(argv[4]) + 1, argv[4], 0);
		upperize(argv[5]);
		SaveCopy(Parm1, strlen(argv[5]) + 1, argv[5], 0);
		upperize(argv[6]);
		SaveCopy(Parm2, strlen(argv[6]) + 1, argv[6], 0);
		GenerateSelList(argc, argv, PortList, PortSize, 7);
		argc = 4;
	}
	else
		if(argc == 6)
			return ProcessInputInfo2(argc, argv, PortList, PortSize, Date, Parm1);
	
	return ProcessInputInfoBase(argc, argv);
}

const char *SaveCopy(char *DestStr, unsigned int DestStrSize, const char *SrcStr, int FailBlank)
{
	if(!DestStr || DestStrSize == 0)
		return DestStr;

	DestStr[0] = 0;

	if(!SrcStr || strlen(SrcStr) == 0)
		return DestStr;

	if(DestStrSize > strlen(SrcStr))
		strcpy_s(DestStr, DestStrSize, SrcStr);
	else
		if(!FailBlank && DestStrSize >= 1)
		{
//			printf("   SaveCopy: Len:%d Size:%d  %s\n", strlen(SrcStr), DestStrSize, SrcStr);
			strncpy_s(DestStr, DestStrSize, SrcStr, DestStrSize - 1);
			DestStr[DestStrSize - 1] = 0;
		}

	return DestStr;
}

const char *SaveConcatenate(char *DestStr, unsigned int DestStrSize, const char *SrcStr)
{
	if(!DestStr || DestStrSize == 0 || strlen(DestStr) == DestStrSize - 1)
		return DestStr;

	if(!SrcStr || strlen(SrcStr) == 0)
		return DestStr;

	if(DestStrSize - strlen(DestStr) > strlen(SrcStr))
		strcat_s(DestStr, DestStrSize, SrcStr);
	else
		if(DestStrSize >= 1)
		{
//			printf("   SaveConcatenate: Len:%d Size:%d  %s   %s\n", strlen(SrcStr), DestStrSize - strlen(DestStr), DestStr, SrcStr);
			strncat_s(DestStr, DestStrSize, SrcStr, DestStrSize - strlen(DestStr) - 1);
			DestStr[DestStrSize - 1] = 0;
		}

	return DestStr;
}


/*
 * Read at most n-1 characters from the given file.
 * Replace ascii 0 with blank.
 * Stop when a newline has been read, or the count runs out.
 * Return first argument, or NULL if no characters were read.
 */
char *fgetline(char *buf, int max, FILE *fp)
{
	int c, i = 0;
	char *p;

	/* get max bytes or upto a newline */

	for (p = buf, max--; max > 0; max--) {
		if ((c = fgetc (fp)) == EOF)
			break;

		if(c == 0) // replace ascii 0 with blank
			c = ' ';
		else
			if(c == '\t')
				c = ',';

		*p++ = c;
		i++;

		if (c == '\n')
			break;
	}
	*p = 0;

	if (p == buf || c == EOF)
		return NULL;

	return (p);
}

__int64 FileSize64(const char * szFileName) 
{ 
  struct __stat64 fileStat; 
  int err = _stat64(szFileName, &fileStat); 
  if (0 != err) return 0;

  return fileStat.st_size; 
}