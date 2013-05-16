#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *strrev(char *string);


char *inttoasc(long long num)                              //Function to convert integers to string
{
        long long dig=num,exp=1;
        char *asc;
        int i;

        asc=(char *)malloc(20*sizeof(char));               //Allocate space assuming the no. is less than 20 digit long

        for (i=0;dig!=0;i++)
        {
                dig=dig/10;                                       //Counting the no. of digits in the no.,i will
                exp=exp*10;                                       //i will be the no. of digits
        }
        if (exp>=10)                                          //exp will be the greatest power of 10 less than or equal to the number
                exp=exp/10;
        dig=i;                                                //dig will now have the no. of digits in the no.
        for (i=0;i<dig;i++,exp=exp/10)
        {
                asc[i]= num/exp + '0';                             //convert int ascii
                num=num%exp;
        }

        if (i==0) asc[i]='0';

        for (;i<20;i++)
                asc[i]='\0';                                        //make all the next characters '\0'
        return asc;
}




main()
{
 int *a,b;
 scanf("%d",&b);
 char *Fuck;
Fuck=inttoasc(b);
printf("%s\n",Fuck);
Fuck=strrev(Fuck);
printf("%s\n",Fuck);
}
