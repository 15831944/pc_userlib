#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "userinfo.h"
#include "gen_fun.h"
#include "csvutil.h"

int CSVRead(CSVDataType *pCSVData, int bHeader);
int CSVProcessData(CSVDataType *pCSVData, char *DataLine, int bHeader);
int STPProcessTagData(CSVDataType *pCSVData, char *DataLine);
int CSVWrite(CSVDataType *pCSVData, const char *Data, int nFieldNo);
void CSVInitHeaders(CSVDataType *pCSVData);
void CSVInitData(CSVDataType *pCSVData);

const int CSV_READ = 1,
		  CSV_WRITE = 3;


void CSVAssignHeader(CSVDataType *pCSVData, const char *Header)
{
	SaveCopy(pCSVData->CSVHeaders[pCSVData->nFieldCount].Header, sizeof(pCSVData->CSVHeaders[pCSVData->nFieldCount].Header), Header, 0);
	pCSVData->CSVHeaders[pCSVData->nFieldCount].Idx = -1;
	pCSVData->nFieldCount ++;
}

int CSVSetup(CSVDataType *pCSVData, const char *FileName, const char *HeaderString, const char *TailString, int nAction)
{
	pCSVData->nAction = nAction;
	pCSVData->nPreRowCount = 0;
	pCSVData->nRowCount = 0;
	pCSVData->nFieldCount = 0;

	if(HeaderString && strlen(HeaderString) > 0)
		SaveCopy(pCSVData->Header, sizeof(pCSVData->Header), HeaderString, 0);
	else
		pCSVData->Header[0] = 0;

	if(TailString && strlen(TailString) > 0)
		SaveCopy(pCSVData->Tail, sizeof(pCSVData->Tail), TailString, 0);
	else
		pCSVData->Tail[0] = 0;

	if(!FileName || strlen(FileName) == 0)
		return 0;

	pCSVData->fp = 0;

	if(pCSVData->nAction == CSV_READ)
		pCSVData->fp = OpenInputFile(FileName, "FMS_DATA");
	else
		if(pCSVData->nAction == CSV_WRITE)
			pCSVData->fp = OpenOutputFile(FileName, "FMS_DATA");

	return !pCSVData->fp ? 0 : 1;
}

int CSVSetup2(CSVDataType *pCSVData, const char *FileName, const char *EnvDir, const char *HeaderString, const char *TailString, int nPreHeaderRows, int nAction)
{
	pCSVData->nAction = nAction;
	pCSVData->nPreRowCount = nPreHeaderRows;
	pCSVData->nRowCount = 0;
	pCSVData->nFieldCount = 0;

	if(HeaderString && strlen(HeaderString) > 0)
		SaveCopy(pCSVData->Header, sizeof(pCSVData->Header), HeaderString, 0);
	else
		pCSVData->Header[0] = 0;

	if(TailString && strlen(TailString) > 0)
		SaveCopy(pCSVData->Tail, sizeof(pCSVData->Tail), TailString, 0);
	else
		pCSVData->Tail[0] = 0;

	if(!FileName || strlen(FileName) == 0)
		return 0;
	
	pCSVData->fp = 0;
	
	if(pCSVData->nAction == CSV_READ)
		pCSVData->fp = OpenInputFile(FileName, EnvDir);
	else
		if(pCSVData->nAction == CSV_WRITE)
			pCSVData->fp = OpenOutputFile(FileName, EnvDir);

	return !pCSVData->fp ? 0 : 1;
}

