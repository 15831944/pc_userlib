#ifndef __FTPUTIL_H__
#define __FTPTUIL_H__

typedef struct {
	char DataName[100];
	char GPGName[100];
} FTPFileType;

typedef struct {
	char CPName[21];
	char Site[61];
	char User[21];
	char Pass[21];
	char Dir[61];
	char ScriptName[100];
	char UtilName[100];
	char BatchName[100];
	FTPFileType FList[50];
	char LocalDir[100];
	char EncryptKey[31];
	int Port;
	int FListSize;
	FILE *fp;
} FTPDataType;

extern FTPDataType FTPData;

int FTPSetup(FTPDataType *pFTPData, const char *CPName, const char *LocalDir);
int FTPAddDataFile(FTPDataType *pFTPData, const char *DataName);
int FTPRun(FTPDataType *pFTPData, int Upload);
int FTPEncrypt(FTPDataType *pFTPData);
int FTPScript(FTPDataType *pFTPData, const char *ScriptName, const char *BatchName, int Upload);

#endif