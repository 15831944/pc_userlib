#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <process.h>
#include "formula.h"


extern double run_fun(char*);

/*****
* Find the next word in the line buffer.
* IN:     wordptr points to the first character of a word or a preceding
*         space.
* RETURN: A pointer to the first character of the expression. If there is no
*         more formula, a pointer to the terminating NULL.
*****/

char *nextword(char *wordptr, char c)
{
	/* Advance to the first non-space. */
	while( *wordptr==SPACE && *wordptr != c)
		wordptr++;
	return(wordptr);
}

/*****
* Find the length of a word. A word is defined as a sequence of characters
* terminated by a space or a NUL.
* IN:    wordptr points to a word.
* RETURN:The length of the word.
*****/
int wordlen(char *wordptr)
{
	char *wordlimit;

	if(*wordptr == '+' ||*wordptr == '-' ||*wordptr == '*' ||*wordptr == '/' ||
		*wordptr == '(' ||*wordptr == ')'||*wordptr == '^')return(1);

	wordlimit = wordptr;
	while(*wordlimit &&        *wordlimit != '+' && *wordlimit != '-' && 
		*wordlimit != '*' && *wordlimit != '/' && *wordlimit != '^' &&
    	*wordlimit != '(' && *wordlimit != ')' && *wordlimit!=SPACE )
		wordlimit++;

	return( wordlimit-wordptr );
}

/**********
***********/
double formula(char *line, char c)
{
	char buf[MAXWORDLEN];
	int	MODE;
	int	STATE;
	double result = 0.0, cur_val;
	int i;

	MODE = NEW;
	STATE = NOT;
	wordptr = nextword(line,c);
	while( *wordptr != c)
	{
	    /* Find the length of this word, increment the proper element of the
			length count array, & point to the space following the word. */
		i = wordlen(wordptr);
		if ( i > MAXWORDLEN )
		{
			printf("Unusually long argument found.\nCannot Parse %.20s....",
			wordptr);
			return 0;
		}
		else
		{
			strncpy_s(buf, MAXWORDLEN, wordptr,i);
			buf[i]='\0';

			if(i==1 && (MODE == EXPR || MODE == NEW))
			{
				switch (buf[0]) 
				{
					case '+':
						printf("\nAddition");
						MODE = ADD;
						STATE = DONE;
						break;
					case '-':
						MODE = SUB;
						STATE = DONE;
						printf("\nSubstraction");
						break;
					case '*':
						printf("\nMultiplication");
						STATE = DONE;
						MODE = MUL;
						break;
					case '/':
						STATE = DONE;
						MODE = DIV;
						printf("\nDivision");
						break;
					case '^':
						printf("\nPower Raising");
						MODE = POW;
						STATE = DONE;
						break;
					default	:
						break;
				}			
			}

			if(MODE != EXPR && STATE == NOT)
			{
				if(sscanf_s(buf, "%lf", &cur_val)==1)
				{
					if(MODE == NEW)
					{
						result = cur_val;
						printf("\nNew Value %.2f",cur_val);
						MODE = EXPR;
						STATE = DONE;
					}
					else
					{
						printf("\nValue %.2f",cur_val);
						result = act(cur_val,result,MODE);
						MODE = EXPR;
						STATE = DONE;
					}
				}
				else
				{
					if(buf[0]== '(')
					{
						wordptr++;
						cur_val = formula(wordptr,')');			
						i=1;
					}
					else
						cur_val = run_fun(buf);
					
					result = act(cur_val,result,MODE);
					MODE = EXPR;
					STATE = DONE;
				}
				printf("\n %.2f",result);
			}
		}
      
		wordptr += i;
		/* Find the next word (if any). */
		wordptr = nextword(wordptr,c);
		printf("\nI now point to %.10s ",wordptr);
		if(STATE == NOT)
		{
			printf("\nUnable to perform action ERROR");
			exit(0);
		}
		else 
			STATE = NOT;
    }
	
	printf("\nFormula returns %.2f",result);
	return(result);
}
/**********************
***********************/

double act(double curval, double res, int mode)
{
	switch (mode)
	{
		case NEW:
			res = curval;
			break;
		case ADD:
			res += curval;
			break;
		case SUB:
			res -= curval;
			break;
		case MUL:
			res *= curval;
			break;
		case POW:
			res = pow(res,curval);
			break;
		case DIV:
			if(curval == 0.0)
			{
				printf("\nDivision by ZERO Error.");
				exit(0);
			}
			else 
				res /= curval;
			break;
		default	:
			printf("Unknown Operator");
			exit(0);
	}			
	return(res);
}

