#include <stdio.h>
#include <math.h>
#include "normsdist.h"

#ifndef PI
#define PI 3.141592653589793238462643
#endif

double SND(double x) // Standard Normal Distribution
{
	return (1.0/sqrt(2.0*PI))*exp(-0.5*x*x);
}

double CSND(double x) // Culmulative Standard Normal Distribution
{
	double b1 = 0.31938153, b2 = -0.356563782, b3 = 1.781477937, b4 = -1.821255978, b5 = 1.330274429;
	double p = 0.2316419; 
	double a, n, t;

	if(x > 6.0)
		return 1.0;
	if(x < -6.0)
		return 0.0;

	a = fabs(x);
	t = 1.0/(1.0 + a*p);
	n = 1.0 - 1.0/sqrt(2*PI)*exp(-x*x/2)*((((b5*t + b4)*t+b3)*t+b2)*t+b1)*t;
	if(x < 0.0)
		n = 1.0 - n;

	return n;
}

double Linest(double *XArray, double *YArray, int ArraySize, double *Slope, 
			  double *Y_intercept, double *SumProduct)
{
   double Sumx = 0, Sumy = 0, Sumxx = 0, Sumxy = 0, Sumres = 0;
   double res, y_estimate;
   int i;

   if(ArraySize <= 0)
	   return -1;

   for(i = 0; i < ArraySize; i++) 
   {
       Sumx += XArray[i];
       Sumy += YArray[i];
       Sumxx += pow(XArray[i], 2);
       Sumxy += XArray[i]*YArray[i];
   }

   *SumProduct = Sumxy;
   *Slope = (Sumxx*Sumy - Sumx*Sumxy) / (ArraySize*Sumxx - Sumx*Sumx);
   *Y_intercept = (ArraySize*Sumxy - Sumx*Sumy)/(ArraySize*Sumxx - Sumx*Sumx);

   Sumres = 0;
   for (i=0; i<ArraySize; i++) 
   {
      y_estimate = *Slope*XArray[i] + *Y_intercept;
      res = YArray[i] - y_estimate;
      Sumres = Sumres + res*res;
   }

   return Sumres;
}
