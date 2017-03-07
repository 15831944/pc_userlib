#include <stdio.h>
#include <stdlib.h>
#include "odbcdb.h"

const char *ODBC_GetOracleDSN(ODBCDataType &ODBCData, const char *UserName, const char *Password);
const char *ODBC_GetExcelDSN(ODBCDataType &ODBCData, const char *FileName);

const char *ODBC_GetOracleDSN(ODBCDataType &ODBCData, const char *UserName, const char *Password)
{
	sprintf_s(ODBCData.DSN, sizeof(ODBCData.DSN), "ODBC;DSN=NWIDB;UID=%s;PWD=%s;DBQ=NWIDB;DBA=W;");
	return ODBCData.DSN;
}

const char *ODBC_GetExcelDSN(ODBCDataType &ODBCData, const char *FileName)
{
	sprintf_s(ODBCData.DSN, sizeof(ODBCData.DSN), "ODBC;DSN=Excel Files;DBQ=%s;DriverID=1046;", FileName);
	return ODBCData.DSN;
}

int ODBC_OpenExcel(ODBCDataType &ODBCData, const char *FileName, BOOL bExclusive, BOOL bReadOnly, BOOL bUserCursorLib)
{
	if(FileName.IsEmpty())
		return FALSE;

	GetExcelDSN(FileName);

	return Open(bExclusive, bReadOnly, bUserCursorLib);
}

int ODBC_OpenOracle(ODBCDataType &ODBCData, const char *UserName, const char *Password, BOOL bExclusive, BOOL bReadOnly, BOOL bUserCursorLib)
{
	if(UserName.IsEmpty() || Password.IsEmpty())
		return FALSE;

	GetOracleDSN(UserName, Password);

	return Open(bExclusive, bReadOnly, bUserCursorLib);
}

int ODBC_OpenExExcel(ODBCDataType &ODBCData, const char *FileName, long dwOptions)
{
	if(FileName.IsEmpty())
		return FALSE;

	GetExcelDSN(FileName);

	return OpenEx(dwOptions);
}

int ODBC_OpenExOracle(ODBCDataType &ODBCData, const char *UserName, const char *Password, long dwOptions)
{
	if(UserName.IsEmpty() || Password.IsEmpty())
		return FALSE;

	GetOracleDSN(UserName, Password);

	return OpenEx(dwOptions);
}

int ODBC_IsDataNull(CDBVariant Var)
{
	return Var.m_dwType == DBVT_NULL ? 1 : 0;
}

const char *ODBC_GetDBString(CDBVariant Var)
{
	char Data[2049];
	TIMESTAMP_STRUCT *pTMData;

	switch(Var.m_dwType)
	{
		case DBVT_NULL:
			break;
		case DBVT_BOOL:
			strcpy_s(Data, sizeof(Data), Var.m_boolVal == 1 ? "TRUE" : "FALSE");
			break;
		case DBVT_UCHAR:
			Data[0] (char) Var.m_chVal;
			Data[1] = 0;
			break;
		case DBVT_SHORT:
			strcpy_s(Data, sizeof(Data), "%d", Var.m_iVal);
			break;
		case DBVT_LONG:
			strcpy_s(Data, sizeof(Data), "%ld", Var.m_lVal);
			break;
		case DBVT_SINGLE:
			strcpy_s(Data, sizeof(Data), "%f", Var.m_fltVal);
			break;
		case DBVT_DOUBLE:
			strcpy_s(Data, sizeof(Data), "%lf", Var.m_dblVal);
			break;
		case DBVT_DATE:
			pTMData = Var.m_pdate;
			strcpy_s(Data, sizeof(Data), "%02d%02d%04d", pTMData->month, pTMData->day, pTMData->year);
			break;
		case DBVT_STRING:
			strcpy_s(Data, sizeof(Data), Var.m_pstring->GetBuffer();
			break;
		case DBVT_BINARY:
//			pData = Var.m_pbinary;
//			Data = pData->GetBuffer();
			break;
		case DBVT_ASTRING:
			pData = Var.m_pstringA;
			strcpy_s(Data, sizeof(Data), Var.m_pstringA->GetBuffer());
			break;
		case DBVT_WSTRING:
//			pData = Var.m_pstringW;
//			Data = pData->GetBuffer();
			break;
		default:
			break;
	}

	return Data;
}

double ODBC_GetDBDouble(CDBVariant Var)
{
	double Data = 0;

	switch(Var.m_dwType)
	{
		case DBVT_SHORT:
			Data = (double) Var.m_iVal;
			break;
		case DBVT_LONG:
			Data = (double) Var.m_lVal;
			break;
		case DBVT_SINGLE:
			Data = Var.m_fltVal;
			break;
		case DBVT_DOUBLE:
			Data = Var.m_dblVal;
			break;
		default:
			break;
	}
	
	return Data;
}

int ODBC_GetDBInt(CDBVariant Var)
{
	int Data = 0;

	switch(Var.m_dwType)
	{
		case DBVT_NULL:
			break;
		case DBVT_BOOL:
			Data = Var.m_boolVal;
			break;
		case DBVT_SHORT:
			Data = Var.m_iVal;
			break;
		case DBVT_LONG:
			Data = Var.m_lVal;
			break;
		default:
			break;
	}

	return Data;
}

