#ifndef __FLOATREPO_H__
#define __FLOATREPO_H__

typedef struct {
	char Portfolio[11];
	char Custodian[36];
	char EuroClear[6];
	char Account[11];
	char Account2[51];
	char AccountID[21];
	char Bank[31];
	char CustodianAccount[21];
	char CustodianCode[21];
	char Margin[2];
	char Contact[31];
	char Fax[16];
	char Phone[31];
} PortfolioAccountType;

typedef struct {
	char PB[21];
	char CP[21];
	double LowLimit;
	double HighLimit;
	double OrFee;
	double OCCFee;
} OrFeeDataType;

OrFeeDataType OrFeeDataArray[50];
int OrFeeDataArraySize;

char cf_date[11];

void GetSecFee(double *Sec_Fee);
int FindOrFeeData(const char *PB, const char *CP, double Contracts);
void LoadOrFeeArray();
double ComputeSecFee(int bStock, char dir, double price, double nominal, double sec_fee);
double ComputeOrFee(const char *PB, const char *CP, double Contracts);
void SetDateFormat();
int CompareCFDate(char *Date, int status);
void SetCFDate();
int ComputeInterest(double amt, double rate, const char *from_date, const char *to_date, 
					const char *enddate, const char *ratebasis, int AADays, double *interest);
int ComputeInterest2(const char *AssetCode, double amt, double rate, const char *from_date, 
					const char *to_date, const char *enddate, const char *ratebasis, int AADays, 
					double *interest);
const char *date_stamp(char *datestamp, int date_size);
int FloatRepoAccrual(double repo_amt, const char *portfolio, long trans_num, long inv_num, 
					const char *from_date, const char *to_date,  double *interest, double *rate);
int FloatRepoInterest(double repo_amt, const char *portfolio, long trans_num, long inv_num, 
					  double *interest, double *rate);
void SqlError(const char *Function, int bExit);
void set_cf_date();
int compare_date(char* date1, char* date2);
void get_day_month_year(char* date, int* day, int* month, int* year);
int GetAB_252Days(const char *FromDate, const char *ToDate);
int GetAAAYearBase(const char *FromDate);
int GetDaysInYear(const char *EndDate);
int GetAARYearBase(const char *Asset, const char *FromDate, int Year);
int UpdateXCurrencyTradeFxrate(const char *Date);
double GetTradeFxrate(const char *Currency, const char *Date);
double GetOptionFxrate(long OptTransNum, const char *Currency, const char *Date);
int GetPortfolioAccountList(PortfolioAccountType *PortAcctArray, int *PortAcctCount);
int FindPortfolioAccountID(const char *Portfolio, const char *Account, 
						   PortfolioAccountType *PortAcctArray, 
							int PortAcctCount, char *AccountID);
void UpdateOptionPosition(const char *Date);
void UpdateFullPosition(const char *Date);
void UpdateFullPosition2(const char *FromDate, const char *Date);
double ComputeBondPrice(double Rate, double Yield, double YearToMaturity, int Freq);
int GetTPlusNDate(char *Date, int DateSize, const char *SrcDate, int PlusDays, const char *Currency);
double YTMYield(const char *Asset, const char *Date, double Price);
double YTMYield2(double Price, double Rate, double YearToMaturity, int Freq);
#endif
