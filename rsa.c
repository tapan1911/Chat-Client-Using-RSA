#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


int chooserandom(int in,int N)
 {
    int ans=in;
       if (ans<0) ans=-ans;
          ans=ans%N;
             if (ans==0) ans=37;
                return (int)ans;
 }

int nthprime(int n, char file[])
{
	int i,ans;
	FILE *f=fopen(file,"r");
	for (i=0;i<n;i++) fscanf(f,"%d",&ans);
        fclose(f);
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

long long coprime(int j, long long phi)
{
	long long i=2,counter=0;
	for (i=2;counter<j;i++)
		if (gcd(phi,i)==1) counter++;
		else if(i>=phi) i=2;
	return i-1;
}

long long findd(long long phi, long long e)
{
	int i=0;
	for (i=0;i<e;i++)
		if (((long long)(1+((long long)i)*(phi)))%((long long)e)==0) return((1+i*phi)/e);
}

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


long long breakinn(char *msg,int start,int n)
{
   int i,j;
   long long send;
   char ans[n+1];
   for (j=0,i=start;i<start+n && i<strlen(msg);j++,i++)
       ans[j]=msg[i];
   ans[j]='\0';
   send=atoll(ans);
   return send;
}

int countdigs(long long dig)
{
int i=0;
        for (i=0;dig!=0;i++)
                dig=dig/10;                                       //Counting the no. of digits in the no.,i will
return i;

}


char *stringtono(char msg[])
{
        int i=0,digits,num,len=strlen(msg),j;
        char *ans;
        ans=(char *)malloc(3*len*sizeof(char));
        ans[0]='\0';
        for (digits=0,i=0;digits<3*len && i<len;i++)
        {
                num=(int)msg[i];
                for (j=0;j<3-countdigs(num);j++)
                     strcat(ans,"0");
                strcat(ans,inttoasc(num));
        }
        return ans;
}

long long power(long long m,long long e, long long N)
{
      long long i=1;
      long long enc=1;

      for (enc=m%N;e>1;e=e/2)
        {
                if (e%2==0)
                    enc=enc*enc;
                else
                    {
                      i=i*enc;
                      e--;
                      enc=enc*enc;
                    }
                enc=enc%N;
                i=i%N;
        }
        enc=enc*i;
        enc=enc%N;
        return enc;
}


char *encrypt(char *msg,int extra, long long e, long long N)
{
	int i,j;
	long long chunk,enc;
	int digm=strlen(msg),digits=countdigs(N);
	char *send;
        j=digits+3;

	i=(int)(((float)digm*j)/(j-1));

	send=(char *)malloc(i*sizeof(char));
	if(send==NULL) printf("Failed\n");
        else 	printf("Success\n");        
	send[0]='\0';


	for (i=0;i<strlen(msg);i=i+digits-extra)
	{
		chunk=breakinn(msg,i,digits-extra);
		enc=power((long long)chunk,e,N);
		digm=countdigs(enc);

		for (j=0;j<digits-digm-(1-extra);j++)
			strcat(send,"0");
		strcat(send,inttoasc(enc));
	}
	return send;
}

char *backtochar(char *msg)
{
	int i,block,j;
	char *ans;
	ans=(char *)malloc((((strlen(msg))*sizeof(char))/3)+1);
	ans[0]='\0';
	for (i=0,j=0;i<strlen(msg);i=i+3,j++)
	{
		block=breakinn(msg,i,3);
		ans[j]=block;
	}
	ans[j]='\0';
	return ans;
}






main ()
{
        srand((unsigned)time(NULL));

	int n;
	printf("Enter total No. of Prime Numbers to be found: ");
	scanf("%d",&n);
	int *numbers;
	numbers=(int *)malloc(n*sizeof(int));
	int p,q,i,j,counter,digits;
	char file[]="output.txt";
	FILE *f=fopen(file,"w");
	for (i=2;i<=n;i++)
		numbers[i]=1;

	for (i=2;i<=n;i++)
	{
		if (numbers[i])
		{
			counter++;
			fprintf(f,"%d\n",i);
		}
		for (j=2;i*j<=n;j++) numbers[i*j]=0;
	}
	fclose(f);

	printf("Prime Numbers upto %d are printed in file %s\n",n,file);
	free(numbers);
	printf("Please Enter the no.s of the two primes to be chosen: ");
	scanf ("%d %d",&p,&q);

        p=nthprime(chooserandom(p,counter),file);
	q=nthprime(chooserandom(q,counter),file);

	long long N,phi=(long long)(p-1)*(q-1);
	N=(long long)p*q;
	digits=countdigs((long long)N);

	printf("You have chosen %d and %d as primes\n",p,q);

	printf("Enter the no. of coprime number to phi=%lld: ",phi);    
	scanf("%d",&j);
        j=chooserandom(j,phi);
	long long e=coprime(j,phi);
        printf("e, N and d are:- ");
	printf("%lld %lld\n",e,N);

	long long d=findd(phi,e);
	printf("%lld\n\n",d);



	printf("Enter a Msg for encryption: ");
	char ascii1[1000],one;
	char *msg,*send,*ascii;

	long long chunk;
	long long dec=1,enc=1;
//	scanf("%s",ascii1);
        scanf("%c",&one);
        if (one!='\n') ungetc((int)one,stdin);
        fgets(ascii1,999,stdin);
	msg=stringtono(ascii1);
	int digm;

        while(((strlen(msg))%(digits-1))!=0) strcat(msg,"0");

        printf("intput=%s\n",msg);
	printf("encryption=\n");

        send=encrypt(msg,1,e,N);

	printf("%s\n",send);
	printf("\nDecrypting...\n");


	printf("Decryption=");

	ascii=encrypt(send,0,d,N);
	printf("%s\n",ascii);

	ascii=backtochar(ascii);
	printf("%s\n",ascii);
}