int CSVSetup3(CSVDataType *pCSVData, const char *FullFileName, const char *HeaderString, const char *TailString, int nPreHeaderRows, int nAction)
{
	pCSVData->nAction = nAction;
	pCSVData->nPreRowCount = nPreHeaderRows;
	pCSVData->nRowCount = 0;
	pCSVData->nFieldCount = 0;

	if(HeaderString && strlen(HeaderString) > 0)
		SaveCopy(pCSVData->Header, sizeof(pCSVData->Header), HeaderString, 0);
	else
		pCSVData->Header[0] = 0;

	if(TailString && strlen(TailString) > 0)
		SaveCopy(pCSVData->Tail, sizeof(pCSVData->Tail), TailString, 0);
	else
		pCSVData->Tail[0] = 0;

	if(!FullFileName || strlen(FullFileName) == 0)
		return 0;
	
	pCSVData->fp = 0;
	
	if(pCSVData->nAction == CSV_READ)
		fopen_s(&pCSVData->fp, FullFileName, "r");
	else
		if(pCSVData->nAction == CSV_WRITE)
			fopen_s(&pCSVData->fp, FullFileName, "w+");

	return !pCSVData->fp ? 0 : 1;
}

int CSVPreHeader(CSVDataType *pCSVData)
{
	char DataLine[10000];
	int i;

	if(strlen(pCSVData->Header) <= 0 && pCSVData->nPreRowCount <= 0)
		return 0;

	if(strlen(pCSVData->Header) > 0)
	{
		for(;;)
		{
			fgetline(DataLine, sizeof(DataLine), pCSVData->fp);
			CSVDP(DataLine);
			if(strstr(DataLine, pCSVData->Header))
				return 1;
		}
	}
	else
	{
		for(i = 0; i < pCSVData->nPreRowCount; i++)
		{
			fgetline(DataLine, sizeof(DataLine), pCSVData->fp);
		}

		return 1;
	}

	return 0;
}

int CSVReadHeaders(CSVDataType *pCSVData)
{
	if(!pCSVData->nAction == CSV_READ)
		return 0;

	CSVPreHeader(pCSVData);

	return CSVRead(pCSVData, 1);
}

int CSVReadData(CSVDataType *pCSVData)
{
	if(!pCSVData->nAction == CSV_READ)
		return 0;

	return CSVRead(pCSVData, 0);
}

void CSVDP(char *Data)
{
	char *p;

	p = strstr(Data, "\r\n");
	if(p)
		p[0] = 0;
	p = strstr(Data, "\n");  // new line
	if(p)
		p[0] = 0;
	p = strstr(Data, "\r");  // Caridge return
	if(p)
		p[0] = 0;
	p = strstr(Data, "\v");  // Vertical Tab
	if(p)
		p[0] = 0;
	p = strstr(Data, "\t"); // Tab
	if(p)
		p[0] = 0;
	upperize(Data);
	if(Data[0] == '\"') // Double quote
		strcpy_s(Data, strlen(Data), Data + 1);
	if(strlen(Data) > 0 && Data[strlen(Data) - 1] == '\"')
		Data[strlen(Data) - 1] = 0;
}

int CSVGetHeaderIndex(CSVDataType *pCSVData, int nIdx, const char *Data)
{
	int i;

	for(i = 0; i < pCSVData->nFieldCount; i++)
	{
		if(strcmp(pCSVData->CSVHeaders[i].Header, Data) == 0)
			pCSVData->CSVHeaders[i].Idx = nIdx;
	}

	return 0;
}

int CSVGetData(CSVDataType *pCSVData, int nIdx, const char *Data)
{
	int i;

	for(i = 0; i < pCSVData->nFieldCount; i++)
	{
		if(pCSVData->CSVHeaders[i].Idx == nIdx)
		{
			SaveCopy(pCSVData->CSVData[i].Data, sizeof(pCSVData->CSVData[i].Data), Data, 0);
			return 1;
		}
	}

	return 0;
}

int CSVRead(CSVDataType *pCSVData, int bHeader)
{
	char DataLine[10001];

	if(!fgetline(DataLine, sizeof(DataLine), pCSVData->fp)) 
		return 0;

	CSVDP(DataLine);

	if(strlen(pCSVData->Tail) > 0 && strstr(DataLine, pCSVData->Tail)) // End of file
		return 0;

	return CSVProcessData(pCSVData, DataLine, bHeader);
}

