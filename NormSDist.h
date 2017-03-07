#ifndef __NORMSDIST_H__
#define __NORMSDIST_H__

double CSND(double x);
double SND(double x);
double Linest(double *XArray, double *YArray, int ArraySize, 
			  double *Slope, double *Y_intercept, double *SumProduct);
#endif
