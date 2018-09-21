#ifndef __COSTMETHOD_H__
#define __COSTMETHOD_H__

typedef struct
{
	char RunDir;
	char LastDir;
	char Future;
	int CostDataArrayIndex;
	int nRunDir;
	int TransType;
	int bAdd;
	int InfBond;
	double NomAmount;
	double CurrNom;
	double AvPrice;
	double AvUSDPrice;
	double AvFxrate;
	double Amort;
	double BondFact;
	double Amount;
	double Profit;
	double FProfit;
	double UnRealProfit;
	double UnRealFProfit;
	double Market;
	double Exrate;
	double RealProfit;
	double RealFProfit;
} CostType;

typedef struct
{
	int TransNum;
	char TradeDate[11];
	char EndDate[11];
	int TransType;
	int Booking;
	char Dir;
	double NomAmount;
	double Price;	
	double Fxrate;
	double ClFxrate;
	double USDPrice;
	double Amort;
	long HoldingDays;
} CostDataType;

typedef struct
{
	int TransNum;
	char TradeDate[11];
	char EndDate[11];
	char Dir;
	double NomAmount;
	double Price;
	double Fxrate;
	double ClFxrate;
	double USDPrice;
	double Profit;
	double FProfit;
	double UnRealPL;
	double UnRealFPL;
	double Remain;
	double Amort;
	int Addable;
} CostNodeType;

void CostAdjustAmortFact(CostType *CostRes, int bFifo, double Amort, double Fxrate, double NomAmount);
void CostAvCostMethod(CostType *CostRes, CostDataType *CostData);
double CostComputeAmount(CostType *CostRes, CostDataType *CostData);
void CostInit(CostType *CostRes, double AmortFact, char Future, int bAdd);
void CostInit2(CostType *CostRes, double Market, double Exrate, double AmortFact, char Future, int bAdd);
void CostSetup(CostType *CostRes, CostDataType *CostData, const char *TransType, const char *AssetType, 
			   char Dir, const char *TradeDate, const char *EndDate, int TransNum, double NomAmount, 
			   double Price, double Fxrate, double AmortFact, int OptTransNum, double OptPrice, 
			   double OptFxrate, char OptDir);
void CostSetup2(CostType *CostRes, CostDataType *CostData, const char *TransType, double Factor, 
				char Dir, const char *TradeDate, const char *EndDate, int TransNum, double NomAmount, 
				double Price, double Fxrate, double AmortFact, int OptTransNum, double OptPrice, 
				double OptFxrate, char OptDir);
void CostSetup3(CostType *CostRes, CostDataType *CostData, const char *TransType, double Factor, 
				char Dir, const char *TradeDate, const char *EndDate, int TransNum, double NomAmount, 
				double Price, double Fxrate, double Market, double Exrate, double AmortFact, int OptTransNum, 
				double OptPrice, double OptFxrate, char OptDir);

void CostFXSetup(CostType *CostRes, CostDataType *CostData, const char *TransType, char Dir, 
				 const char *TradeDate, int TransNum, double NomAmount, double Price, 
				 double TrFxrate, double ClFxrate, double Factor, int Booking, double AmortFact);

void CostFxPL(CostType *CostRec, CostDataType *CostData);

int CostFIFOMethod(CostType *CostRes, CostDataType *CostData);
int CostFIFOMethodDetail(CostType *CostRes, CostDataType *CostData);

int CostFIFOMethodNav(CostType *CostRes, CostDataType *CostData, 
		const char *PrevDate, double PrevAmort, double PrevPrice, double PrevFxrate);
int CostFIFOMethodNavDetail(CostType *CostRes, CostDataType *CostData, 
			const char *PrevDate, double PrevAmort, double PrevPrice, double PrevFxrate);
void CostFIFOAvCostNavMethod(CostType *CostRes, CostDataType *CostData, 
						  const char *PrevDate, double PrevPrice, double PrevFxrate);
void CostUnRealPL(CostType *CostRes);
void CostUnRealPL2(CostType *CostRes, double Amort);
void CostUpdateTotal(CostType *CostResult, CostType *CostRes);

void CostFIFOAvCostMethod(CostType *CostRes, CostDataType *CostData);
void CostAddPL(CostType *CostRes);
#endif