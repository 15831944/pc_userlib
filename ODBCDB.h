#ifndef __ODBCDB_H__
#define __ODBCDB_H__

#include<afxdb.h>

typedef struct {
	char DSN[500];
	int BlankLimit;
} ODBCDataType;

const char *ODBC_GetDSN(ODBCDataType &ODBCData);
int ODBC_GetBlankLimit(ODBCDataType &ODBCData);
int ODBC_OpenExcel(ODBCDataType &ODBCData, const char *FileName, int bExclusive, int bReadOnly, int bUserCursorLib);
int ODBC_OpenOracle(ODBCDataType &ODBCData, const char *UserName, const char *Password, int bExclusive, int bReadOnly, int bUserCursorLib);
int ODBC_OpenExExcel(ODBCDataType &ODBCData, const char *FileName, long dwOptions);
int ODBC_OpenExOracle(ODBCDataType &ODBCData, const char *UserName, const char *Password, long dwOptions);

int ODBC_IsDataNull(CDBVariant Var);
const char *ODBC_GetDBString(CDBVariant Var);
double ODBC_GetDBDouble(CDBVariant Var);
int ODBC_GetDBInt(CDBVariant Var);
#endif // __ODBCDB_H__