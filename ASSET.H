#ifndef __ASSET_H__
#define __ASSET_H__

typedef struct
{
   char m_RateBasis[9];
   char m_Date[12];
   char m_EndDate[12];
   int  m_nMonthBase;
   int  m_nYearBase;
   int  m_nYearBase2;
   int  m_nDay;
   int  m_nIntDays;
   int  m_nIntDays2;
   int  m_nMonth;
   int  m_nYear;
   int  m_nAADays;
} DateType;
   
typedef struct
{
   char   m_FromDate[12];
   char   m_ToDate[12];
   double m_AmortFact;
   double m_Rate;
   int    m_nAADays;
   int  m_nInclusive;
} InterestType;

typedef struct
{
   double       m_NomAmount;
   double       m_BondFact;
   double       m_Price;
   double       m_FxRate;
   double       m_MktPrice;
   double       m_MktFxRate;
   int          m_nAADays;
   int          m_IntArrCount;
   InterestType m_IntArr[20];
   int          m_bAccruable;
   int          m_bPrePaid;
   int          m_AccruedDays;
   char         m_ValueDate[12];
   char			m_EndDate[12];
   char         m_CapitStart[12];
   char         m_CapitEnd[12];
   double       m_AmortFact;
   double       m_CapitRate;
   double       m_PlusAmount;
   char         m_TransType[11];
   char         m_RateBasis[9];
} AssetType;
/*************************************************
   Class type of C functions.
*************************************************/
void FmsDateSetup(DateType* FmsDate, const char* Date, const char *EndDate, 
                  const char* RateBasis, int AAdays);
void FmsDateGetBasis(DateType* FmsDate, const char* RateBasis);
int  FmsDateGetDayMonthYear(DateType* FmsDate, const char* Date);
int  FmsDateIsLeapYear(DateType* FmsDate);
const char* FmsDateGetDate(DateType* FmsDate);
const char* FmsDateGetRateBasis(DateType* FmsDate);
int FmsDateGetYearBase(DateType* FmsDate);
int FmsDateGetMonthBase(DateType* FmsDate);
int FmsDateCompareDate(DateType* FmsDate, const char* Date);
int FmsDateGetDays(DateType* FmsDate, const char* Date);
int FmsDateLastDayofMonth(DateType* FmsDate);

void AssetSetup(AssetType* Asset, double NomAmount, double Price, 
                const char* ValueDate, const char *EndDate, const char* RateBasis, double FxRate, 
                double MktPrice, double MktFxRate, double BondFact,
                int AADays, int Accruable, int PrePaid, const char* TransType);
void AssetResetInterest(AssetType* Asset); 
void AssetSetupInterest(AssetType* Asset, const char* FromDate, const char* ToDate, 
						double IntRate, double AmortFact, int AADays);
void AssetSetupInterest2(AssetType* Asset, const char* FromDate, const char* ToDate, 
						double IntRate, double AmortFact, int AADays, int nInclusive);
void AssetSetupCapit(AssetType* Asset, const char* CapitStart, const char* CapitEnd, 
					 double AmortFact, double CapitRate);
void AssetSetupPlusAmount(AssetType* Asset, double PlusAmount);
int AssetGetAccruedDays(AssetType* Asset);
double AssetGetFaceAmount(AssetType* Asset);
double AssetGetAccretionAmount(AssetType* Asset);
double AssetGetPrePaidAccretionAmount(AssetType* Asset);
double AssetGetPrincipleCash(AssetType* Asset);
double AssetGetAccretionCash(AssetType* Asset);
double AssetGetPrePaidAccretionCash(AssetType* Asset);
double AssetGetInterest(AssetType* Asset);
double AssetGetPrePaidInterest(AssetType* Asset);
double AssetGetMarketValue(AssetType* Asset);
double AssetGetAccretionMarketValue(AssetType* Asset);
double AssetGetLeverageExpense(double Amount, double IntRate, const char* FromDate, 
							   const char* ToDate, const char *EndDate, const char* RateBasis);
double AssetComputeInterest(double NomAmount, double Rate, int Days, int Base, 
			double BondFact, double AmortFact, double Plus, const char *RateBasis);
double AssetComputeInterestWO(double NomAmount, double Rate, int Days, int Base, 
			double BondFact, double AmortFact, double Plus, const char *RateBasis);
double AssetComputeInterest2(double NomAmount, double Rate, const char *IntFrom, 
		const char *IntTo, const char *EndDate, double BondFact, double AmortFact, 
		double Plus, const char* RateBasis, int AADays, int *Days);
#endif