int STPTagReadData(CSVDataType *pCSVData)
{
	int i;
	char *p, DataLine[10001], DataBuf[10001], Tag[21];

	if(!fgetline(DataLine, sizeof(DataLine), pCSVData->fp))
		return 0;

	CSVDP(DataLine);

	for(i = 0; i < pCSVData->nFieldCount; i++)
	{
		SaveCopy(Tag, sizeof(Tag), pCSVData->CSVHeaders[i].Header, 0);
		strcat_s(Tag, sizeof(Tag), "=");

		p = strstr(DataLine, Tag);
		while(p)
		{
			SaveCopy(DataBuf, sizeof(DataBuf), p + strlen(Tag), 0);
			p[0] = 0;
			
			if(strlen(DataLine) > 0)
				strcat_s(DataLine, sizeof(DataLine), ",");

			strcat_s(DataLine, sizeof(DataLine), pCSVData->CSVHeaders[i].Header);
			strcat_s(DataLine, sizeof(DataLine), "|");

			strcat_s(DataLine, sizeof(DataLine), DataBuf);

			p = strstr(DataLine, Tag);
		}
	}

	STPProcessTagData(pCSVData, DataLine);

	return 1;
}

int CSVProcessData(CSVDataType *pCSVData, char *DataLine, int bHeader)
{
	int i, j = 0, k = 0, QuoteCnt = 0;
	char DataChar, Data[201];

	if(!bHeader)
	{
		for(i = 0; i < pCSVData->nFieldCount; i ++)
			pCSVData->CSVData[i].Data[0] = 0;

		pCSVData->nRowCount++;
	}

	for(i = 0; i < (int) strlen(DataLine); i++)
	{
		DataChar = DataLine[i];
		switch(DataChar)
		{
			case ',':
				if(QuoteCnt % 2 == 1)
				{
					Data[j] = DataChar;
					j++;
				}
				else
				{
					Data[j] = 0;

					if(bHeader)
						CSVGetHeaderIndex(pCSVData, k, Data);
					else
						CSVGetData(pCSVData, k, Data);

					k++;
					Data[0] = 0;
					QuoteCnt = 0;
					j = 0;
				}
				break;
			case '"':
				QuoteCnt ++;
				break;
			default:
				if(isprint(DataChar))
				{
					Data[j] = DataChar;
					j++;
				}
				else
					Data[j] = 0;
				break;
		}
	}

	Data[j] = 0;
	if(bHeader)
		CSVGetHeaderIndex(pCSVData, k, Data);
	else
		CSVGetData(pCSVData, k, Data);

	return 1;
}

int STPProcessTagData(CSVDataType *pCSVData, char *DataLine)
{
	char *p, *q, *r, Tag[21];
	int i, j;

	for(i = 0; i < pCSVData->nFieldCount; i++)
		pCSVData->CSVData[i].Data[0] = 0;

	p = strtok_s(DataLine, ",", &q);
	while(p)
	{
		for(i = 0; i < pCSVData->nFieldCount; i ++)
		{
			SaveCopy(Tag, sizeof(Tag), pCSVData->CSVHeaders[i].Header, 0);
			strcat_s(Tag, sizeof(Tag), "|");
			r = strstr(p, Tag);
			if(r)
			{
				pCSVData->CSVHeaders[i].Idx = i;
				SaveCopy(pCSVData->CSVData[i].Data, sizeof(pCSVData->CSVData[i].Data), r + strlen(Tag), 0);
				for(j = 0; j < (int) strlen(pCSVData->CSVData[i].Data); j++)
					if(pCSVData->CSVData[i].Data[j] == '^')
						pCSVData->CSVData[i].Data[j] = ' ';
			}
		}
		
		p = strtok_s(NULL, ",", &q);
	}

	return 1;
}

