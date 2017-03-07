#include "normsdist.h"
#include "blacksholes.h"
#include <math.h>
#include <string.h>
#include "gen_fun.h"
#include "USERINFO.H"

double _BlackSholesDelta(BSDataType *BSData);
double _BlackSholesDelta2(BSDataType *BSData);
double _BlackSholesGamma(BSDataType *BSData);
double _BlackSholesGamma2(BSDataType *BSData);
double _BlackSholesTheta(BSDataType *BSData);
double _BlackSholesVega(BSDataType *BSData);
double _BlackSholesRho(BSDataType *BSData);
double _BlackSholesRho2(BSDataType *BSData);
double _BlackSholesVanna(BSDataType *BSData);
double _BlackSholesCharm(BSDataType *BSData);
double _BlackSholesSpeed(BSDataType *BSData);
double _BlackSholesZomma(BSDataType *BSData);
double _BlackSholesColor(BSDataType *BSData);
double _BlackSholesDvegaDtime(BSDataType *BSData);
double _BlackSholesVomma(BSDataType *BSData);

/********************************************
********************************************/
double BlackSholesDt(BSDataType *BSData)
{
	BSData->Dt = BSData->Volatility*sqrt(BSData->YearsToExp);

	return BSData->Dt;
}

/********************************************
********************************************/
double BlackSholesExp(BSDataType *BSData)
{
	BSData->ExpYield = exp(-BSData->Yield*BSData->YearsToExp);
	BSData->ExpRiskFreeRate = exp(-BSData->RiskFreeRate*BSData->YearsToExp);

	return BSData->ExpYield;
}
/********************************************
********************************************/
double BlackSholesD1D2(BSDataType *BSData)
{
	BlackSholesDt(BSData);

	BSData->D1 = (log(BSData->Price/BSData->Strike) + (BSData->RiskFreeRate - BSData->Yield + 
					0.5*BSData->Volatility*BSData->Volatility)*BSData->YearsToExp)/BSData->Dt;

	BSData->D2 = BSData->D1 - BSData->Dt;

	BSData->bD1D2Ready = 1;

	BSData->NN1 = SND(BSData->D1);
	BSData->NN2 = SND(BSData->D2);

	BlackSholesExp(BSData);
	return BSData->Dt;
}

/********************************************
********************************************/
double BlackSholesND1ND2(BSDataType *BSData)
{
	double Factor = 1.0;

	BlackSholesD1D2(BSData);

	if(strstr(BSData->OptType, "PUT"))
		Factor *= -1;
	
	BSData->ND1 = SND(Factor*BSData->D1);
	BSData->ND2 = SND(Factor*BSData->D2);
	BSData->CND1 = CSND(Factor*BSData->D1);
	BSData->CND2 = CSND(Factor*BSData->D2);

	BSData->bND1ND2Ready = 1;

	return BSData->ND1;
}

/********************************************
********************************************/
double BlackSholesDelta(BSDataType *BSData)
{
	BlackSholesND1ND2(BSData);
	
	return _BlackSholesDelta(BSData);
}

/********************************************
********************************************/
double BlackSholesDelta2(BSDataType *BSData)
{
	BlackSholesND1ND2(BSData);
	
	return _BlackSholesDelta2(BSData);
}

/********************************************
********************************************/
double BlackSholesGamma(BSDataType *BSData)
{
	BlackSholesD1D2(BSData);
	
	return _BlackSholesGamma(BSData);
}

/********************************************
********************************************/
double BlackSholesGamma2(BSDataType *BSData)
{
	BlackSholesD1D2(BSData);
	
	return _BlackSholesGamma2(BSData);
}

/********************************************
********************************************/
double BlackSholesTheta(BSDataType *BSData)
{
	BlackSholesND1ND2(BSData);

	return _BlackSholesTheta(BSData);
}

/********************************************
********************************************/
double BlackSholesVega(BSDataType *BSData)
{
	BlackSholesD1D2(BSData);

	return _BlackSholesVega(BSData);
}

/********************************************
********************************************/
double BlackSholesRho(BSDataType *BSData)
{
	BlackSholesND1ND2(BSData);

	return _BlackSholesRho(BSData);
}

/********************************************
********************************************/
double BlackSholesRho2(BSDataType *BSData)
{
	BlackSholesND1ND2(BSData);

	return _BlackSholesRho2(BSData);
}

double BlackSholesVanna(BSDataType *BSData)
{
	BlackSholesD1D2(BSData);
	
	return _BlackSholesVanna(BSData);
}

double BlackSholesCharm(BSDataType *BSData)
{
	BlackSholesD1D2(BSData);
	
	return _BlackSholesCharm(BSData);
}

