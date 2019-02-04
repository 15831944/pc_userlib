#ifndef __USERINFO_H__
#define __USERINFO_H__

#include <stdio.h>

char *GetFileName(char *FileName, int s_Size, const char *EnvVar, const char *FName);
char *fgetline(char *buf, int max, FILE *fp);
FILE *OpenOutputFile(const char *FName, const char *EnvVar);
FILE *OpenInputFile(const char *FName, const char *EnvVar);
void write_userinfo(const char* username, const char* password, const char *db);
int read_userinfo(char* username, char* password, char *db);
int IsDateValid(const char* Date);
void GenerateSelList(int Max, char *AvailList[], char *SelList, int sSize, int Start);
void MMDDYYYYToDDMONYY(const char *SrcDate, char *Date);
void MMDDYYYYToDDMONYYYY(const char *SrcDate, char *Date);
const char *SaveCopy(char *DestStr, unsigned int DestStrSize, const char *SrcStr, int FailBlank);
const char *SaveConcatenate(char *DestStr, unsigned int DestStrSize, const char *SrcStr);
int ProcessInputInfoBase(int argc, char *argv[]);
int ProcessInputInfo(int argc, char *argv[], char *PortList, int PortSize);
int ProcessInputInfo1(int argc, char *argv[], char *PortList, int PortSize, char *Date);
int ProcessInputInfo2(int argc, char *argv[], char *PortList, int PortSize, char *Date, char *Parm1);
int ProcessInputInfo3(int argc, char *argv[], char *PortList, int PortSize, char *Date, char *Parm1, char *Parm2);
__int64 FileSize64(const char * szFileName);
#endif