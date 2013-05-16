#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*int chooserandom(int in,int N)
{
   int ans=in;
   if (ans<0) ans=-ans;
   ans=ans%N;
   if (ans==0) ans=37;
   return (int)ans;
}*/

int nthprime(int n, char file[])
{
  int i,ans;
  FILE *f=fopen(file,"r");
  for (i=0;i<n;i++) fscanf(f,"%d",&ans);
  return ans;
}

void swap(int *m, int *n)
{
  int p=*m;
  *m=*n;
  *n=p;
}


int gcd(int m, int n)
{
  if (m<n) swap(&m,&n);
  if (m==1) return 1;
  if (m%n==0) return n;
  else return gcd(n,m%n);
} 

int coprime(int j, int phi)
{
  int i=2,counter=0;
  for (i=2;counter<j;i++)
      if (gcd(phi,i)==1) counter++;
      else if(i>=phi) i=2;
  return i-1;
}

long int findd(long int phi, int e)
{
  int i=0;
  for (i=0;i<e;i++)
     if (((long int)(1+((long int)i)*(phi)))%((long int)e)==0) return((1+i*phi)/e);
}

char *inttoasc(int num)                              //Function to convert integers to string
{
	int i,dig=num,exp=1;
	char *asc;

	asc=(char *)malloc(20*sizeof(char));               //Allocate space assuming the no. is less than 20 digit long

	for (i=0;dig!=0 && i<5;i++)
	{
		dig=dig/10;                                       //Counting the no. of digits in the no.,i will
		exp=exp*10;                                       //i will be the no. of digits
	}
	if (exp>=10)                                          //exp will be the greatest power of 10 less than or equal to the number
		exp=exp/10;
	dig=i;                                                //dig will now have the no. of digits in the no.
	printf("%d\t%d\n",exp,num);
	for (i=0;i<dig;i++,exp=exp/10)
	{
		asc[i]= num/exp + '0';                             //convert int ascii
		num=num%exp;
	}

	for (;i<10;i++)
		asc[i]='\0';                                        //make all the next characters '\0'
	return asc;
}


long int breakin9(char *msg,int start)
{
   int i;
   long int send;
   char ans[10];
   for (i=start;i<start+9 && i<strlen(msg);i++)
       ans[i]=msg[i];
   ans[i]='\0';
   send=atoi(ans);
   return send;
}



char *stringtono(char *msg, int len)
{
	int i=0,digits,num;
	char *ans;
	ans=(char *)malloc(3*len*sizeof(char));
        ans[0]='\0'
	for (digits=0,i=0;digits<3*len && i<len;i++)
	{
		num=(int)msg[i];
                strcat(ans,inttoasc(num));
        }
        return ans;
}
