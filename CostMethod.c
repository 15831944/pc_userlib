#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "asset.h"
#include "costmethod.h"
#include "gen_fun.h"
#include "floatrepo.h"
#include "USERINFO.H"

CostNodeType CostDataArray[2000];

int COSTMETHOD_OPT = 0;
int COSTMETHOD_CASH = 1;
int COSTMETHOD_LEVERAGE = 2;
int COSTMETHOD_REPO = 3;
int COSTMETHOD_IRS = 4;
int COSTMETHOD_FUTU = 5;
int COSTMETHOD_FXPVALUE = 6;
int COSTMETHOD_SECURITIES = 7;
int COSTMETHOD_CDS = 8;

long CostFIFODaysOfHolding();
int CostGetTransType(CostDataType *CostData, const char *TransType);

/*****************************************
*****************************************/
double CostComputeAmount(CostType *CostRes, CostDataType *CostData)
{
	return (CostData->Dir == 'P' ? -1: 1)*CostData->NomAmount*CostData->Amort*
			CostData->Price*CostRes->BondFact/CostData->Fxrate;
}
/*****************************************
*****************************************/
int CostGetTransType(CostDataType *CostData, const char *TransType)
{
	CostData->TransType = 0;
	if(strstr(TransType, "CASH"))
		CostData->TransType = 1;
	else
		if(strstr(TransType, "LEVERAGE"))
			CostData->TransType = 2;
		else
			if(strstr(TransType, "REPO"))
				CostData->TransType = 3;
			else
				if(strstr(TransType, "INT. SWAP"))
					CostData->TransType = 4;
				else
					if(strstr(TransType, "FUTU COST"))
						CostData->TransType = 5;
					else
						if(strstr(TransType, "FX PVALUE"))
							CostData->TransType = 6;
						else
							if(strstr(TransType, "SECURITIES"))
								CostData->TransType = 7;
							else
								if(strstr(TransType, "CDS"))
									CostData->TransType = 8;

	return CostData->TransType;
}
/*****************************************
*****************************************/
void CostUpdateDir(CostType *CostRes, char Dir)
{
	switch(Dir)
	{
		case 'P':
			CostRes->nRunDir = 1;
			break;
		case 'S':
			CostRes->nRunDir = -1;
			break;
		default:
			CostRes->nRunDir = 0;
			break;
	}
}
/*****************************************
*****************************************/
void CostPriceAdjustment(CostDataType *CostData, long OptTransNum, double OptPrice, double OptFxrate, char OptDir)
{
	if(CostData->TransType >= COSTMETHOD_SECURITIES) // Securities
	{
		if(OptTransNum != 0)
		{
			if(OptDir == CostData->Dir)
			{
				CostData->Price += OptPrice;
				CostData->USDPrice += OptPrice/OptFxrate;
			}
			else
			{
				CostData->Price -= OptPrice;
				CostData->USDPrice -= OptPrice/OptFxrate;
			}
		}
	}
}
/*****************************************
*****************************************/
void CostInit(CostType *CostRes, double AmortFact, char Future, int bAdd)
{
	CostRes->RunDir = 0;
	CostRes->LastDir = 0;
	CostRes->Future = Future;
	CostRes->CostDataArrayIndex = 0;
	CostRes->NomAmount = 0;
	CostRes->AvPrice = 0;
	CostRes->AvUSDPrice = 0;
	CostRes->Amort = AmortFact;
	CostRes->Profit = 0;
	CostRes->FProfit = 0;
	CostRes->UnRealFProfit = 0;
	CostRes->UnRealProfit = 0;
	CostRes->RealProfit = 0;
	CostRes->RealFProfit = 0;
	CostRes->Amount = 0;
	CostRes->bAdd = bAdd;
}
/*****************************************
*****************************************/
void CostInit2(CostType *CostRes, double Market, double Exrate, double AmortFact, char Future, int bAdd)
{
	CostInit(CostRes, AmortFact, Future, bAdd);
	CostRes->Market = Market;
	CostRes->Exrate = Exrate;
}
/*****************************************
*****************************************/
void CostSetup(CostType *CostRes, CostDataType *CostData, const char *TransType, 
			   const char *AssetType, char Dir, const char *TradeDate, const char *EndDate, 
			   int TransNum,  double NomAmount, double Price, double Fxrate, double AmortFact, 
			   int OptTransNum, double OptPrice, double OptFxrate, char OptDir)
{
	CostRes->BondFact = strstr(AssetType, "BOND") ? 100 : 1;

	CostSetup2(CostRes, CostData, TransType, CostRes->BondFact, Dir, TradeDate, EndDate, TransNum,
			NomAmount, Price, Fxrate, AmortFact, OptTransNum, OptPrice, OptFxrate, OptDir);
}
/*****************************************
*****************************************/
void CostSetup2(CostType *CostRes, CostDataType *CostData, const char *TransType, 
				double Factor, char Dir, const char *TradeDate, const char *EndDate, 
				int TransNum, double NomAmount, double Price, double Fxrate, double AmortFact, 
				int OptTransNum, double OptPrice, double OptFxrate, char OptDir)
{
	CostRes->BondFact = 1/Factor;

	CostGetTransType(CostData, TransType);
	CostData->Dir = Dir;

	SaveCopy(CostData->TradeDate, sizeof(CostData->TradeDate), TradeDate, 0);
	SaveCopy(CostData->EndDate, sizeof(CostData->EndDate), EndDate, 0);

	CostData->TransNum = TransNum;
	CostData->NomAmount = NomAmount;
	CostData->Fxrate = Fxrate;
	CostData->Price = Price;
	CostData->USDPrice = Price/Fxrate;
	CostData->Amort = AmortFact;
	CostData->HoldingDays = -1;

	CostPriceAdjustment(CostData, OptTransNum, OptPrice, OptFxrate, OptDir);
	
	CostRes->Profit = 0;
	CostRes->FProfit = 0;
	CostRes->UnRealProfit = 0;
	CostRes->UnRealFProfit = 0;
	CostRes->TransType = CostData->TransType;
}
/*****************************************
*****************************************/
void CostSetup3(CostType *CostRes, CostDataType *CostData, const char *TransType, 
				double Factor, char Dir, const char *TradeDate, const char *EndDate, 
				int TransNum, double NomAmount, double Price, double Fxrate, double Market, 
				double Exrate, double AmortFact, int OptTransNum, double OptPrice, 
				double OptFxrate, char OptDir)
{
	CostSetup2(CostRes, CostData, TransType, Factor, Dir, TradeDate, EndDate, TransNum, 
				NomAmount, Price, Fxrate, AmortFact, OptTransNum, OptPrice, 
				OptFxrate, OptDir);
	
	CostRes->Market = Market;
	CostRes->Exrate = Exrate;
}
/*****************************************
*****************************************/
void CostFXSetup(CostType *CostRes, CostDataType *CostData, const char *TransType, 
				 char Dir, const char *TradeDate, int TransNum, double NomAmount, 
				 double Price, double TrFxrate, double ClFxrate, double Factor, 
				 int Booking, double AmortFact)
{
	CostRes->BondFact = 1/Factor;

	CostGetTransType(CostData, TransType);
	CostData->Dir = Dir;

	SaveCopy(CostData->TradeDate, sizeof(CostData->TradeDate), TradeDate, 0);
	CostData->EndDate[0] = 0;

	CostData->TransNum = TransNum;
	CostData->NomAmount = NomAmount;
	CostData->Fxrate = TrFxrate;
	CostData->ClFxrate = ClFxrate;
	CostData->Price = Price;
	CostData->USDPrice = Price/TrFxrate;
	CostData->Amort = AmortFact;
	CostData->HoldingDays = -1;
	CostData->Booking = Booking;

	CostRes->Profit = 0;
	CostRes->FProfit = 0;
	CostRes->UnRealProfit = 0;
	CostRes->UnRealFProfit = 0;
}
/*****************************************
*****************************************/
void CostOptionPL(CostType *CostRes, CostDataType *CostData)
{
	switch(CostData->TransType)
	{
		case 0: /* Option Expiration */
			CostRes->Profit = (CostData->Dir == 'P' ? -1: 1)*CostData->NomAmount*CostData->Amort*
								CostData->Price*CostRes->BondFact/CostData->Fxrate;
			break;
		case 1: /* Cash */
			CostRes->Profit = (CostData->NomAmount*CostData->Amort + CostData->Price);
			break;
		case 2: /* Repo/leverage */
		case 3:
			CostRes->Profit = (CostData->Price)*(CostData->Dir == 'P' ? 1 : -1);
			break;
		case 4: /* IRS */
			CostRes->Profit = CostData->Price;
			break;
		case 5: /* Future Cost */
		case 6: /* FX Preset Value */
			CostRes->Profit = (CostData->NomAmount/CostData->Fxrate);
			break;
		default:
			CostRes->Profit = 0;
			break;
	}

	CostRes->CurrNom = CostData->NomAmount;
	CostRes->FProfit = CostRes->Profit;
	CostAddPL(CostRes);
}
/*****************************************
*****************************************/
void CostAddPL(CostType *CostRes)
{
	if(CostRes->bAdd)
	{
		CostRes->RealProfit += CostRes->Profit;
		CostRes->RealFProfit += CostRes->FProfit;
	}
	else
	{
		CostRes->RealProfit = CostRes->Profit;
		CostRes->RealFProfit = CostRes->FProfit;
	}
}
/*****************************************
*****************************************/
void CostAvCostPL(CostType *CostRes, CostDataType *CostData, double NomAmount) 				
{
	double Profit;

	if(CostData->TransType == COSTMETHOD_CDS)
	{
		Profit = NomAmount*((CostData->Price -100) - (CostRes->AvPrice - 100))*
				CostRes->nRunDir*CostRes->Amort*CostRes->BondFact/CostData->Fxrate;
		CostRes->Profit += Profit;
		CostRes->FProfit += NomAmount*((CostData->Price - 100)/CostData->Fxrate - 
									(CostRes->AvPrice - 100)/CostRes->AvFxrate)*
									CostRes->nRunDir*CostRes->Amort*CostRes->BondFact;
	}
	else
	{
		if(CostRes->InfBond)
		{
			Profit = NomAmount*(CostData->Price*CostData->Amort - CostRes->AvPrice * CostRes->Amort)*
								CostRes->nRunDir*CostRes->BondFact/CostData->Fxrate;
			CostRes->Profit += Profit;
			CostRes->FProfit += NomAmount*(CostData->USDPrice*CostData->Amort - CostRes->AvUSDPrice*CostRes->Amort)*
								CostRes->nRunDir*CostRes->BondFact;
		}
		else
		{
			Profit = NomAmount*(CostData->Price - CostRes->AvPrice)*CostRes->nRunDir*
								CostRes->Amort*CostRes->BondFact/CostData->Fxrate;
			CostRes->Profit += Profit;

			if(CostRes->Future == 'F') /* Future */
				CostRes->FProfit += Profit;
			else
				CostRes->FProfit += NomAmount*(CostData->USDPrice - CostRes->AvUSDPrice)*
									CostRes->nRunDir*CostRes->Amort*CostRes->BondFact;
		}
	}
}
/*****************************************
*****************************************/
void CostFIFOPL(CostNodeType *pItem, CostType *CostRes, CostDataType *CostData)
{
	pItem->Profit = pItem->NomAmount*(CostData->Price - pItem->Price)*CostRes->nRunDir*
					CostRes->Amort*CostRes->BondFact/CostData->Fxrate;

	if(CostRes->Future == 'F')
		pItem->FProfit = pItem->Profit;
	else
		pItem->FProfit = pItem->NomAmount*(CostData->USDPrice - pItem->USDPrice)*
						CostRes->nRunDir*CostRes->Amort*CostRes->BondFact;
	CostRes->CurrNom = pItem->NomAmount;
	CostData->HoldingDays = CostFIFODaysOfHolding(pItem, CostData);
}
/*****************************************
*****************************************/
void CostFxPL(CostType *CostRes, CostDataType *CostData)
{
	switch(CostData->TransType)
	{
		case 0: // Option
		case 7: // Security
			CostRes->FProfit = (CostData->Dir == 'P' ? -1 : 1)*CostData->NomAmount*
								CostData->Price*CostRes->Amort*CostRes->BondFact*
								(1/CostData->ClFxrate - 1/CostData->Fxrate);
			break;
		case 2: // Leverage
			if(CostData->Booking)
				CostRes->FProfit = (CostData->Dir == 'P' ? 1 : -1)*CostData->NomAmount*
									CostData->Price*CostRes->Amort*CostRes->BondFact*
									(1/CostData->ClFxrate - 1/CostData->Fxrate);
			else
				CostRes->FProfit = (CostData->Dir == 'P' ? -1 : 1)*CostData->NomAmount*
									CostData->Price*CostRes->Amort*CostRes->BondFact*
									(1/CostData->ClFxrate - 1/CostData->Fxrate);
			break;
		case 3: // Repo
			if(CostData->Booking)
				CostRes->FProfit = (CostData->Dir == 'P' ? -1 : 1)*CostData->NomAmount*
									CostData->Price*CostRes->Amort*CostRes->BondFact*
									(1/CostData->ClFxrate - 1/CostData->Fxrate);
			else
				CostRes->FProfit = (CostData->Dir == 'P' ? 1 : -1)*CostData->NomAmount*
									CostData->Price*CostRes->Amort*CostRes->BondFact*
									(1/CostData->ClFxrate - 1/CostData->Fxrate);
			break;
		case 4: // IRS
			break;
		case 8: // CDS
			CostRes->FProfit = (CostData->Dir == 'P' ? 1 : -1)*CostData->NomAmount*
								(100 - CostData->Price)*CostRes->Amort*CostRes->BondFact*
								(1/CostData->ClFxrate - 1/CostData->Fxrate);
			break;
		default:
			CostRes->FProfit = (CostData->Dir == 'P' ? -1 : 1)*CostData->NomAmount*
								CostData->Price*CostRes->Amort*CostRes->BondFact*
								(1/CostData->ClFxrate - 1/CostData->Fxrate);
			break;
	}
	
	CostAddPL(CostRes);
}
/*****************************************
*****************************************/
void CostAvCostMethod(CostType *CostRes, CostDataType *CostData)
{
	if(CostData->TransType < COSTMETHOD_SECURITIES)
	{
		CostOptionPL(CostRes, CostData);
		return;
	}

	if(fabs(CostRes->NomAmount) <= 0.000001)
	{
		CostRes->NomAmount = CostData->NomAmount;
		CostRes->AvPrice= CostData->Price;
		CostRes->RunDir = CostData->Dir;
		CostRes->AvUSDPrice = CostData->USDPrice;
		CostRes->Amort = CostData->Amort;
		CostUpdateDir(CostRes, CostRes->RunDir);
		
		if(CostData->TransType == COSTMETHOD_CDS)
			CostRes->AvFxrate = CostData->Fxrate;
		if(CostData->Dir == 'N')
			CostRes->RunDir = 0;
		return;
	}

	if(!CostRes->InfBond)
	{
		if((CostRes->Amort != CostData->Amort) && CostRes->NomAmount != 0)
		{
			if(CostRes->Amort > CostData->Amort)
			{
				if(CostRes->Future == 'F')
				{
					CostRes->FProfit = CostRes->nRunDir*CostRes->NomAmount*CostRes->BondFact*
										(CostRes->Amort - CostData->Amort)*(100 - CostRes->AvPrice)/CostData->Fxrate;
					CostRes->Profit = CostRes->FProfit;
				}
				else
				{
					CostRes->FProfit = CostRes->nRunDir*CostRes->NomAmount*CostRes->BondFact*
										(CostRes->Amort - CostData->Amort)*(100/CostData->Fxrate - CostRes->AvUSDPrice);
					CostRes->Profit = CostRes->nRunDir*CostRes->NomAmount*CostRes->BondFact*
										(CostRes->Amort - CostData->Amort)*(100 - CostRes->AvPrice)/CostData->Fxrate;
				}
			}
			else 
				if(CostRes->Amort < CostData->Amort)
				{
					CostRes->AvPrice = (CostRes->Amort*CostRes->AvPrice + (CostData->Amort - CostRes->Amort)*100)/CostData->Amort;
					CostRes->AvUSDPrice = (CostRes->Amort*CostRes->AvUSDPrice + 
											(CostData->Amort - CostRes->Amort)*100/CostData->Fxrate)/CostData->Amort;
				}

			CostRes->Amort = CostData->Amort;
		
			if(CostData->NomAmount == 0)
			{
				CostAddPL(CostRes);
				return;
			}
		}
	}

	if(CostData->Dir == 'N') /* take trans_direction as runing dir */
	{          /* this is used for amortization computation */
		CostData->Dir = CostRes->RunDir;
		return;
	}

	if(CostRes->RunDir == CostData->Dir)
	{
		if(CostData->TransType == COSTMETHOD_CDS)
		{
			if(CostData->Price == 100 && CostRes->AvPrice == 100)
			{
				CostRes->AvFxrate = (CostData->NomAmount + CostRes->NomAmount)/
									(CostData->NomAmount/CostData->Fxrate + 
									 CostRes->NomAmount/CostRes->AvFxrate);
			}
			else
			{
				CostRes->AvFxrate = ((CostData->Price - 100)*CostData->NomAmount + 
									CostRes->NomAmount*(CostRes->AvPrice - 100))/
									((CostData->Price - 100)*CostData->NomAmount/CostData->Fxrate + 
									CostRes->NomAmount*(CostRes->AvPrice - 100)/CostRes->AvFxrate);
			}
		}
		else
			if(CostRes->InfBond)
			{
				CostRes->AvPrice = (CostData->Price*CostData->NomAmount*CostData->Amort + CostRes->AvPrice*CostRes->NomAmount*CostRes->Amort)/
									((CostData->NomAmount + CostRes->NomAmount)*CostData->Amort);
				CostRes->AvUSDPrice = (CostData->USDPrice*CostData->NomAmount*CostData->Amort + CostRes->AvUSDPrice*CostRes->NomAmount*CostRes->Amort)/
										((CostData->NomAmount + CostRes->NomAmount)*CostData->Amort);
				CostRes->Amort = CostData->Amort;
			}
			else
			{
				CostRes->AvPrice = (CostData->Price*CostData->NomAmount + CostRes->AvPrice*CostRes->NomAmount)/
									(CostData->NomAmount + CostRes->NomAmount);
				CostRes->AvUSDPrice = (CostData->USDPrice*CostData->NomAmount + CostRes->AvUSDPrice*CostRes->NomAmount)/
								(CostData->NomAmount + CostRes->NomAmount);
			}

			CostRes->NomAmount += CostData->NomAmount;
			return;
	}

	CostUpdateDir(CostRes, CostRes->RunDir);

	if(CostData->NomAmount >= CostRes->NomAmount)
	{
		CostAvCostPL(CostRes, CostData, CostRes->NomAmount);
		CostRes->NomAmount = CostData->NomAmount - CostRes->NomAmount;
		if(CostRes->NomAmount > 0)
		{
			CostRes->RunDir = CostData->Dir;
			CostRes->AvPrice = CostData->Price;
			CostRes->AvUSDPrice = CostData->USDPrice;
			CostRes->Amort = CostData->Amort;
			if(CostData->TransType == COSTMETHOD_CDS)
				CostRes->AvFxrate = CostData->Fxrate;
		}
		else
			CostRes->NomAmount = 0.0;
	}
	else 
	{
		CostAvCostPL(CostRes, CostData, CostData->NomAmount);
		CostRes->NomAmount -= CostData->NomAmount;
	}
	
	CostAddPL(CostRes);
}
/*****************************************
*****************************************/
void CostUnRealPL(CostType *CostRes)
{
	CostUnRealPL2(CostRes, CostRes->Amort);
}
/*****************************************
*****************************************/
void CostUnRealPL2(CostType *CostRes, double Amort)
{
	CostUpdateDir(CostRes, CostRes->RunDir);

	if(CostRes->NomAmount != 0)
	{
		CostRes->Amount = CostRes->nRunDir*CostRes->NomAmount*CostRes->Market*
						  Amort*CostRes->BondFact/CostRes->Exrate;

		if(CostRes->TransType == COSTMETHOD_CDS)
		{
			CostRes->UnRealProfit = CostRes->nRunDir*CostRes->NomAmount*Amort*
									((CostRes->Market -100) - (CostRes->AvPrice - 100))*
									CostRes->BondFact/CostRes->AvFxrate;
			CostRes->UnRealFProfit = CostRes->nRunDir*CostRes->NomAmount*Amort*
								((CostRes->Market - 100)/CostRes->Exrate - 
								(CostRes->AvPrice - 100)/CostRes->AvFxrate)*
								CostRes->BondFact;
		}
		else
		{
			if(CostRes->InfBond)
			{
				CostRes->UnRealProfit = CostRes->nRunDir*CostRes->NomAmount*CostRes->BondFact*
										(CostRes->Market*Amort - CostRes->AvPrice*CostRes->Amort)/CostRes->Exrate;
				CostRes->UnRealFProfit = CostRes->nRunDir*CostRes->NomAmount*CostRes->BondFact*
										(CostRes->Market*Amort/CostRes->Exrate - CostRes->AvUSDPrice*CostRes->Amort);
			}
			else
			{
				CostRes->UnRealProfit = CostRes->nRunDir*CostRes->NomAmount*Amort*CostRes->BondFact*
										(CostRes->Market - CostRes->AvPrice)/CostRes->Exrate;
				if(CostRes->Future == 'F')
					CostRes->UnRealFProfit = CostRes->UnRealProfit;
				else
					CostRes->UnRealFProfit = CostRes->nRunDir*CostRes->NomAmount*Amort*CostRes->BondFact*
												(CostRes->Market/CostRes->Exrate - CostRes->AvUSDPrice);
			}
		}
	}
	else
	{
		CostRes->UnRealProfit = 0;
		CostRes->UnRealFProfit = 0;
		CostRes->Amount = 0;
	}
}
/*****************************************
*****************************************/
void CostAssignEntry(CostNodeType *pItem, CostDataType *CostData)
{
	SaveCopy(pItem->TradeDate, sizeof(pItem->TradeDate), CostData->TradeDate, 0);
	SaveCopy(pItem->EndDate, sizeof(pItem->EndDate), CostData->EndDate, 0);
	pItem->TransNum = CostData->TransNum;
	pItem->Price = CostData->Price;
	pItem->Fxrate = CostData->Fxrate;
	pItem->USDPrice = CostData->USDPrice;
	pItem->Dir = CostData->Dir;
	pItem->NomAmount = 0;
	pItem->Remain = CostData->NomAmount;
	pItem->Amort = CostData->Amort;
	pItem->Profit = 0;
	pItem->FProfit = 0;
	pItem->UnRealPL = 0;
	pItem->UnRealFPL = 0;
	pItem->Addable = 1;
}
/*****************************************
*****************************************/
void CostCopyEntry(CostNodeType *pItem1, CostNodeType *pItem2)
{
	memcpy(pItem1, pItem2, sizeof(*pItem2));
}
/*****************************************
*****************************************/
int CostAddEntry(CostType *CostRes, CostNodeType *pDataArray, CostNodeType *pItem)
{
	if(CostRes->CostDataArrayIndex < 2000)
	{
		CostCopyEntry(pDataArray + CostRes->CostDataArrayIndex, pItem);
		CostRes->CostDataArrayIndex ++;
		return CostRes->CostDataArrayIndex;
	}

	return -1;
}
/*****************************************
*****************************************/
int CostCountEntry(CostType *CostRes, CostNodeType *pCostDataArray, char Dir)
{
	int i, count = 0;

	for(i = 0; i < CostRes->CostDataArrayIndex; i ++)
	{
		if(pCostDataArray[i].Dir != Dir && pCostDataArray[i].Remain > 0)
			count++;
	}

	return count;
}
/*****************************************
*****************************************/
int CostFindEntry(CostType *CostRes, CostNodeType *pDataArray, char Dir)
{
	int i;

	for(i = 0; i < CostRes->CostDataArrayIndex; i ++)
	{
		if(pDataArray[i].Dir != Dir && pDataArray[i].Remain > 0)
			return i;
	}

	return -1;
}
/*****************************************
*****************************************/
void CostAdjustAmortFact(CostType *CostRes, int bFIFO, double Amort, double Fxrate, double NomAmount)
{
	int i;

	if(CostRes->Amort == Amort)
		return;

	if(bFIFO) 
	{
		for(i = 0; i < CostRes->CostDataArrayIndex; i ++)
		{
			if(CostDataArray[i].Amort != Amort)
			{
				if(CostDataArray[i].Amort > Amort)
				{
					if(CostRes->Future == 'F')
					{
						CostDataArray[i].FProfit += (CostDataArray[i].Dir == 'P' ? 1 : -1)*
									CostDataArray[i].Remain*(CostDataArray[i].Amort - Amort)*
									(100 - CostDataArray[i].Price)*CostRes->BondFact/Fxrate;
						CostDataArray[i].Profit = CostDataArray[i].FProfit;
					}
					else
					{
						CostDataArray[i].FProfit += (CostDataArray[i].Dir == 'P' ? 1 : -1)*
									CostDataArray[i].Remain*(CostDataArray[i].Amort - Amort)*
									(100/Fxrate - CostDataArray[i].USDPrice)*CostRes->BondFact;
						CostDataArray[i].Profit += (CostDataArray[i].Dir == 'P' ? 1 : -1)*
									CostDataArray[i].Remain*(CostDataArray[i].Amort - Amort)*
									(100 - CostDataArray[i].Price)*CostRes->BondFact/Fxrate;
					}
				}
				else
					if(CostDataArray[i].Amort < Amort)
						CostDataArray[i].Price = (CostDataArray[i].Amort*CostDataArray[i].Price +
													(Amort - CostDataArray[i].Amort)*100)/Amort;

				CostDataArray[i].Amort = Amort;
			}
		}
	}
	else
	{
		if((CostRes->Amort != Amort) && CostRes->NomAmount != 0)
		{
			if(CostRes->Amort > Amort)
			{
				if(CostRes->Future == 'F')
				{
					CostRes->FProfit = CostRes->nRunDir*CostRes->NomAmount*CostRes->BondFact*
										(CostRes->Amort - Amort)*
										(100 - CostRes->AvPrice)/Fxrate;
					CostRes->Profit = CostRes->FProfit;
				}
				else
				{
					CostRes->FProfit = CostRes->nRunDir*CostRes->NomAmount*CostRes->BondFact*
										(CostRes->Amort - Amort)*(100/Fxrate - CostRes->AvUSDPrice);
					CostRes->Profit = CostRes->nRunDir*CostRes->NomAmount*CostRes->BondFact*
										(CostRes->Amort - Amort)*(100 - CostRes->AvPrice)/Fxrate;
				}
			}
			else 
				if(CostRes->Amort < Amort)
				{
					CostRes->AvPrice = (CostRes->Amort*CostRes->AvPrice + 
										(Amort - CostRes->Amort)*100)/Amort;
					CostRes->AvUSDPrice = (CostRes->Amort*CostRes->AvUSDPrice + 
											(Amort - CostRes->Amort)*100/Fxrate)/Amort;
				}

			CostRes->Amort = Amort;
		
			if(NomAmount == 0)
			{
				CostAddPL(CostRes);
				return;
			}
		}
	}
}
/*****************************************
*****************************************/
long CostFIFODaysOfHolding(CostNodeType *pItem, CostDataType *CostData)
{
	DateType FmsDate;

	FmsDateSetup(&FmsDate, pItem->TradeDate, pItem->EndDate, "A/365", 0);
           
	return FmsDateGetDays(&FmsDate, CostData->TradeDate);
}
/*****************************************
*****************************************/
int CostFIFOMethodDetail(CostType *CostRes, CostDataType *CostData)
{
	int Index;
	int bMore = 0;
	CostNodeType Item;

	CostRes->Profit = 0;
	CostRes->FProfit = 0;

	Index = CostFindEntry(CostRes, CostDataArray, CostData->Dir);
	
	if(Index < 0)
	{
		CostAssignEntry(&Item, CostData);
		CostRes->CostDataArrayIndex = 0;
		CostAddEntry(CostRes, CostDataArray, &Item);
		CostRes->LastDir = CostData->Dir;
		Item.Addable = 0;
		return 0;
	}

	CostCopyEntry(&Item, CostDataArray + Index);
	
	/* Short sale is short term gain */
	if(CostData->NomAmount >= Item.Remain)
	{
		Item.NomAmount = Item.Remain;
		CostFIFOPL(&Item, CostRes, CostData);
		CostData->NomAmount -= Item.NomAmount;
		Item.Remain = 0;

		CostCopyEntry(CostDataArray + Index, &Item); /* Update Data Array */
		if(CostData->NomAmount > 0)
			bMore = 1;
		else
		{
			if(CostCountEntry(CostRes, CostDataArray, CostData->Dir) == 0)
			{
				CostRes->CostDataArrayIndex = 0;
				CostRes->LastDir = 0;
			}
		}
	}
	else    // if amount < item.remain
	{
		Item.NomAmount = CostData->NomAmount;
		CostFIFOPL(&Item, CostRes, CostData);
		Item.Remain -= CostData->NomAmount;

		CostCopyEntry(CostDataArray + Index, &Item); /* Update Data Array */
		CostData->NomAmount = 0;
	}

	if(Item.Addable)
	{
		CostRes->AvPrice = Item.Price;
		CostRes->AvUSDPrice = Item.USDPrice;
		CostRes->Profit = Item.Profit;
		CostRes->FProfit = Item.FProfit;
	}

	CostAddPL(CostRes);
	return bMore;
}
/*****************************************
*****************************************/
int CostFIFOMethod(CostType *CostRes, CostDataType *CostData)
{
	CostNodeType Item;

	if(CostData->TransType < COSTMETHOD_SECURITIES)
	{
		CostOptionPL(CostRes, CostData);
		return 0;
	}
	
	CostRes->NomAmount += (CostData->Dir == 'P' ? 1 : -1)*CostData->NomAmount;

	if(CostRes->LastDir == 0 || 
		(CostRes->LastDir == CostData->Dir && (CostData->Dir == 'S' || CostData->Dir == 'P')))
	{
		CostAssignEntry(&Item, CostData);
		CostAddEntry(CostRes, CostDataArray, &Item);
		CostRes->LastDir = CostData->Dir;
		Item.Profit = 0;
		Item.Addable = 0;
		return 0;
	}
	
	if(CostRes->Amort != CostData->Amort)
	{
		CostAdjustAmortFact(CostRes, 1, CostData->Amort, CostData->Fxrate, CostData->NomAmount);
		CostRes->Amort = CostData->Amort;
	}

	CostUpdateDir(CostRes, CostRes->LastDir);
	
	if(CostRes->LastDir != CostData->Dir && 
		(CostRes->LastDir == 'S' || CostRes->LastDir == 'P') && 
		(CostData->Dir == 'P' || CostData->Dir == 'S'))
		return CostFIFOMethodDetail(CostRes, CostData);

	return 0;
}
/*****************************************
*****************************************/
void CostFIFOAvCostMethod(CostType *CostRes, CostDataType *CostData)
{
	int i;
	int bInit = 0;
	CostNodeType *pItem;
	CostDataType NewCostData;
	CostType NewCostRes;

	CostInit(&NewCostRes, CostRes->Amort, CostRes->Future, CostRes->bAdd);

	for(i = 0; i < CostRes->CostDataArrayIndex; i ++)
	{
		pItem = &CostDataArray[i];

		if((pItem->Dir == 'P' || pItem->Dir == 'S') && pItem->Remain > 0)
		{
			if(!bInit)
			{
				memcpy(&NewCostData, CostData, sizeof(NewCostData));
				bInit = 1;
			}

			NewCostData.Dir = pItem->Dir;
			NewCostData.TransNum = CostData->TransNum;
			NewCostData.NomAmount = pItem->Remain;
			NewCostData.Price = pItem->Price;
			NewCostData.Fxrate = pItem->Fxrate;
			NewCostData.USDPrice = pItem->USDPrice;
			NewCostData.Amort = pItem->Amort;
			CostAvCostMethod(&NewCostRes, &NewCostData);
		}
	}
	
	CostRes->RunDir = NewCostRes.RunDir;
	CostRes->AvPrice = NewCostRes.AvPrice;
	CostRes->AvUSDPrice = NewCostRes.AvUSDPrice;
	CostRes->NomAmount = NewCostRes.NomAmount;
	CostUpdateDir(CostRes, CostRes->RunDir);
}
/*****************************************
*****************************************/
double CostMEPLAdjustment(CostNodeType *pItem, CostType *CostRes, const char *PrevDate, 
						  double Amort, double PrevPrice, double PrevFxrate, 
						  double *AdjPL, double *AdjFxPL)
{
	DateType FmsDate;	

	FmsDateSetup(&FmsDate, pItem->TradeDate, pItem->EndDate, "A/360", 0);
	if(FmsDateCompareDate(&FmsDate, PrevDate) <= 0)
	{
		*AdjPL = -1*pItem->NomAmount*(PrevPrice - pItem->Price)*CostRes->nRunDir*
					CostRes->Amort*CostRes->BondFact/PrevFxrate;
		if(CostRes->Future == 'F')
			*AdjFxPL = *AdjPL;
		else
			*AdjFxPL = -1*pItem->NomAmount*(PrevPrice/PrevFxrate - pItem->USDPrice)*
						CostRes->nRunDir*CostRes->Amort*CostRes->BondFact;
	}

	return *AdjPL;
}
/*****************************************
*****************************************/
int CostFIFOMethodNav(CostType *CostRes, CostDataType *CostData, const char *PrevDate, 
					  double PrevAmort, double PrevPrice, double PrevFxrate)
{
	CostNodeType Item;

	if(CostData->TransType < COSTMETHOD_SECURITIES)
	{
		CostOptionPL(CostRes, CostData);
		return 0;
	}

	CostRes->NomAmount += (CostData->Dir == 'P' ? 1 : -1)*CostData->NomAmount;

	if(CostRes->LastDir == 0 || 
		(CostRes->LastDir == CostData->Dir && (CostData->Dir == 'S' || CostData->Dir == 'P')))
	{
		CostAssignEntry(&Item, CostData);
		CostAddEntry(CostRes, CostDataArray, &Item);
		CostRes->LastDir = CostData->Dir;
		Item.Profit = 0;
		Item.Addable = 0;
		return 0;
	}
	
	if(CostRes->Amort != CostData->Amort)
	{
		CostAdjustAmortFact(CostRes, 1, CostData->Amort, CostData->Fxrate, CostData->NomAmount);
		CostRes->Amort = CostData->Amort;
	}

	CostUpdateDir(CostRes, CostRes->LastDir);
	
	if(CostRes->LastDir != CostData->Dir && 
		(CostRes->LastDir == 'S' || CostRes->LastDir == 'P') && 
		(CostData->Dir == 'P' || CostData->Dir == 'S'))
		return CostFIFOMethodNavDetail(CostRes, CostData, PrevDate, PrevAmort, PrevPrice, PrevFxrate);

	return 0;
}
/*****************************************
*****************************************/
int CostFIFOMethodNavDetail(CostType *CostRes, CostDataType *CostData, const char *PrevDate, 
							double PrevAmort, double PrevPrice, double PrevFxrate)
{
	int Index, bMore = 0, bComp1, bComp2;
	double AdjPL = 0, AdjFxPL = 0;
	CostNodeType Item;
	DateType FmsDate, FmsDate2;

	CostRes->Profit = 0;
	CostRes->FProfit = 0;

	Index = CostFindEntry(CostRes, CostDataArray, CostData->Dir);
	
	if(Index < 0)
	{
		CostAssignEntry(&Item, CostData);
		CostRes->CostDataArrayIndex = 0;
		CostAddEntry(CostRes, CostDataArray, &Item);
		CostRes->LastDir = CostData->Dir;
		Item.Addable = 0;
		return 0;
	}

	CostCopyEntry(&Item, CostDataArray + Index);
	/* Short sale is short term gain */

	FmsDateSetup(&FmsDate, Item.TradeDate, Item.EndDate, "A/360", 0); // Entry date vs last nav date
	bComp1 = FmsDateCompareDate(&FmsDate, PrevDate);

	FmsDateSetup(&FmsDate2, CostData->TradeDate, CostData->EndDate, "A/360", 0); // Unwind date vs last nav date
	bComp2 = FmsDateCompareDate(&FmsDate2, PrevDate);

	if(CostData->NomAmount >= Item.Remain)
	{
		Item.NomAmount = Item.Remain;
		CostFIFOPL(&Item, CostRes, CostData);
		if(bComp1 <= 0) // Entry Before prev month end
		{
			if(bComp2 > 0) // Unwind Trade date after prev month end
				CostMEPLAdjustment(&Item, CostRes, PrevDate, PrevAmort, PrevPrice, 
									PrevFxrate, &AdjPL, &AdjFxPL);
			else // Unwind Trade date after prev month end
			{
				Item.Profit = 0;
				Item.FProfit = 0;
			}
		}

		CostData->NomAmount -= Item.NomAmount;
		Item.Remain = 0;

		CostCopyEntry(CostDataArray + Index, &Item); /* Update Data Array */
		if(CostData->NomAmount > 0)
			bMore = 1;
		else
		{
			if(CostCountEntry(CostRes, CostDataArray, CostData->Dir) == 0)
			{
				CostRes->CostDataArrayIndex = 0;
				CostRes->LastDir = 0;
			}
		}
	}
	else    // if amount < item.remain
	{
		Item.NomAmount = CostData->NomAmount;
		CostFIFOPL(&Item, CostRes, CostData);
		if(bComp1 <= 0) // Entry Before prev month end
		{
			if(bComp2 > 0) // Unwind Trade date after prev month end
				CostMEPLAdjustment(&Item, CostRes, PrevDate, PrevAmort, PrevPrice, 
									PrevFxrate, &AdjPL, &AdjFxPL);
			else // Unwind Trade date after prev month end
			{
				Item.Profit = 0;
				Item.FProfit = 0;
			}
		}

		Item.Remain -= CostData->NomAmount;

		CostCopyEntry(CostDataArray + Index, &Item); /* Update Data Array */
		CostData->NomAmount = 0;
	}

	if(Item.Addable)
	{
		CostRes->AvPrice = Item.Price;
		CostRes->AvUSDPrice = Item.USDPrice;
		Item.Profit += AdjPL;
		Item.FProfit += AdjFxPL;
		CostRes->Profit = Item.Profit;
		CostRes->FProfit = Item.FProfit;		
	}

	return bMore;
}
/*****************************************
*****************************************/
void CostFIFOAvCostNavMethod(CostType *CostRes, CostDataType *CostData, const char *PrevDate, 
							 double PrevPrice, double PrevFxrate)
{
	int i;
	int bInit = 0, bPrev;
	CostNodeType *pItem;
	CostDataType NewCostData;
	CostType NewCostRes;
	DateType FmsDate;

	CostInit(&NewCostRes, CostRes->Amort, CostRes->Future, CostRes->bAdd);
	for(i = 0; i < CostRes->CostDataArrayIndex; i ++)
	{
		pItem = &CostDataArray[i];
		if(pItem->Remain > 0)
		{
			if(!bInit)
			{
				memcpy(&NewCostData, CostData, sizeof(NewCostData));
				bInit = 1;
			}

			FmsDateSetup(&FmsDate, pItem->TradeDate, pItem->EndDate, "A/360", 0);
			bPrev = FmsDateCompareDate(&FmsDate, PrevDate);

			NewCostData.Dir = pItem->Dir;
			NewCostData.TransNum = CostData->TransNum;
			NewCostData.NomAmount = pItem->Remain;
			NewCostData.Amort = pItem->Amort;

			if(bPrev < 0)
			{
				NewCostData.Price = PrevPrice;
				NewCostData.USDPrice = PrevPrice/PrevFxrate;
				NewCostData.Fxrate = PrevFxrate;
			}
			else
			{
				NewCostData.Price = pItem->Price;
				NewCostData.USDPrice = pItem->USDPrice;
				NewCostData.Fxrate = pItem->Fxrate;
			}
			
			CostAvCostMethod(&NewCostRes, &NewCostData);
		}
	}
	
	CostRes->RunDir = NewCostRes.RunDir;
	CostRes->AvPrice = NewCostRes.AvPrice;
	CostRes->AvUSDPrice = NewCostRes.AvUSDPrice;
	CostRes->NomAmount = NewCostRes.NomAmount;
}
/*****************************************
*****************************************/
void CostUpdateTotal(CostType *CostResult, CostType *CostRes)
{
	CostResult->RunDir = CostRes->RunDir;
	CostResult->LastDir = CostRes->LastDir;
	CostResult->nRunDir = CostRes->nRunDir;;
	CostResult->NomAmount = CostRes->NomAmount;
	CostResult->AvPrice = CostRes->AvPrice;
	CostResult->AvUSDPrice = CostRes->AvUSDPrice;
	CostResult->Amort = CostRes->Amort;
	CostResult->BondFact = CostRes->BondFact;
	CostResult->Profit += CostRes->Profit;
	CostResult->FProfit += CostRes->FProfit;
}