#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "asset.h"
#include "gen_fun.h"
#include "floatrepo.h"
#include "USERINFO.H"

/*******************************
   Last Modified: 15-JUL-99
   LX 02-MAR-2000  Fixed a bug in days computation.
   LX 21-MAR-2000  Allow Equity Type to Accrual.
   LX 25-JAN-2002  Support DD-MON-YYYY and DD-MON-YY
*******************************/

long FmsDateGetActualDays(DateType* FmsDate);

/********** Class Type of C Functions ****************/
void FmsDateSetup(DateType* FmsDate, const char* Date, const char *EndDate, const char* RateBasis, int AADays)
{
	FmsDate->m_Date[0] = 0;
	FmsDate->m_RateBasis[0] = 0;

	FmsDate->m_nAADays = AADays;
	if(RateBasis && strlen(RateBasis) > 0)
		SaveCopy(FmsDate->m_RateBasis, sizeof(FmsDate->m_RateBasis), RateBasis, 0);
	else
		SaveCopy(FmsDate->m_RateBasis, sizeof(FmsDate->m_RateBasis), "A/365", 0); // Default

	SaveCopy(FmsDate->m_Date, sizeof(FmsDate->m_Date), Date, 0);
	SaveCopy(FmsDate->m_EndDate, sizeof(FmsDate->m_EndDate), EndDate, 0);

	FmsDateGetDayMonthYear(FmsDate, Date);
	FmsDateGetBasis(FmsDate, RateBasis);
}
/*****************************************************/
/*****************************************************/
void FmsDateGetBasis(DateType* FmsDate, const char* RateBasis)
{
	FmsDate->m_nYearBase2 = 0;
	FmsDate->m_nYearBase = 365;
	FmsDate->m_nMonthBase = 31;

	if(RateBasis && strlen(RateBasis) > 0)
	{
		SaveCopy(FmsDate->m_RateBasis, sizeof(FmsDate->m_RateBasis), RateBasis, 0);

		if(strstr(RateBasis, "360"))
			FmsDate->m_nYearBase = 360;
		else
			if(strcmp(RateBasis, "A/A (A)") == 0)
				FmsDate->m_nYearBase = GetAAAYearBase(FmsDate->m_Date);
			else
				if(strcmp(RateBasis, "A/A (AS)") == 0 || strcmp(RateBasis, "A/A (R)") == 0)
					FmsDate->m_nYearBase = GetDaysInYear(FmsDate->m_EndDate);
				else
					if(strcmp(RateBasis, "A/364") == 0)
						FmsDate->m_nYearBase = 364;
					else
						if(strstr(RateBasis, "A/A"))
						{
							if(FmsDate->m_nAADays == 0)
		   						FmsDate->m_nYearBase = 364;
							else
								FmsDate->m_nYearBase = FmsDate->m_nAADays*((int) round(365.0/FmsDate->m_nAADays, 0));
						}
						else
							if(strstr(RateBasis, "AB/252"))
								FmsDate->m_nYearBase = 252;

		if(strstr(RateBasis, "30"))
			FmsDate->m_nMonthBase = 30;
	}
}
/*****************************************************/
/*****************************************************/
int FmsDateGetDayMonthYear(DateType* FmsDate, const char* Date)
{
	char* p, *q = NULL, Buffer[100];
	const char* month[] = {" ", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", 
                          "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
	int i;     

	FmsDate->m_nDay = 0;
	FmsDate->m_nMonth = 0;
	FmsDate->m_nYear = 0;

	SaveCopy(Buffer, sizeof(Buffer), Date, 0);

	p = strtok_s(Buffer, "-", &q);
	while(p)
	{
		if(isdigit(p[0]))
		{
			if(!FmsDate->m_nDay)
				FmsDate->m_nDay = atoi(p);
			else
			{
				FmsDate->m_nYear = atoi(p);
				if(strlen(p) == 2)
					if(FmsDate->m_nYear >= 80)
						FmsDate->m_nYear += 1900;
					else
						FmsDate->m_nYear += 2000;
			}
		}
		else
			for(i = 0; i < 13; i++)
				if(strcmp(p, month[i]) == 0)
					FmsDate->m_nMonth = i;

		p = strtok_s(NULL, "-", &q);
	}

	return (FmsDate->m_nDay && FmsDate->m_nMonth && FmsDate->m_nYear) ? 1 : 0;
}
/*****************************************************/
/*****************************************************/
int FmsDateIsLeapYear(DateType* FmsDate)
{
	if((FmsDate->m_nYear % 4 == 0 && FmsDate->m_nYear % 100 != 0) ||
		(FmsDate->m_nYear % 4 == 0 && FmsDate->m_nYear % 100 == 0 && FmsDate->m_nYear % 400 == 0))
			return 1;

	return 0;
}
/*****************************************************/
/*****************************************************/
const char* FmsDateGetDate(DateType* FmsDate)
{
	return FmsDate->m_Date;
}
/*****************************************************/
/*****************************************************/
const char* FmsDateGetRateBasis(DateType* FmsDate)
{
	return FmsDate->m_RateBasis;
}
/*****************************************************/
/*****************************************************/
int FmsDateGetMonthBase(DateType* FmsDate)
{
	FmsDate->m_nMonthBase = 31;
	if(FmsDate->m_RateBasis && strstr(FmsDate->m_RateBasis, "30"))
		FmsDate->m_nMonthBase = 30;

	return FmsDate->m_nMonthBase;
}
/*****************************************************/
/*****************************************************/
int FmsDateGetYearBase(DateType* FmsDate)
{
	return FmsDate->m_nYearBase;
}
/*****************************************************/
/*****************************************************/
int FmsDateCompareDate(DateType* FmsDate, const char* Date)
{
	DateType FmsDate2;

	FmsDateSetup(&FmsDate2, Date, FmsDate->m_EndDate, FmsDate->m_RateBasis, FmsDate->m_nAADays);

	if(FmsDate->m_nYear > FmsDate2.m_nYear) 
		return 1;
	else
		if(FmsDate->m_nYear == FmsDate2.m_nYear && 
			FmsDate->m_nMonth > FmsDate2.m_nMonth) 
			return 1;
		else
			if(FmsDate->m_nYear == FmsDate2.m_nYear && 
				FmsDate->m_nMonth == FmsDate2.m_nMonth &&
				FmsDate->m_nDay > FmsDate2.m_nDay)
				return 1;
			else
				if(FmsDate->m_nYear == FmsDate2.m_nYear && 
					FmsDate->m_nMonth == FmsDate2.m_nMonth &&
					FmsDate->m_nDay == FmsDate2.m_nDay)
					return 0;
	return -1;
}
/*****************************************************/
/*****************************************************/
long FmsDateGetActualDays(DateType* FmsDate)
{
	char Buf[30];
	int  Mp, Yp, T;
	long Days = 1;

	if(FmsDate->m_nMonth <= 2)
	{
		Mp = 0;
		Yp = FmsDate->m_nYear - 1;
	}
	else
	{
		sprintf_s(Buf, sizeof(Buf), "%lf", 0.4*FmsDate->m_nMonth + 2.3);
		Mp = atoi(Buf);
		Yp = FmsDate->m_nYear;
	}

	T = Yp/4 - Yp/100 + Yp/400;
	Days = Days*365*FmsDate->m_nYear + 31*(FmsDate->m_nMonth - 1) + FmsDate->m_nDay + T - Mp;

	return Days;
}
/*****************************************************/
/*****************************************************/
int FmsDateLastDayofMonth(DateType* FmsDate)
{
	const int DaysOfMonth[] = {0, 31, 28, 31, 30, 31, 30, 
                              31, 31, 30, 31, 30, 31};

	if(FmsDate->m_nMonth == 2)
		return DaysOfMonth[FmsDate->m_nMonth] + FmsDateIsLeapYear(FmsDate);
	else
		return DaysOfMonth[FmsDate->m_nMonth];
}
/*****************************************************/
/*****************************************************/
int FmsDateGetDays(DateType* FmsDate, const char* Date)
{
	const int DaysOfMonth[] = {0, 31, 28, 31, 30, 31, 30, 
                              31, 31, 30, 31, 30, 31};
	
	DateType FmsDate2, FmsDate3;
	char buf[20];
	int D1, D2, M1, M2, Y1, Y2;

	FmsDate->m_nIntDays = 0;
	FmsDate->m_nIntDays2 = 0;
	FmsDate->m_nYearBase2 = 0;
	if(FmsDateCompareDate(FmsDate, Date) >= 0)
		return 0;

	FmsDateSetup(&FmsDate2, Date, FmsDate->m_EndDate, FmsDate->m_RateBasis, FmsDate->m_nAADays);
	if(strstr(FmsDate->m_RateBasis, "AB/252"))
	{
		FmsDate->m_nIntDays = GetAB_252Days(FmsDate->m_Date, Date);
		return FmsDate->m_nIntDays;
	}

	if(strncmp(FmsDate->m_RateBasis, "30", 2) == 0 && strstr(FmsDate->m_RateBasis, "/360"))
	{
		D1 = FmsDate->m_nDay;
		M1 = FmsDate->m_nMonth;
		Y1 = FmsDate->m_nYear;
		D2 = FmsDate2.m_nDay;
		M2 = FmsDate2.m_nMonth;
		Y2 = FmsDate2.m_nYear;

		if(strcmp(FmsDate->m_RateBasis, "30/360") == 0)
		{
			if(D2 == FmsDateLastDayofMonth(&FmsDate2) && FmsDate2.m_nMonth == 2 &&
				D1 == FmsDateLastDayofMonth(FmsDate) && FmsDate->m_nMonth == 2)
				D2 = 30;   

			if(D1 == FmsDateLastDayofMonth(FmsDate) && FmsDate->m_nMonth == 2)
				D1 = 30;

			if(D2 == 31)		// 			if(D2 == 31 && D1 >= 30)
				D2 = 30;		//				D2 = 30;

			if(D1 == 31)
				D1 = 30;
		}
		else
			if(strcmp(FmsDate->m_RateBasis, "30E/360") == 0)
			{
				if(D1 == 31)
					D1 = 30;
				if(D2 == 31)
					D2 = 30;
			}
			else
				if(strcmp(FmsDate->m_RateBasis, "30EI/360") == 0)
				{
					if(D1 == FmsDateLastDayofMonth(FmsDate))
						D1 = 30;
					
					if(D2 == FmsDateLastDayofMonth(&FmsDate2) && FmsDate2.m_nMonth != 2)
						D2 = 30;
				}
				else
					if(strcmp(FmsDate->m_RateBasis, "30E+/360") == 0)
					{
						if(D1 == 31)
							D1 = 30;
						if(D2 == 31)
						{
							D2 = 1;
							M2 += 1;
							if(M2 > 12)
							{
								Y2 += 1;
								M2 = 1;
							}
						}
					}
		
			FmsDate->m_nIntDays = (D2 - D1) + ((M2 - M1) + (Y2 - Y1)*12)*30;
			return FmsDate->m_nIntDays;
	}

	if(strcmp(FmsDate->m_RateBasis, "A/A (I)") == 0) // day1/365 + day2/366 = (day1 + day2 * 365/366)/365
	{
		if(FmsDate->m_nYear != FmsDate2.m_nYear)
		{
			Y1 = 365 + FmsDateIsLeapYear(FmsDate);
			Y2 = 365 + FmsDateIsLeapYear(&FmsDate2);
		}
		
		if(FmsDate->m_nYear == FmsDate2.m_nYear || Y1 == Y2)
			FmsDate->m_nIntDays = FmsDateGetActualDays(&FmsDate2) - FmsDateGetActualDays(FmsDate);
		else
		{
			FmsDate->m_nYearBase = Y1;
			FmsDate->m_nYearBase2 = Y2;

			sprintf_s(buf, sizeof(buf), "31-DEC-%02d", FmsDate->m_nYear % 100);
			FmsDateSetup(&FmsDate3, buf, FmsDate->m_EndDate, FmsDate->m_RateBasis, FmsDate->m_nAADays);

			FmsDate->m_nIntDays = FmsDateGetActualDays(&FmsDate3) - FmsDateGetActualDays(FmsDate) + 1;
			FmsDate->m_nIntDays2 = FmsDateGetActualDays(&FmsDate2) - FmsDateGetActualDays(&FmsDate3);
		}
	}
	else
		FmsDate->m_nIntDays = FmsDateGetActualDays(&FmsDate2) - FmsDateGetActualDays(FmsDate);

	return FmsDate->m_nIntDays;
}
   


/*****************************************************/
/*****************************************************/
void AssetSetup(AssetType* Asset, double NomAmount, double Price, const char* ValueDate, const char *EndDate, 
				const char* RateBasis, double FxRate, double MktPrice, double MktFxRate, double BondFact, 
				int AADays, int Accruable, int PrePaid, const char* TransType)
{
	Asset->m_NomAmount = NomAmount;
	Asset->m_Price = Price;
	Asset->m_FxRate = FxRate;
	Asset->m_MktPrice = MktPrice;
	Asset->m_MktFxRate = MktFxRate;
	Asset->m_BondFact = BondFact;
	Asset->m_nAADays = AADays; /* Days in asset periods */
	Asset->m_bAccruable = Accruable;
	Asset->m_bPrePaid = PrePaid;
	Asset->m_ValueDate[0] = 0;
	Asset->m_TransType[0] = 0;
	Asset->m_RateBasis[0] = 0;
	Asset->m_CapitRate = 0;
	Asset->m_AmortFact = 1;
	Asset->m_PlusAmount = 0;
	Asset->m_AccruedDays = 0;
	AssetResetInterest(Asset);

	if(ValueDate && strlen(ValueDate) > 0)
		SaveCopy(Asset->m_ValueDate, sizeof(Asset->m_ValueDate), ValueDate, 0);

	if(EndDate && strlen(EndDate) > 0)
		SaveCopy(Asset->m_EndDate, sizeof(Asset->m_EndDate), EndDate, 0);

	if(RateBasis && strlen(RateBasis) > 0)
		SaveCopy(Asset->m_RateBasis, sizeof(Asset->m_RateBasis), RateBasis, 0);

	if(TransType && strlen(TransType) > 0)
		SaveCopy(Asset->m_TransType, sizeof(Asset->m_TransType), TransType, 0);
}
/*****************************************************/
/*****************************************************/
void AssetResetInterest(AssetType* Asset)
{
	Asset->m_IntArrCount = 0;
}
/*****************************************************/
/*****************************************************/
void AssetSetupInterest(AssetType* Asset, const char* FromDate, const char* ToDate, 
						double Rate, double AmortFact, int AADays)
{
	if(FromDate && strlen(FromDate) > 0)
	{
		SaveCopy(Asset->m_IntArr[Asset->m_IntArrCount].m_FromDate, 
				sizeof(Asset->m_IntArr[Asset->m_IntArrCount].m_FromDate), FromDate, 0);
   
		if(ToDate && strlen(ToDate) > 0)
		{
			SaveCopy(Asset->m_IntArr[Asset->m_IntArrCount].m_ToDate, 
					sizeof(Asset->m_IntArr[Asset->m_IntArrCount].m_ToDate), ToDate, 0);
			Asset->m_IntArr[Asset->m_IntArrCount].m_nAADays = AADays; /* Days in asset periods */
			Asset->m_IntArr[Asset->m_IntArrCount].m_Rate = Rate;
			Asset->m_IntArr[Asset->m_IntArrCount].m_AmortFact = AmortFact;
			Asset->m_IntArr[Asset->m_IntArrCount].m_nInclusive = 0;
			Asset->m_IntArrCount++;
		}
	}
}
/*****************************************************/
/*****************************************************/
void AssetSetupInterest2(AssetType* Asset, const char* FromDate, const char* ToDate, 
						double Rate, double AmortFact, int AADays, int Inclusive)
{
	if(FromDate && strlen(FromDate) > 0)
	{
		SaveCopy(Asset->m_IntArr[Asset->m_IntArrCount].m_FromDate, 
				sizeof(Asset->m_IntArr[Asset->m_IntArrCount].m_FromDate), FromDate, 0);
   
		if(ToDate && strlen(ToDate) > 0)
		{
			SaveCopy(Asset->m_IntArr[Asset->m_IntArrCount].m_ToDate, 
					sizeof(Asset->m_IntArr[Asset->m_IntArrCount].m_ToDate), ToDate, 0);
			Asset->m_IntArr[Asset->m_IntArrCount].m_nAADays = AADays; /* Days in asset periods */
			Asset->m_IntArr[Asset->m_IntArrCount].m_Rate = Rate;
			Asset->m_IntArr[Asset->m_IntArrCount].m_AmortFact = AmortFact;
			Asset->m_IntArr[Asset->m_IntArrCount].m_nInclusive = Inclusive;
			Asset->m_IntArrCount++;
		}
	}
}
/*****************************************************/
/*****************************************************/
void AssetSetupCapit(AssetType* Asset, const char* CapitStart, const char* CapitEnd, 
					 double AmortFact, double CapitRate)
{
	if(CapitStart && strlen(CapitStart) > 0)
		SaveCopy(Asset->m_CapitStart, sizeof(Asset->m_CapitStart), CapitStart, 0);
   
	if(CapitEnd && strlen(CapitEnd) > 0)
		SaveCopy(Asset->m_CapitEnd, sizeof(Asset->m_CapitEnd), CapitEnd, 0);
   
	Asset->m_AmortFact = AmortFact; 
	Asset->m_CapitRate = CapitRate;
}
/*****************************************************/
/*****************************************************/
void AssetSetupPlusAmount(AssetType* Asset, double PlusAmount)
{
	Asset->m_PlusAmount = PlusAmount;
}
/*****************************************************/
/*****************************************************/
int AssetGetAccruedDays(AssetType* Asset)
{
	return Asset->m_AccruedDays;
}
/*****************************************************/
/*****************************************************/
double AssetGetFaceAmount(AssetType* Asset)
{
	if(strcmp(Asset->m_TransType, "LEVERAGE") == 0)
		return Asset->m_NomAmount;

	return Asset->m_NomAmount*Asset->m_AmortFact;
}
/*****************************************************/
/*****************************************************/
double AssetGetAccretionAmount(AssetType* Asset)
{
	int Days;
   
	if(Asset->m_CapitRate == 0)
		return 0;
   
	return AssetComputeInterest2(Asset->m_NomAmount, Asset->m_CapitRate, Asset->m_CapitStart, 
				Asset->m_CapitEnd, Asset->m_EndDate, Asset->m_BondFact, Asset->m_AmortFact, 
				0, Asset->m_RateBasis, Asset->m_nAADays, &Days);
}
/*****************************************************/
/*****************************************************/
double AssetGetPrePaidAccretionAmount(AssetType* Asset)
{
	int Days;

	if(Asset->m_CapitRate == 0)
		return 0;
   
	return AssetComputeInterest2(Asset->m_NomAmount, Asset->m_CapitRate, Asset->m_CapitStart, 
				Asset->m_ValueDate, Asset->m_EndDate, Asset->m_BondFact, Asset->m_AmortFact, 
				0, Asset->m_RateBasis, Asset->m_nAADays, &Days);
}
/*****************************************************/
/*****************************************************/
double AssetGetPrincipleCash(AssetType* Asset)
{
	double Factor;

	if(strcmp(Asset->m_TransType, "LEVERAGE") == 0 ||
		strcmp(Asset->m_TransType, "FOREX") == 0)
		Factor = 1;
	else
		if(strcmp(Asset->m_TransType, "CDS") == 0)
			Factor = (Asset->m_Price - 100)*Asset->m_BondFact;
		else
			Factor = Asset->m_Price*Asset->m_BondFact;
 
	return AssetGetFaceAmount(Asset)*Factor;
}
/*****************************************************/
/*****************************************************/
double AssetGetAccretionCash(AssetType* Asset)
{
	return AssetGetAccretionAmount(Asset)*Asset->m_Price*Asset->m_BondFact;
}
/*****************************************************/
/*****************************************************/
double AssetGetPrePaidAccretionCash(AssetType* Asset)
{
	return AssetGetPrePaidAccretionAmount(Asset)*Asset->m_Price*Asset->m_BondFact;
}
/*****************************************************/
/*****************************************************/
double AssetGetMarketValue(AssetType* Asset)
{
	double Factor;

	if(strcmp(Asset->m_TransType, "LEVERAGE") == 0 ||
		strcmp(Asset->m_TransType, "FOREX") == 0)
		Factor = 1;
	else
		Factor = Asset->m_MktPrice*Asset->m_BondFact; // Face value 

	return AssetGetFaceAmount(Asset)*Factor;
}
/*****************************************************/
/*****************************************************/
double AssetGetAccretionMarketValue(AssetType* Asset)
{
	return Asset->m_MktPrice*AssetGetAccretionAmount(Asset);
}
/*****************************************************/
/*****************************************************/
double AssetGetInterest(AssetType* Asset)
{
	double Amt = 0;
	int i, Days;

	Asset->m_AccruedDays = 0;
	if(!Asset->m_bAccruable)
		return Amt;
   
	for(i = 0; i < Asset->m_IntArrCount; i++)
	{
		if(Asset->m_IntArr[i].m_Rate != 0)
		{
			Amt += AssetComputeInterest2(Asset->m_NomAmount, Asset->m_IntArr[i].m_Rate, 
								Asset->m_IntArr[i].m_FromDate, Asset->m_IntArr[i].m_ToDate, 
								Asset->m_IntArr[i].m_ToDate, Asset->m_BondFact, 
								Asset->m_IntArr[i].m_AmortFact, 0, Asset->m_RateBasis, 
								Asset->m_IntArr[i].m_nAADays, &Days);
			Asset->m_AccruedDays += Days;
		}
	}

	return Amt;
}
/*****************************************************/
/*****************************************************/
double AssetGetPrePaidInterest(AssetType* Asset)
{
	int i, Days;
	double Factor = 1, Amount = 0;
	DateType ValueDate;

	if(!Asset->m_bPrePaid)
		return 0;

	FmsDateSetup(&ValueDate, Asset->m_ValueDate, Asset->m_EndDate, Asset->m_RateBasis, Asset->m_nAADays);
	
	for(i = 0; i < Asset->m_IntArrCount; i++)
	{
		if(Asset->m_IntArr[i].m_nInclusive)
		{
			if(FmsDateCompareDate(&ValueDate, Asset->m_IntArr[i].m_FromDate) >= 0 &&
				FmsDateCompareDate(&ValueDate, Asset->m_IntArr[i].m_ToDate) <= 0)
			{
				return AssetComputeInterest2(Asset->m_NomAmount, Asset->m_IntArr[i].m_Rate, 
						Asset->m_IntArr[i].m_FromDate, Asset->m_ValueDate, Asset->m_IntArr[i].m_ToDate, 
						Asset->m_BondFact, Asset->m_IntArr[i].m_AmortFact, 
						Asset->m_PlusAmount, Asset->m_RateBasis, 
						Asset->m_IntArr[i].m_nAADays, &Days)*Factor;
			}
		}
		else
		{
			if(FmsDateCompareDate(&ValueDate, Asset->m_IntArr[i].m_FromDate) >= 0 &&
				FmsDateCompareDate(&ValueDate, Asset->m_IntArr[i].m_ToDate) < 0)
			{
				return AssetComputeInterest2(Asset->m_NomAmount, Asset->m_IntArr[i].m_Rate, 
						Asset->m_IntArr[i].m_FromDate, Asset->m_ValueDate, Asset->m_IntArr[i].m_ToDate, 
						Asset->m_BondFact, Asset->m_IntArr[i].m_AmortFact, 
						Asset->m_PlusAmount, Asset->m_RateBasis, 
						Asset->m_IntArr[i].m_nAADays, &Days)*Factor;
			}
		}
	}

	return 0;
}
/*****************************************************/
/*****************************************************/
double AssetGetLeverageExpense(double Amount, double IntRate, const char* FromDate, 
							   const char* ToDate, const char *EndDate, const char* RateBasis)
{
	int Days;

	return AssetComputeInterest2(Amount, IntRate, FromDate, ToDate, EndDate, 0.01, 1, 0, RateBasis, 0, &Days);
}
/*****************************************************/
/*****************************************************/
double AssetComputeInterest(double NomAmount, double Rate, int Days, int Base, 
			double BondFact, double AmortFact, double Plus, const char* RateBasis)
{
	if(strcmp(RateBasis, "A/A (AS)") == 0)
		return round(NomAmount/1000*round(1000*(Rate/100*Days/Base*AmortFact + 
									Plus*BondFact), 2), 2);
	else
		if(strcmp(RateBasis, "A/365I") == 0)
		{
			double d, x, y;
			d = Days;
			x = round(100*(pow(1 + Rate/100, d/365) - 1), 5);
			y = round(x*d/365, 5);
			return NomAmount/100*y;
		}
		else
			if(strcmp(RateBasis, "AB/252") == 0)
				return round(NomAmount*((pow(1.0 + Rate/100, ((double) Days)/Base) - 1)*AmortFact + Plus*BondFact), 2);
			else
				return round(NomAmount*(Rate/100*Days/Base*AmortFact + Plus*BondFact), 2);
}
/*****************************************************/
/*****************************************************/
double AssetComputeInterestWO(double NomAmount, double Rate, int Days, int Base, 
			double BondFact, double AmortFact, double Plus, const char* RateBasis)
{
	if(strcmp(RateBasis, "A/A (AS)") == 0)
		return NomAmount/1000*round(1000*(Rate/100*Days/Base*AmortFact + 
									Plus*BondFact), 2);
	else
		if(strcmp(RateBasis, "A/365I") == 0)
		{
			double d, x, y;
			d = Days;
			x = round(100*(pow(1 + Rate/100, d/365) - 1), 5);
			y = round(x*d/365, 5);
			return NomAmount/100*y;
		}
		else
			if(strcmp(RateBasis, "AB/252") == 0)
				return NomAmount*((pow(1.0 + Rate/100, ((double) Days)/Base) - 1)*AmortFact + Plus*BondFact);
			else
				return NomAmount*(Rate/100*Days/Base*AmortFact + Plus*BondFact);
}
/*****************************************************/
/*****************************************************/
double AssetComputeInterest2(double NomAmount, double Rate, const char *IntFrom, 
		const char *IntTo, const char *EndDate, double BondFact, double AmortFact, 
		double Plus, const char* RateBasis, int AADays, int *Days)
{
	double Amount;
	DateType FromDate;

	FmsDateSetup(&FromDate, IntFrom, EndDate, RateBasis, AADays);

	*Days = FmsDateGetDays(&FromDate, IntTo);
	Amount = AssetComputeInterestWO(NomAmount, Rate, FromDate.m_nIntDays, 
						FromDate.m_nYearBase, BondFact, AmortFact, Plus, RateBasis);

	if(strcmp(RateBasis, "A/A (I)") == 0 && FromDate.m_nYearBase2 > 0)
	{
		Amount += AssetComputeInterestWO(NomAmount, Rate, FromDate.m_nIntDays2, 
						FromDate.m_nYearBase2, BondFact, AmortFact, Plus, RateBasis);
		*Days += FromDate.m_nIntDays2;
	}

	return round(Amount, 2);
}