double BlackSholesSpeed(BSDataType *BSData)
{
	BlackSholesD1D2(BSData);

	return _BlackSholesSpeed(BSData);
}

double BlackSholesZomma(BSDataType *BSData)
{
	BlackSholesD1D2(BSData);
	
	return _BlackSholesZomma(BSData);
}

double BlackSholesColor(BSDataType *BSData)
{
	BlackSholesD1D2(BSData);
	
	return _BlackSholesColor(BSData);
}

double BlackSholesDvegaDtime(BSDataType *BSData)
{
	BlackSholesD1D2(BSData);
	
	return _BlackSholesDvegaDtime(BSData);
}

double BlackSholesVomma(BSDataType *BSData)
{
	BlackSholesD1D2(BSData);
	
	return _BlackSholesVomma(BSData);
}
/********************************************
********************************************/
double _BlackSholesDelta(BSDataType *BSData)
{
	if(!BSData->bD1D2Ready || !BSData->bND1ND2Ready)
		return BlackSholesDelta(BSData);

	BSData->Delta = BSData->ExpYield*BSData->CND1;
	if(strstr(BSData->OptType, "PUT"))
		BSData->Delta *= -1;

	return BSData->Delta;
}

/********************************************
********************************************/
double _BlackSholesDelta2(BSDataType *BSData)
{
	if(!BSData->bD1D2Ready || !BSData->bND1ND2Ready)
		return BlackSholesDelta2(BSData);
	
	if(strstr(BSData->OptType, "CALL"))
		BSData->Delta2 = -BSData->ExpRiskFreeRate*BSData->CND2;
	else
		BSData->Delta2 = BSData->ExpRiskFreeRate*BSData->CND2;

	return BSData->Delta2;
}

/********************************************
********************************************/
double _BlackSholesGamma(BSDataType *BSData)
{
	if(!BSData->bD1D2Ready || !BSData->bND1ND2Ready)
		return BlackSholesGamma(BSData);

	BSData->Gamma = BSData->NN1*BSData->ExpYield/(BSData->Price*BSData->Dt);

	return BSData->Gamma;
}

/********************************************
********************************************/
double _BlackSholesGamma2(BSDataType *BSData)
{
	if(!BSData->bD1D2Ready || !BSData->bND1ND2Ready)
		return BlackSholesGamma2(BSData);

	BSData->Gamma2 = BSData->NN2*BSData->ExpRiskFreeRate/(BSData->Strike*BSData->Dt);

	return BSData->Gamma2;
}

/********************************************
********************************************/
double _BlackSholesTheta(BSDataType *BSData)
{
	if(!BSData->bD1D2Ready || !BSData->bND1ND2Ready)
		return BlackSholesTheta(BSData);
	
	if(strstr(BSData->OptType, "CALL"))
	{
		BSData->Theta = -BSData->RiskFreeRate*BSData->Strike*BSData->ExpRiskFreeRate*BSData->CND2;
		BSData->Theta += BSData->ExpYield*BSData->Price*
			             (BSData->Yield*BSData->CND1 - BSData->NN1*BSData->Volatility/(2*sqrt(BSData->YearsToExp)));
	}
	else
	{
		BSData->Theta = BSData->RiskFreeRate*BSData->Strike*BSData->ExpRiskFreeRate*BSData->CND2;
		BSData->Theta -= BSData->ExpYield*BSData->Price*
			             (BSData->Yield*BSData->CND1 + BSData->Volatility*BSData->NN1/(2*sqrt(BSData->YearsToExp)));
	}

	return BSData->Theta;
}

/********************************************
********************************************/
double _BlackSholesVega(BSDataType *BSData)
{
	if(!BSData->bD1D2Ready || !BSData->bND1ND2Ready)
		return BlackSholesVega(BSData);

	BSData->Vega = BSData->Price*BSData->ExpYield*BSData->NN1*sqrt(BSData->YearsToExp);

	return BSData->Vega;
}

/********************************************
********************************************/
double _BlackSholesRho(BSDataType *BSData)
{
	if(!BSData->bD1D2Ready || !BSData->bND1ND2Ready)
		return BlackSholesRho(BSData);

	if(strstr(BSData->OptType, "CALL"))
		BSData->Rho = BSData->Strike*BSData->YearsToExp*BSData->ExpRiskFreeRate*BSData->CND2;
	else
		BSData->Rho = -BSData->Strike*BSData->YearsToExp*BSData->ExpRiskFreeRate*BSData->CND2;

	return BSData->Rho;
}

