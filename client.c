#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

error(char *err)
{
	perror(err);
	exit(0);
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


char *encrypt(char *msg,int extra, long long e, long long N)
{
        int i,j;
        long long chunk,enc;
        int digm=strlen(msg),digits=countdigs(N);
        char *send;
        j=digits+3;

        i=(int)(((float)digm*j)/(j-1));

        send=(char *)malloc(i*sizeof(char));
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



char *encryption(char ascii1[], long long e, long long N)
{
	char *msg,*send,*ascii;

	long long chunk;
	long long dec=1,enc=1;
	msg=stringtono(ascii1);
	int digm;
        int digits=countdigs((long long) N);
	while(((strlen(msg))%(digits-1))!=0) strcat(msg,"0");

	send=encrypt(msg,1,e,N);

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



char *decryption(char send[], long long d, long long N)
{
	char *ascii;
        ascii=encrypt(send,0,d,N);

        ascii=backtochar(ascii);
	return ascii;
}

int extractid(char data[])
{
   int id,i,n,exp=1;
   i=strlen(data);
   i--;
   for (id=0;data[i]<='9' && data[i]>='0';i--,exp=exp*10)
        {
		n=data[i];
		id=id+n*exp;

        }
   return id;

}

void shiftadd(char *string, char *add)
{
	int i=strlen(string);
        int j=strlen(add);
	for (;i>=0;i--)
		string[i+j]=string[i];
	for(i=0;i<j;i++)
		string[i]=add[i];

}



main(int argc, char *argv[])
{
        char *recv_data, send_data[2000];
	int servport,sockfd,n;
	char address[1024];
	bzero(address,sizeof(address));
        bzero(send_data,sizeof(send_data));

	if (argc==1)
	{
		printf("Enter server ip-address:-\n");
		scanf("%s",address);
		printf("Enter port of server:\n");
		scanf("%d",&servport);
	}
	else if (argc==2)
		error("usage: file_name ip_address port_no.\n");

	else
	{
		servport=atoi(argv[2]);
		strcpy(address,argv[1]);
	}

	struct sockaddr_in servaddr;
	struct hostent *server;
	int recvd_bytes,fdmax;

        fd_set master, read_fds;
        FD_ZERO(&master);
        FD_ZERO(&read_fds);

	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if (sockfd<0) error("Failure on creating socket");

	server=gethostbyname(address);
	if (server==NULL) error("Error connecting to Server");

	bzero((char *)&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	bcopy((char *)server->h_addr,(char *)&servaddr.sin_addr.s_addr,server->h_length);
	servaddr.sin_port=htons(servport);


	if (connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
		error("Error Connecting");

 	FD_SET(sockfd,&master);
        FD_SET(STDIN_FILENO,&master);
        fdmax=sockfd;

        int selfid=0;
        long long N,e,d;
        char encrypted[1024];
        printf("Enter e,N and d for encryption\ne= ");
        scanf("%lld",&e);
        printf("N= ");
        scanf("%lld",&N);
        printf("d= ");
        scanf("%lld",&d);
        printf("\n");

	while(1)
	{
                read_fds=master;
		if (select(fdmax+1,&read_fds,NULL,NULL,NULL)==-1)
		 	error("Selection error");
		if (FD_ISSET(sockfd,&read_fds))
		{
			bzero(encrypted,sizeof(encrypted));
			recvd_bytes=recv(sockfd,encrypted,sizeof(encrypted),0);
                        if (encrypted[0]>='0' && encrypted[0]<='9')
                        {
				recv_data=decryption(encrypted,d,N);
				printf("%s",recv_data);
			}
			else 
			{
				printf("%s",encrypted);
				if (selfid==0) selfid=extractid(encrypted);
			}
		}
                else if (FD_ISSET(STDIN_FILENO,&read_fds))
		{
			bzero(encrypted,sizeof(encrypted));
			bzero(send_data,sizeof(send_data));
			fgets(encrypted,1023,stdin);
			printf("\t\t\t\t\tYOU- %s",encrypted);
			if (strchr(encrypted,':')==NULL)
			{
				shiftadd(encrypted,"\t\t\t\t\tHIM-");
				strcpy(send_data,encryption(encrypted,e,N));
				send(sockfd,send_data,strlen(send_data),0);
			}
			else send(sockfd,encrypted,strlen(encrypted),0);

		}
	}
}
