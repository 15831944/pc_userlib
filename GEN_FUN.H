#ifndef 	_GEN_FUN_DEFINED

/*****************************************************
gen_fun.h  Header File
*****************************************************/

/* Macros
**********/
#define	  MIN(a,b)	( a>b ? b : a)
#define	  MAX(a,b)	( a>b ? a : b)


/*  Function Prototypes
************************/

int Day(char *date);
int Month(char *date);
int Year(char *date);
int leap(int year);
int Days_in(int ,int );
int not_valid_date(char *date);
int get_days_out(int , int );
int int_size(int i);
int Days_between(char *after,char *before);
int Day_of_week(char *date);
int	find_day_in_month(int,int,int,int);
int is_remote(void);
char *commas(double d,int len,int dec, char *output, int out_size);
double round(double,int);
double trunc(double,int);
void upperize(char*);
void lower(char*);
char *conv_date(char*,char*);
char *conv_back_date(char*,char*);
char* strrtrim(char* str);
char* strltrim(char* str);
char* strtrim(char* str);
char *StrRTrim(char *str, char TrimedChar);
char *StrLTrim(char *str, char TrimedChar);
char *StrTrim(char *str, char TrimedChar);
int IsIn(const char *str, const char *instr);
int GetInput(const char *Prompt, char *Buf);
int GetConditions(const char *Prompt, char *Conditions, int con_size);
int GetDate(const char *Prompt, char *Date);
void DateToYearMonthDay(const char *Date, int *Year, int *Month, int *Day);
int GetJulianDays(int Year, int Month, int Day);

/*Global Data Structures
************************/

extern char *months[];
extern char *days[];

#define 	_GEN_FUN_DEFINED				1
#endif
