#ifndef __COSTLINK_H__
#define __COSTLINK_H__

typedef struct CCostNodeT
{
	int TransNum;
	int InvNum;
	int Seq;
	char TradeDate[11];
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
	struct CCostNodeT *Next;
	struct CCostNodeT *Prev;
} CCostNodeType;

void NodeAppend(CCostNodeType **Header, CCostNodeType *pCostNode);
void NodeAssign(CCostNodeType *pDestNode, CCostNodeType *pSourceNode);
int NodeCompare(CCostNodeType *pNode1, CCostNodeType *pNode2);
int NodeCompareInvNum(CCostNodeType *pNode1, CCostNodeType *pNode2);
int NodeCount(CCostNodeType *Header);
CCostNodeType *NodeCreate(CCostNodeType *pCostNode);
void NodeDelete(CCostNodeType **Header, CCostNodeType *pCostNode);
void NodeDeleteAll(CCostNodeType **Header);
CCostNodeType *NodeGetFirst(CCostNodeType **Header, CCostNodeType *pCostNode);
void NodeInsert(CCostNodeType **Header, CCostNodeType *pCostNode);
#endif

