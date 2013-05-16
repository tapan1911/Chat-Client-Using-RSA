#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/select.h>
#define MAXUSR 5

void error(char *a)
{
	perror(a);
	exit(0);
}

int search(int i, int chatting[][MAXUSR+3])
{
	int j;
	for (j=0;j<MAXUSR+3;j++)
		if (chatting[i][j]==1)
			return j;
	return -1;
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




void print(int chatting[][MAXUSR+3])
{
        int i,j;
        for (i=0;i<MAXUSR+3;i++)
	{
		for (j=0;j<MAXUSR+3;j++)
			printf("%d ",chatting[i][j]);
                printf("\n");
	}
}


main(int argc, char *argv[])
{
	int port;
	if (argc<2)
	{
		printf("Enter port No.:- ");
		scanf("%d",&port);
	}
	else port=atoi(argv[1]);

	fd_set master, read_fds, write_fds;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);

	int serverfd, newfd,fdmax, i, j, addrlen;
	struct sockaddr_in serveraddr, clientaddr;

	char buf[1024], msg[MAXUSR+3][1024];
	int chatting[MAXUSR+3][MAXUSR+3];
        int useractive[MAXUSR+3];
    	bzero(useractive,sizeof(useractive));
        bzero(msg,sizeof(msg));
	bzero(buf,sizeof(buf));
	bzero(chatting,sizeof(chatting));
	for (i=0;i<MAXUSR+3;i++)
		chatting[i][i]=-1;

	serverfd=socket(AF_INET,SOCK_STREAM,0);
	if (serverfd==-1)
		error("socket call failed");

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	serveraddr.sin_port=htons(port);
	bzero(serveraddr.sin_zero,8);

	if (bind(serverfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))==-1)
		error("Failure in Binding");

	printf("Server Established\n");

	if (listen(serverfd,5)==-1) error("Failure On Connecting");


	FD_SET(serverfd,&master);
	fdmax=serverfd;
	while (1)
	{
		read_fds=master;
                write_fds=master;
		if (select(fdmax+1,&read_fds,&write_fds,NULL,NULL)==-1)
			error("Selection error");

		for (i=0;i<=fdmax;i++)
		{
			if (FD_ISSET(i,&read_fds))
			{
				if (i==serverfd)
				{
					addrlen=sizeof(clientaddr);
					newfd=accept(serverfd,(struct sockaddr *)&clientaddr,&addrlen);

					if (newfd==-1)
						error("Failure while Accepting Connection\n");
					else
					{
						bzero(buf,sizeof(buf));
						if (newfd>fdmax) fdmax=newfd;
						FD_SET(newfd,&master);

						printf("Connection Accepted from client at socket %d\n",newfd);
						strcpy(buf,"\t\t\t\t\tServer-Connected Successfully with server Your id is ");
						useractive[newfd]=1;
						strcat(buf,inttoasc(newfd));
						send(newfd,buf,strlen(buf),0);
						bzero(buf,sizeof(buf));
					}      
				}
				else
				{
					bzero(buf,sizeof(buf));
					if (j=recv(i,buf,sizeof(buf),0)<=0)
					{
						if (j==0)
							printf("Client on Socket %d hung up\n",i);
						else printf("Error in connection with socket %d. Closing connection\n",i);
						FD_CLR(i,&master);
						useractive[i]=0;
						close(i);
						int id=search(i,chatting);
						chatting[i][id]=chatting[id][i]=0;
					}
					else 
					{
						int total=strlen(buf),k=0;
						char command[5],input[51];
						bzero(command,sizeof(command));
						bzero(input,sizeof(input));
						printf("%d: %s\n",i,buf);
						if (strchr(buf,':')!=NULL)
						{
							for (j=0;buf[j]!=':';j++)
								command[j]=buf[j];
							command[j]='\0';
							j++;
							for (k=0;j<total+1 && k<50; k++,j++)
								input[k]=buf[j];
							input[k]='\0';
printf("Command: %s\nInput: %s\n",command,input);
							if (strcmp(command,"id")==0)
							{
								bzero(buf,sizeof(buf));
								int id=atoi(input);
								if (id<=fdmax && useractive[id])
								{
									if (search(i,chatting)==-1)
									{
										chatting[id][i]=chatting[i][id]=1;
										strcpy(buf,"\t\t\t\t\tServer-You Can chat now\n");
									}
									else strcpy(buf,"\t\t\t\t\tServer-You Cannot chat with two people simultaneously\n");
								}
								else
									strcpy(buf,"\t\t\t\t\tServer-ERROR... No client with that id\n");
								send(i,buf,strlen(buf),0);
							}
							else if (strcmp(command,"end")==0)
							{
								if(strcmp(input,"client")==0)
								{
									int id=search(i,chatting);
									FD_CLR(i,&master);
									close(i);
									useractive[i]=0;
									chatting[i][id]=chatting[id][i]=0;
								}
								else if (strcmp(input,"chat")==0)
								{
									bzero(buf,sizeof(buf));
									int id=search(i,chatting);
									if(id==-1) strcpy(buf,"\t\t\t\t\tServer-You are not chatting to anyone\n");
									else
									{ 
										strcpy(buf,"\t\t\t\t\tServer-Chatting closed successfully\n");
										chatting[i][id]=chatting[id][i]=0;
										send(id,buf,strlen(buf),0);
									}
									send(i,buf,strlen(buf),0);
								}
								else 
								{
									bzero(buf,sizeof(buf));
									strcpy(buf,"\t\t\t\t\tServer-Invalid input\n");
									send(i,buf,strlen(buf),0);
								}
							}
							else 
							{
								bzero(buf,sizeof(buf));
								strcpy(buf,"\t\t\t\t\tServer-Invalid Command\n");
								send(i,buf,strlen(buf),0);
							}
						}
                                                else
						{
					
							int id;
							if ((id=search(i,chatting))==-1)
							{
								bzero(buf,sizeof(buf));
								strcpy(buf,"\t\t\t\t\tServer-You are not chatting to anyone currently\n");
								send(i,buf,strlen(buf),0);
							}
							else
							{
								strcpy(msg[i],buf);
								bzero(buf,sizeof(buf));
							}
						}
					}
				}
			}	
			if(FD_ISSET(i,&write_fds))
			{
				int id;
				if ((id=search(i,chatting))!=-1)
				{
					send(i,msg[id],strlen(msg[id]),0);
					bzero(msg[id],sizeof(msg[id]));
                                }
				if (useractive[i]==1)
				{
					char new[50]="Server-You are chatting with ";
					bzero(new,sizeof(new));
					strcat(new,inttoasc(search(i,chatting)));
					send(i,buf,strlen(buf),0);
					bzero(buf,sizeof(buf));
					useractive[i]=2;
				}
			}
		}
	}
}
