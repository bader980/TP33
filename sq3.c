//--------------------------TCP over UDP-------------------
//---------------------------TP3 serveur-------------------

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void error(char *msg)
{
	perror(msg);
	exit(0);
}

int max(int x, int y)
{
	if (x > y)
		return x;
	else
		return y;
}

int main(int argc, char *argv[])
{
	int sock1, sock2, length1, length2, fromlen, n, nready, maxfdp1;
	struct sockaddr_in addr1, addr2;
	struct sockaddr_in from;
	char buf[1024];
	fd_set rset;
	
	if (argc !=3)
	{ 
		fprintf(stderr, "ERROR, format <programme> <port1> <port2>\n");
		exit(0);
	}
	
	//Création des sockets
	sock1 = socket(AF_INET, SOCK_DGRAM,0);
	if (sock1 < 0)
	{
		error("Error in opening socket1");
	}
	
	sock2 = socket(AF_INET, SOCK_DGRAM,0);
	if (sock2 < 0)
	{
		error("Error in opening socket2");
	}
	
	printf("Socket created succefully\n");
	
	length1 = sizeof(addr1);
	bzero(&addr1, length1);
	addr1.sin_family = AF_INET;
	addr1.sin_addr.s_addr = INADDR_ANY;
	addr1.sin_port = htons(atoi(argv[1]));
	
	length2 = sizeof(addr2);
	bzero(&addr2, length2);
	addr2.sin_family = AF_INET;
	addr2.sin_addr.s_addr = INADDR_ANY;
	addr2.sin_port = htons(atoi(argv[2]));
	
	//Bind
	if (bind(sock1,(struct sockaddr *)&addr1, length1) < 0)
	{
		error("Error in binding1");
	}
	if (bind(sock2,(struct sockaddr *)&addr2, length2) < 0)
	{
		error("Error in binding2");
	}
	
	printf("Binding...\n");
	
	fromlen = sizeof(struct sockaddr_in);
	
	//Clear the descriptor set
	FD_ZERO(&rset);
	
	//Get maxfd
	maxfdp1 = max(sock1, sock2) + 1;
	
	//Echange de données
	for(;;)
	{	
		
		//Set sock1 and sock2 in readset
		FD_SET(sock1, &rset);
		FD_SET(sock2, &rset);
		
		//Select the ready descriptor
		nready = select(maxfdp1, &rset, NULL, NULL, NULL);
		
		//Echange des messages d'ouverture de connexion
		if (FD_ISSET(sock1, &rset))
		{
			printf("**********************Echange des messages d'ouverture de connexion*****************************\n");
			n = recvfrom(sock1, buf, 1024, 0, (struct sockaddr *)&from,&fromlen);
			if (n < 0)
			{
				error("Error in recvfrom");
			}
			write(1,buf,n);
			n = sendto(sock1, "SYN_ACK\n",8,0,(struct sockaddr *)&from,fromlen);
			
			n = sendto(sock1, "Veuillez maintenant envoyer vos données via ce port : ",55,0,(struct sockaddr *)&from,fromlen);
			n = sendto(sock1, argv[2], strlen(argv[2]),0,(struct sockaddr *)&from,fromlen);
			if (n < 0)
			{
				error("Error in sendto");
			}
			n = recvfrom(sock1, buf, 1024, 0, (struct sockaddr *)&from,&fromlen);
			if (n < 0)
			{
				error("Error in recvfrom");
			}
			write(1,buf,n);
		}
		
		//Echange des messages de données
		if (FD_ISSET(sock2, &rset))
		{
			printf("*****************************Echange des donnees*****************************\n");
			while(1)
			{
				n = recvfrom(sock2, buf, 1024, 0, (struct sockaddr *)&from,&fromlen);
				if (n < 0)
				{
					error("Error in recvfrom");
				}
				write(1,"Received a datagram: ",21);
				write(1,buf,n);
				
				n = sendto(sock2, "Got your message\n",17,0,(struct sockaddr *)&from,fromlen);
				if (n < 0)
				{
					error("Error in sendto");
				}
				
				printf("SVP, entrez votre message : ");
				bzero(buf, 1024);
				fgets(buf,1024,stdin);
				n = sendto(sock2, buf, strlen(buf), 0, (struct sockaddr *)&from, fromlen);
				if (n < 0)
				{
					error("Error in sendto");
				}
				
				n = recvfrom(sock2, buf, 1024, 0, (struct sockaddr *)&from, &fromlen);
				if (n < 0)
				{
					error("Error in recvfrom");
				}
				write(1," + Got an ACK: ",12);
				write(1, buf, n);
			}
		
		}
		
	}
}










