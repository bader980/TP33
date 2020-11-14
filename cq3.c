//--------------------------TCP over UDP--------------------
//---------------------------TP3 client---------------------

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
#include <ctype.h>

void error(char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sock, length, n, i;
	struct sockaddr_in server, from;
	struct hostent *hp;
	char buffer[1024];
	
	
	if (argc != 3)
	{
		fprintf(stderr, "ERROR, format <programme> <adresse IP> <port>\n");
		exit(0);
	}
	
	//Création de la socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		error("Error in creating socket");
	}
	
	memset(&server,0, sizeof(server));	
	
	server.sin_family = AF_INET;
	hp = gethostbyname(argv[1]);
	if (hp == 0)
	{
		error("Unknown host");
	}
	bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
	server.sin_port = htons(atoi(argv[2]));
	length = sizeof(struct sockaddr_in);
	
	//Echange de données
	//while(1)
	//{
	printf("Voulez-vous faire une demande d'ouverture de connexion?[Y/n]");
	bzero(buffer, 1024);
	fgets(buffer,1024,stdin);
	for(int i=0;i<4;++i)
	{
		buffer[i] = toupper(buffer[i]);
	}
	if (strncmp(buffer, "Y",1) == 0)
		{
			printf("**********************Echange des messages d'ouverture de connexion*****************************\n");
			n = sendto(sock, "SYN\n", 4, 0, (struct sockaddr *)&server, length);
			if (n < 0)
			{
				error("Error in sendto");
			}
			n = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&from, &length);
			if (n < 0)
			{
				error("Error in recvfrom");
			}
			write(1, buffer, n);
			n = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&from, &length);
			if (n < 0)
			{
				error("Error in recvfrom");
			}
			write(1, buffer, n);
			n = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&from, &length);
			if (n < 0)
			{
				error("Error in recvfrom");
			}
			write(1, buffer, n);
			printf("\n");
			n = sendto(sock, "ACK\n", 4, 0, (struct sockaddr *)&server, length);
			if (n < 0)
			{
				error("Error in sendto");
			}
			close(sock);
		}
		else
		{
		printf("*****************************Echange des donnees*****************************\n");	
		while(1)
		{	
			printf("SVP, entrez votre message : ");
			bzero(buffer, 1024);
			fgets(buffer,1024,stdin);
			n = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server, length);
			if (n < 0)
			{
				error("Error in sendto");
			}
			
			n = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&from, &length);
			if (n < 0)
			{
				error("Error in recvfrom");
			}
			write(1," + Got an ACK: ",12);
			write(1, buffer, n);
			
			n = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&from, &length);
			if (n < 0)
			{
				error("Error in recvfrom");
			}
			write(1,"Received a datagram: ",21);
			write(1, buffer, n);
			
			n = sendto(sock, "Got your message\n",17,0,(struct sockaddr *)&from,length);
			if (n < 0)
			{
				error("Error in sendto");
			}
			
			
		}				
		}
}









