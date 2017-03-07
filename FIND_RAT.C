#include <stdio.h>
#include <math.h>


const double tp_rate= 0.07;

double find_365(double trans_entered_rate, char *rate_ind, int term_days)
{
	float n;
	double trans_rate365, trans_rate360;

	if(term_days < 0 )
	{
		printf("Error Negative term_days !!!");
		return(0.0);
	}

	if (term_days < 46) 
		n = 1.0;
	else 
		if (term_days < 76) 
			n = 2.0;
		else 
			if (term_days < 106) 
				n = 3.0;
			else 
				if (term_days < 136) 
					n = 4.0;
				else 
					if (term_days < 166) 
						n = 5.0;
					else 
						if (term_days < 196) 
							n = 6.0;
						else 
							if (term_days < 226) 
								n = 7.0;
							else 
								if (term_days < 256) 
									n = 8.0;
								else 
									if (term_days < 286) 
										n = 9.0;
								else 
									if (term_days < 316) 
										n = 10.0;
									else 
										if (term_days < 351) 
											n = 11.0;
										else 
											n = 12.0;

	if (rate_ind[3] == 's')    
	{
		trans_rate365 = trans_entered_rate * (1 - (.21096 * tp_rate));
		trans_rate360 = trans_entered_rate;
	}
	else 
		if (rate_ind[2] == '0')
		{
			trans_rate360 = trans_entered_rate;
 
			if (n == 1.0)
				trans_rate365 = trans_rate360 * (365.00/360.00);
			else if (n > 1.0 && n < 13.0)
			trans_rate365 = ((pow((1+((trans_rate360 * term_days)/36000.00)),
					         (1.00/n)) - 1) * ((n * 36500.00)/term_days));
		}  
		else 
			if (rate_ind[2] == '5')
			{
				trans_rate365 = trans_entered_rate;

				if (n == 1.0)
					trans_rate360 = trans_rate365/(365.00/360.00);
				else 
					if (n > 1.0 && n < 13.0)
				trans_rate360 = ((pow((1 + ((trans_rate365 * term_days)/(n * 36500.00))),
							      n) - 1) * (36000.00/term_days)); 
			}
			else 
				return(0.0);

	return(trans_rate365);
}


double  find_360 (double trans_entered_rate, char *rate_ind, int term_days)
{
	float n;
	double	trans_rate365, trans_rate360;

	if(term_days < 0 )
	{
		printf("Error Negative term_days !!!");
		return(0.0);
	}

	if ( term_days < 46) 
		n = 1.0;
	else 
		if (term_days < 76)
			n = 2.0;
		else 
			if (term_days < 106) 
				n = 3.0;
			else 
				if (term_days < 136) 
					n = 4.0;
				else 
					if (term_days < 166) 
						n = 5.0;
					else 
						if (term_days < 196) 
							n = 6.0;
						else 
							if (term_days < 226) 
								n = 7.0;
							else 
								if (term_days < 256) 
									n = 8.0;
								else 
									if (term_days < 286) 
										n = 9.0;
									else 
										if (term_days < 316) 
											n = 10.0;
										else 
											if (term_days < 351) 
												n = 11.0;
										else 
											n = 12.0;

	if (rate_ind[3] == 's')    
	{
		trans_rate365 = trans_entered_rate * (1 - (.21096 * tp_rate));
		trans_rate360 = trans_entered_rate;
	}
	else 
		if (rate_ind[2] == '0')
		{
			trans_rate360 = trans_entered_rate;
 
			if (n == 1.0)
				trans_rate365 = trans_rate360 * (365.00/360.00);
			else 
				if (n > 1.0 && n < 13.0)
					trans_rate365 = ((pow((1+((trans_rate360 * term_days)/36000.00)),
									(1.00/n)) - 1) * ((n * 36500.00)/term_days));
		}  
		else 
			if (rate_ind[2] == '5')
			{
				trans_rate365 = trans_entered_rate;

				if (n == 1.0)
					trans_rate360 = trans_rate365/(365.00/360.00);
				else 
					if (n > 1.0 && n < 13.0)
						trans_rate360 = ((pow((1 + ((trans_rate365 * term_days)/(n * 36500.00))),
										n) - 1) * (36000.00/term_days)); 
			}
			else 
				return(0.0);

	return(trans_rate360);
}
