#ifndef _GET_UPLOAD_TRADES
/******************************************
GetUploadTrades.h  Header File

*******************************************/
void getAdminUploadTrades(const char* date, const char*countparty, const char *portlist, int bIn);
void getUploadMultiFundTrades(const char* date, const char* counterparty, const char *portlist, int bIn);
void getUploadTrades(const char*, const char*);
void markUploaded(const char*);
void markUploadedbyTransNum(int TransNum);

#define _GET_UPLOAD_TRADES
#endif
