#include <stdio.h>


extern FILE  *fpntr;

void
table(dataOutPtr,number,col,row,ncol)
char *(* dataOutPtr)();
int number;
int col;
int row;                            
int ncol;

{ 
  int done,cstart,cend;
  int index1,index2,index3;
  static char buffer[450];
           
  done = 0;       
  cstart = 0;
  if (col < ncol)
    cend = col;
  else
    cend = ncol;

  do
  {                          
    fprintf(fpntr,"%s",(*dataOutPtr)(-1,-1,-1,(char *)&buffer));

    /* Print the Column headings */
    for(index3=cstart;index3<cend;index3++)
      fprintf(fpntr,"%s",(*dataOutPtr)(-1,-1,index3,(char *)&buffer));
    fprintf(fpntr,"\n");
    for(index2=0;index2<row;index2++)
    {
      for(index1=1;index1<=number;index1++)
      {                            
        /* Print the row heading */
        fprintf(fpntr,"%s",(*dataOutPtr)(index1,index2,-1,(char *)&buffer));
        /* Print the rest of the table */
        for(index3=cstart;index3<cend;index3++)
          fprintf(fpntr,"%s",(*dataOutPtr)(index1,index2,index3,(char *)&buffer));
        /* End of the line. */
        fprintf(fpntr,"\n");
      }                       
      /* End of the table. */
      fprintf(stderr,".");
    }
    
    if (cend == col)
      done = 1;
    else
    {
      cstart = cend;
      cend = cstart + ncol; 
      if (cend >= col)
        cend = col;
    }
  } while (done == 0); 

  fprintf(stderr,"\n");
}
