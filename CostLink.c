#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<memory.h>
#include<stdlib.h>
#include "costlink.h"
#include "USERINFO.H"

void NodeAppend(CCostNodeType **Header, CCostNodeType *pCostNode)
{
	CCostNodeType *p, *r;
	
	r = NodeCreate(pCostNode);

	if(*Header == NULL)
	{
		*Header = r;
		return;
	}

	p = *Header;
	while(p->Next != NULL)
		p = p->Next;

	p->Next = r;
	r->Prev = p;
}

void NodeAssign(CCostNodeType *pDestNode, CCostNodeType *pSourceNode)
{
	pDestNode->TransNum = pSourceNode->TransNum;
	pDestNode->InvNum = pSourceNode->InvNum;
	pDestNode->Seq = pSourceNode->Seq;
	SaveCopy(pDestNode->TradeDate, sizeof(pDestNode->TradeDate), pSourceNode->TradeDate, 0);
	pDestNode->Dir = pSourceNode->Dir;
	pDestNode->NomAmount = pSourceNode->NomAmount;
	pDestNode->Price = pSourceNode->Price;
	pDestNode->Fxrate = pSourceNode->Fxrate;
	pDestNode->ClFxrate = pSourceNode->ClFxrate;
	pDestNode->USDPrice = pSourceNode->USDPrice;
	pDestNode->Profit = pSourceNode->Profit;
	pDestNode->FProfit = pSourceNode->FProfit;
	pDestNode->UnRealPL = pSourceNode->UnRealPL;
	pDestNode->UnRealFPL = pSourceNode->UnRealFPL;
	pDestNode->Remain = pSourceNode->Remain;
	pDestNode->Amort = pSourceNode->Amort;
	pDestNode->Addable = pSourceNode->Addable;
	pDestNode->Next = NULL;
	pDestNode->Prev = NULL;
}

int NodeCompare(CCostNodeType *pCostNode1, CCostNodeType *pCostNode2)
{
	if(strcmp(pCostNode2->TradeDate, pCostNode1->TradeDate) == 0) // Same TradeDate
	{
		if(pCostNode2->Seq >= 0 && pCostNode1->Seq >= 0)
		{
			if(pCostNode2->Seq > pCostNode1->Seq) // Node2 > Node1
				return 1;
			else
				if(pCostNode2->Seq == pCostNode1->Seq) // Node2 equal to Node1
					return 0;
				else
					return -1;					 // Node2 < Node1
		}
	}

	if(pCostNode2->TransNum > pCostNode1->TransNum) // Node2 > Node1
		return 1;
	else
		if(pCostNode2->TransNum == pCostNode1->TransNum) // Node2 == Node1
			return 0;
		else
			return -1;									// Node2 < Node1
}

int NodeCompareInvNum(CCostNodeType *pCostNode1, CCostNodeType *pCostNode2)
{
	if(pCostNode2->InvNum > pCostNode1->InvNum) // Node2 > Node1
		return 1;
	else
		if(pCostNode2->InvNum == pCostNode1->InvNum) // Node2 == Node1
			return 0;
		else
			return -1;									// Node2 < Node1
}

int NodeCount(CCostNodeType *Header)
{
	int Count = 0;

	if(Header == NULL)
		return 0;
	
	while(Header != NULL)
	{
		Count++;
		Header = Header->Next;
	}
	
	return Count;
}

CCostNodeType *NodeCreate(CCostNodeType *pCostNode)
{
	CCostNodeType *p = NULL;

	p = (CCostNodeType *) malloc(sizeof(CCostNodeType));
	NodeAssign(p, pCostNode);
	return p;
}

void NodeDelete(CCostNodeType **Header, CCostNodeType *pCostNode)
{
	CCostNodeType *old, *p, *r;

	if(*Header == NULL)
		return;

	p = *Header;
	while(p != NULL)
	{
		if(NodeCompareInvNum(p, pCostNode) == 0)  /* If Match */
		{
			if(p == *Header)				/* First Node case */
			{
				*Header = p->Next;          /* shifted the header node */
				(*Header)->Prev = NULL;
			}
			else
			{
				r = p->Next;
				old->Next = r;
				r->Prev = old;
			}

			free(p);
		}
		else
		{
			old = p;
			p = p->Next;
		}
	}
}

void NodeDeleteAll(CCostNodeType **Header)
{
	CCostNodeType *p;

	if(*Header == NULL)
		return;

	p = *Header;
	while(p->Next)
	{
		*Header = p->Next;
		(*Header)->Prev = NULL;
		
		free(p);
		p = *Header;
	}

	if(p)
		free(p);
}

CCostNodeType *NodeGetFirst(CCostNodeType **Header, CCostNodeType *pCostNode)
{
	CCostNodeType *p;

	if(*Header == NULL)
		return NULL;

	p = *Header;
	while(p->Next != NULL)
		p = p->Next;

	NodeAssign(pCostNode, p);
	return pCostNode;
}

void NodeInsert(CCostNodeType **Header, CCostNodeType *pCostNode)
{
	CCostNodeType *p, *r;
	
	if(*Header == NULL) // If it is empty
		NodeAppend(Header, pCostNode);
	else
	{
		r = NodeCreate(pCostNode);

		p = *Header;
		for(;;)
		{
			if(NodeCompare(r, p) < 0)
			{
				p->Next = r;
				r->Prev = p;
				*Header = p;
				return;
			}
			else
			{
				r = r->Next;
				if(r->Next == NULL)
				{
					r->Next = p;
					p->Prev = r;
					return;
				}
			}
		}
	}
}



