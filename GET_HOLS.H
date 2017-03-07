#ifndef _GET_HOLS_DEFINED
/******************************************
get_hols.h  Header File

*******************************************/
void get_holidays(const char*);

struct it {
char c;
char	h[11];
	};

typedef  struct it	it , *itpntr;

itpntr	holidays;

#define _GET_HOLS_DEFINED
#endif
