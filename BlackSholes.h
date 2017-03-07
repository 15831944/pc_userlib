#ifndef __BLACKSHOLES_H__
#define __BLACKSHOLES_H__

typedef struct {
	char OptType[10];
	char AssetType[16];
	char Dir;
	double D1;
	double D2;
	double Dt;
	double Price;
	double Strike;
	int DaysToExp;
	double YearsToExp;
	double RiskFreeRate;
	double Yield;
	double Volatility;
	double OptPrice;
	double Delta;
	double Delta2;
	double Gamma;
	double Gamma2;
	double Theta;
	double Vega;
	double Rho;
	double Rho2;
	double Vanna;
	double Charm;
	double Speed;
	double Zomma;
	double Color;
	double DvegaDtime;
	double Vomma;
	double ND1;   // Standard Normal Distribution for D1
	double ND2;   // Standard Normal Distribution for D2
	double CND1;  // Culmulative Standard Normal Distribution for D1
	double CND2;  // Culmulative Standard Normal Distribution for D2
	double NN1;
	double NN2;
	double ExpYield;
	double ExpRiskFreeRate;
	int bD1D2Ready;
	int bND1ND2Ready;
} BSDataType;

double BlackSholesDt(BSDataType *BSData);
double BlackSholesD1D2(BSDataType *BSData);
double BlackSholesND1ND2(BSDataType *BSData);
double BlackSholesDelta(BSDataType *BSData);
double BlackSholesDelta2(BSDataType *BSData);
double BlackSholesGamma(BSDataType *BSData);
double BlackSholesGamma2(BSDataType *BSData);
double BlackSholesTheta(BSDataType *BSData);
double BlackSholesVega(BSDataType *BSData);
double BlackSholesRho(BSDataType *BSData);
double BlackSholesRho2(BSDataType *BSData);
double BlackSholesVanna(BSDataType *BSData);
double BlackSholesCharm(BSDataType *BSData);
double BlackSholesSpeed(BSDataType *BSData);
double BlackSholesZomma(BSDataType *BSData);
double BlackSholesColor(BSDataType *BSData);
double BlackSholesDvegaDtime(BSDataType *BSData);
double BlackSholesVomma(BSDataType *BSData);
double BlackSholesImpliedVolatility(BSDataType *BSData);
double BlackSholesOptionPrice(BSDataType *BSData);

double BlackSholes(BSDataType *BSData);
void BlackSholesSetup(BSDataType *BSData, const char *OptType, char Dir, 
					  const char *AssetType, double Price, double Strike, 
					  int DaysToExp, double RiskFreeRate, double Yield, 
					  double Volatility, double OptPrice);
#endif
