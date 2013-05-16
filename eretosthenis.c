#include <stdio.h>
#include <stdlib.h>

main ()
{
	int n;
	scanf("%d",&n);
	int numbers[n+1];
	int p,q,i,j,counter;
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

	scanf ("%d %d",&p,&q);
	p=nthprime(p,file);
	q=nthprime(q,file);

	long int N,phi=(long int)(p-1)*(q-1);
	N=(long int)p*q;
	scanf("%d",&j);
	int e=coprime(j,phi);
	printf("p=%d\nq=%d\nphi=%ld\ne=%d\nN=%ld\n",p,q,phi,e,N);

	int d=findd(phi,e);
	printf("d=%d\n",d);



	printf("Enter a Msg for encryption\n");
	char msg[1000],send[1000];
	char *ascii;

	long int m,enc=1;
	scanf("%d",&m);
	for (j=0;j<e;j++)
	{
		enc=enc*m;
		enc=enc%N;
	}
	printf("encryption=%d",enc);

	printf("\nDecrypting...\n");

	long int dec=1;
	for (j=0;j<d;j++)
	{
		dec=(long int)dec*enc;
		dec=dec%N;
	}

	printf("Decryption=%d\n",dec);

}
