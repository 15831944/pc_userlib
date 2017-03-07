#ifndef  _FORMULA_DEFINED
#define MAXWORDLEN      16
#define SPACE           ((char)0x20)

#define	EXPR	0
#define	ADD	1
#define	SUB	2
#define	MUL	3
#define	DIV	4
#define POW	5
#define NEW	9

#define	DONE  1
#define NOT   0

char	        *wordptr;       /* Pointer to next word in linebfr. */


double 	formula(char *, char);
char 	*nextword(char *, char);
int 	wordlen(char *);
double 	act(double, double, int);
#define  _FORMULA_DEFINED
#endif
