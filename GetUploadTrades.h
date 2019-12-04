#ifndef _GET_UPLOAD_TRADES
/******************************************
GetUploadTrades.h  Header File

*******************************************/
void setupTradeUploadFundSql(const char *portlist, int bIn, char *FundSql, int nSqlSize);
void getAdminUploadTrades(const char* date, const char*countparty, const char *portlist, int bIn);
void getDateTimeStamp();
void getTodaysTrades(const char *date, int secs);
void getUploadMultiFundTrades(const char* date, const char* counterparty, const char *portlist, int bIn);
void markUploaded(const char*);
void markUploadedbyTransNum(int TransNum);


#define _GET_UPLOAD_TRADES
#endif
