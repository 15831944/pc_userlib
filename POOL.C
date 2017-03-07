/************************************************************************

  Pool.c - Generic package to pool dates.


*************************************************************************/
#include <stdio.h>

extern int pools[];

int getPool(days)
int days;

{
  int index=0;

  while ((pools[index] < days) && (pools[index] != -1))
    index++;

  if (pools[index] == -1)
    return(-1);
  else
    return(index);
} /* getPool */
