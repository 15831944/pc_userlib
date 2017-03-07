#ifndef _OPT_MATH_DEFINED
double normal(double x);
double der_normal(double x);
double b_s_call(double s,double k,double r,double t, double v,double d);
double b_s_put(double s,double k,double r,double t,double v, double d);
double b_s_cdelta(double s,double k,double r,double t,double v, double d);
double b_s_pdelta(double s,double k,double r,double t,double v, double d);
double b_s_crho(double s,double k,double r,double t,double v, double d);
double b_s_prho(double s,double k,double r,double t,double v, double d);
double b_s_lambda(double s,double k,double r,double t,double v, double d);
double b_s_gamma(double s,double k,double r,double t,double v, double d);
double b_s_ctheta(double s,double k,double r,double t,double v, double d);
double b_s_ptheta(double s,double k,double r,double t,double v, double d);
double i_vol_call(double s, double k,double r,double t, double root, double d, int *erp);
double i_vol_put(double s, double k, double r, double t, double root, double d, int *erp);
double bs_geo_put(double h,double t0,double t1,double t2,double S,double K,
                  double A,double r, double d,double v	);
double bs_geo_call(double h,double t0,double t1,double t2,double S,double K,
                  double A,double r, double d,double v	);
#define  _OPT_MATH_DEFINED   1
#endif