/********************************************
********************************************/
double _BlackSholesRho2(BSDataType *BSData)
{
	if(!BSData->bD1D2Ready || !BSData->bND1ND2Ready)
		return BlackSholesRho2(BSData);

	if(strstr(BSData->OptType, "CALL"))
		BSData->Rho2 = -BSData->Price*BSData->ExpYield*BSData->YearsToExp*BSData->CND1;
	else	
		BSData->Rho2 = BSData->Price*BSData->ExpYield*BSData->YearsToExp*BSData->CND1;

	return BSData->Rho2;
}

/********************************************
********************************************/
double _BlackSholesVanna(BSDataType *BSData)
{
	if(!BSData->bD1D2Ready || !BSData->bND1ND2Ready)
		return BlackSholesVanna(BSData);

	BSData->Vanna = -BSData->ExpYield*BSData->NN1*BSData->D2/BSData->Volatility;

	return BSData->Vanna;
}

/********************************************
********************************************/
double _BlackSholesCharm(BSDataType *BSData)
{
	if(!BSData->bD1D2Ready || !BSData->bND1ND2Ready)
		return BlackSholesCharm(BSData);

	if(strstr(BSData->OptType, "CALL"))
		BSData->Charm = -BSData->Yield*BSData->ExpYield*BSData->CND1 + 
						BSData->ExpYield*BSData->NN1*(2*(BSData->RiskFreeRate - BSData->Yield)*BSData->YearsToExp - 
														BSData->D2*BSData->Dt)/(2*BSData->YearsToExp*BSData->Dt);
	else
		BSData->Charm = BSData->Yield*BSData->ExpYield*BSData->CND1 + 
						BSData->ExpYield*BSData->NN1*(2*(BSData->RiskFreeRate - BSData->Yield)*BSData->YearsToExp - 
														BSData->D2*BSData->Dt)/(2*BSData->YearsToExp*BSData->Dt);
	return BSData->Charm;
}

double _BlackSholesSpeed(BSDataType *BSData)
{
	if(!BSData->bD1D2Ready || !BSData->bND1ND2Ready)
		return BlackSholesSpeed(BSData);

	BSData->Speed = -BSData->ExpYield*BSData->NN1*(BSData->D1/BSData->Dt + 1)/(BSData->Price*BSData->Price*BSData->Dt);

	return BSData->Speed;
}

double _BlackSholesZomma(BSDataType *BSData)
{
	if(!BSData->bD1D2Ready || !BSData->bND1ND2Ready)
		return BlackSholesZomma(BSData);

	BSData->Zomma = BSData->ExpYield*BSData->NN1*(BSData->D1*BSData->D2 - 1)/(BSData->Price*BSData->Volatility*BSData->Dt);

	return BSData->Zomma;
}

double _BlackSholesColor(BSDataType *BSData)
{
	if(!BSData->bD1D2Ready || !BSData->bND1ND2Ready)
		return BlackSholesColor(BSData);

	BSData->Color = -BSData->ExpYield*BSData->NN1/(2*BSData->Price*BSData->YearsToExp*BSData->Dt);

	return BSData->Color;
}

double _BlackSholesDvegaDtime(BSDataType *BSData)
{
	if(!BSData->bD1D2Ready || !BSData->bND1ND2Ready)
		return BlackSholesDvegaDtime(BSData);

	BSData->DvegaDtime = BSData->Price*BSData->ExpYield*BSData->NN1*sqrt(BSData->YearsToExp)*(BSData->Yield + 
							(BSData->RiskFreeRate - BSData->Yield)*BSData->D1/BSData->Dt - 
							(1 + BSData->D1*BSData->D2)/2*BSData->YearsToExp);

	return BSData->DvegaDtime;
}

double _BlackSholesVomma(BSDataType *BSData)
{
	if(!BSData->bD1D2Ready || !BSData->bND1ND2Ready)
		return BlackSholesVomma(BSData);

	BSData->Vomma = BSData->Price*BSData->ExpYield*BSData->NN1*sqrt(BSData->YearsToExp)*BSData->D1*BSData->D2/BSData->Volatility;
	
	return BSData->Vomma;
}

/********************************************
********************************************/
double BlackSholesImpliedVolatility(BSDataType *BSData)
{
	// check for arbitrage violations:
	// if price at almost zero volatility greater than price, return 0
	double VolHigh = 5;
	double VolLow = 0;
	double TargetOptPrice;

	TargetOptPrice = BSData->OptPrice;

	do {
		BSData->Volatility = (VolHigh + VolLow)/2;
		BlackSholesOptionPrice(BSData);

		if(BSData->OptPrice > TargetOptPrice)
			VolHigh = (VolHigh + VolLow)/2;
		else
			VolLow = (VolHigh + VolLow)/2;
	} while ((VolHigh - VolLow) > 0.000001);

	BSData->OptPrice = TargetOptPrice;
	BSData->Volatility = (VolHigh + VolLow)/2;

	return BSData->Volatility;
}

