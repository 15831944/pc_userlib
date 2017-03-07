#ifndef __CSVUTIL_H__
#define __CSVUTIL_H__

typedef struct {
	char Data[101];
} CSVFieldType;

typedef struct {
	char Header[101];
	int Idx;
} CSVHeaderType;

typedef struct {
	int nAction;
	int nPreRowCount;
	int nRowCount;
	int nFieldCount;
	FILE *fp;
	char Header[101];
	char Tail[101];
	CSVHeaderType CSVHeaders[500];
	CSVFieldType CSVData[500];
} CSVDataType;

void CSVAssignHeader(CSVDataType *pCSVData, const char *Header);
int CSVSetup(CSVDataType *pCSVData, const char *FileNmae, const char *Header, const char *Tail, int nAction);
int CSVSetup2(CSVDataType *pCSVData, const char *FileName, const char *EnvDir, const char *Header, const char *Tail, int nPreHeaderRows, int nAction);
int CSVSetup3(CSVDataType *pCSVData, const char *FullFileName, const char *Header, const char *Tail, int nPreHeaderRows, int nAction);
int CSVReadHeaders(CSVDataType *pCSVData);
int CSVPreHeader(CSVDataType *pCSVData);
int CSVReadData(CSVDataType *pCSVData);
int STPTagReadData(CSVDataType *pCSVData);
int STPQuickProcessTagData(CSVDataType *pCSVData, char *DataLine);
int CSVGetHeaderIndex(CSVDataType *pCSVData, int nIdx, const char *Data);
int CSVGetData(CSVDataType *pCSVData, int nIdx, const char *Data);
int CSVClose(CSVDataType *pCSVData);
int CSVGetNumber(CSVDataType *pCSVData, char *DestNum, int DestNumSize, const char *SrcNum);
void CSVDP(char *Data);
int CSVWriteHeader(CSVDataType *pCSVData);
int CSVWritePreHeader(CSVDataType *pCSVData);
int CSVWriteData(CSVDataType *pCSVData);
int CSVWriteLine(CSVDataType *pCSVData);
void CSVWriteTail(CSVDataType *pCSVData);

#endif // __CSVUtil_H__