int STPQuickProcessTagData(CSVDataType *pCSVData, char *DataLine)
{
	char *p, *q;
	int i;

	for(i = 0; i < pCSVData->nFieldCount; i++)
		pCSVData->CSVData[i].Data[0] = 0;

	p = strtok_s(DataLine, "|", &q);
	while(p)
	{
		for(i = 0; i < pCSVData->nFieldCount; i ++)
		{
			if(strncmp(p, pCSVData->CSVHeaders[i].Header, strlen(pCSVData->CSVHeaders[i].Header)) == 0)
				SaveCopy(pCSVData->CSVData[i].Data, sizeof(pCSVData->CSVData[i].Data), p + strlen(pCSVData->CSVHeaders[i].Header), 0);
		}

		p = strtok_s(NULL, "|", &q);
	}

	return 1;
}

int CSVWritePreHeader(CSVDataType *pCSVData)
{
	int i;

	if(strlen(pCSVData->Header) <= 0 && pCSVData->nPreRowCount <= 0)
		return 0;

	if(strlen(pCSVData->Header) > 0)
		fprintf(pCSVData->fp, "%s\n", pCSVData->Header);
	else
		for(i = 0; i < pCSVData->nPreRowCount; i++)
			fprintf(pCSVData->fp, "%c\n", 'A' + i);
	
	return 1;
}

int CSVWriteHeader(CSVDataType *pCSVData)
{
	int i;

	if(pCSVData->nAction == CSV_WRITE)
		return 0;
	
	CSVWritePreHeader(pCSVData);
		
	for(i = 0; i < pCSVData->nFieldCount; i++)
		CSVWrite(pCSVData, pCSVData->CSVHeaders[i].Header, i);
	CSVWriteLine(pCSVData);

	return 1;
}

int CSVWriteData(CSVDataType *pCSVData)
{
	int i;

	if(pCSVData->nAction == CSV_WRITE)
		return 0;

	for(i = 0; i < pCSVData->nFieldCount; i++)
		return CSVWrite(pCSVData, pCSVData->CSVData[i].Data, i);
	CSVWriteLine(pCSVData);

	return 1;
}

int CSVWrite(CSVDataType *pCSVData, const char *Data, int nFieldNo)
{
	int bWithQuote;

	bWithQuote = strstr(Data, ",") ? 1 : 0;
	
	if(pCSVData->nAction == CSV_WRITE && nFieldNo > 0)
		fprintf(pCSVData->fp, ",");

	if(bWithQuote)
		fprintf(pCSVData->fp, "\"%s\"", Data);
	else
		fprintf(pCSVData->fp, "%s", Data);

	return 1;
}

int CSVWriteLine(CSVDataType *pCSVData)
{
	fprintf(pCSVData->fp, "\n");
	
	return 1;
}

int CSVClose(CSVDataType *pCSVData)
{
	fclose(pCSVData->fp);

	return 1;
}

void CSVWriteTail(CSVDataType *pCSVData)
{
	if(strlen(pCSVData->Tail) > 0)
		fprintf(pCSVData->fp, "%s\n", pCSVData->Tail);
}

int CSVGetNumber(CSVDataType *pCSVData, char *DestNum, int DestNumSize, const char *SrcNum)
{
	char Data, Buffer[1024];
	int i, j, Len = 0, QuoteCnt = 0;

	DestNum[0] = 0;
	Buffer[0] = 0;

	if(!SrcNum)
		return 0;

	Len = strlen(SrcNum);
	if(Len <= 0 || Len >= 1024)
		return 0;

	j = 0;
	for(i = 0; i < Len; i++)
	{
		Data = SrcNum[i];
		if(Data == '(' || Data == ')')
			QuoteCnt ++;
		else
			if(strchr("-0123456789.", Data))
			{
				Buffer[j] = Data;
				j++;
			}
	}

	Buffer[j] = 0;
	if(strlen(Buffer) > 0 && QuoteCnt > 0)
		SaveCopy(DestNum, DestNumSize, "-", 0);

	SaveConcatenate(DestNum, DestNumSize, Buffer);

	return 1;
}