/********************************************
********************************************/
double BlackSholesOptionPrice(BSDataType *BSData)
{
	BlackSholesD1D2(BSData);
		
	if(strstr(BSData->OptType, "CALL"))
	{
		if(BSData->Yield >= 0)
			BSData->OptPrice = BSData->Price*BSData->ExpYield*CSND(BSData->D1) - 
								BSData->Strike*BSData->ExpRiskFreeRate*CSND(BSData->D2);
		else
			BSData->OptPrice = BSData->Price*CSND(BSData->D1) - 
								BSData->Strike*BSData->ExpRiskFreeRate*CSND(BSData->D2);
	}
	else
	{
		if(BSData->Yield >= 0)
			BSData->OptPrice = BSData->Strike*BSData->ExpRiskFreeRate*CSND(-BSData->D2) - 
								BSData->Price*BSData->ExpYield*CSND(-BSData->D1);
		else
			BSData->OptPrice = BSData->Strike*BSData->ExpRiskFreeRate*CSND(-BSData->D2) - 
								BSData->Price*CSND(-BSData->D1);
	}

	return BSData->OptPrice;
}

/********************************************
********************************************/
double BlackSholes(BSDataType *BSData)
{
	if(strstr(BSData->OptType, "CALL") || strstr(BSData->OptType, "PUT"))
	{
		BSData->bD1D2Ready = 0;
		BSData->bND1ND2Ready = 0;

		if(BSData->Price < 0 || BSData->Strike < 0)
			return -1.0;
		
		if(BSData->OptPrice == 0.0 && (BSData->Volatility == 0.0 || round(BSData->Volatility, 4) == 0))
			BSData->Volatility = 0.2; // If OptPrice is zero, and volatility is zero, then set volatility 20%.

		BSData->YearsToExp = ((double) BSData->DaysToExp)/365.0;

		if(BSData->OptPrice < 0 && BSData->Volatility <= 0) // use 20% volatility as default
			BSData->Volatility = .20;

		if(BSData->OptPrice < 0)
			BSData->OptPrice = BlackSholesOptionPrice(BSData);
		else
			BSData->Volatility = BlackSholesImpliedVolatility(BSData);

		BlackSholesND1ND2(BSData);
		_BlackSholesDelta(BSData);
		_BlackSholesDelta2(BSData);
		_BlackSholesGamma(BSData);
		_BlackSholesGamma2(BSData);
		_BlackSholesTheta(BSData);
		_BlackSholesVega(BSData);
		_BlackSholesRho(BSData);
		_BlackSholesRho2(BSData);
		_BlackSholesVanna(BSData);
		_BlackSholesCharm(BSData);
		_BlackSholesSpeed(BSData);
		_BlackSholesZomma(BSData);
		_BlackSholesColor(BSData);
		_BlackSholesDvegaDtime(BSData);
		_BlackSholesVomma(BSData);
	}

	return BSData->OptPrice;
}

/********************************************
********************************************/
void BlackSholesSetup(BSDataType *BSData, const char *OptType, char Dir, const char *AssetType, 
					  double Price, double Strike, int DaysToExp, double RiskFreeRate, 
					  double Yield, double Volatility, double OptPrice)
{
	SaveCopy(BSData->OptType, sizeof(BSData->OptType), OptType, 0);
	BSData->Dir = Dir;
	SaveCopy(BSData->AssetType, sizeof(BSData->AssetType), AssetType, 0);
	BSData->Price = Price;
	BSData->Strike = Strike;
	BSData->DaysToExp = DaysToExp;
	BSData->RiskFreeRate = RiskFreeRate;
	BSData->Yield = Yield;
	BSData->Volatility = Volatility;
	if(strstr(BSData->AssetType, "FUTURE") || strstr(BSData->AssetType, "ZERO COUPON")) // Make sure yield to be zero.
		BSData->Yield = 0;
	BSData->OptPrice = OptPrice;
	BSData->Delta = 0;
	BSData->Gamma = 0;
	BSData->Theta = 0;
	BSData->Vega = 0;
	BSData->Rho = 0;
	BSData->Rho2 = 0;
	BSData->Vanna = 0;
	BSData->Charm = 0;
	BSData->Speed = 0;
	BSData->Zomma = 0;
	BSData->Color = 0;
	BSData->DvegaDtime = 0;
	BSData->Vomma = 0;
	BSData->bD1D2Ready = 0;
	BSData->bND1ND2Ready = 0;
}
