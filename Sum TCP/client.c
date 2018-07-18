#include <stdio.h>
#include <stdlib.h>     /* exit() */
#include <string.h>    /* bzero(), bcopy() */
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// prototypes

void error(char* msg);

// main

int main(int argc, char** argv){
	int sockfd;
	int portno;
	int connectno;
	struct sockaddr_in serv_addr;
	struct hostent* server;
	int n, number1, number2, sum;

	if(argc < 3){
		error("Error: insert an hostname or IP address and a port number");
	}

	//preparazione dati da inserire in connect

	//assegnazione numero di porta 
	portno = atoi(argv[2]);
	//query dns per ottenere ip server
	server = gethostbyname(argv[1]);
	if(server == NULL){
		error("Error on host name");
	}
	//azzera i bit dell'indirizzo del server
	memset((void*) &serv_addr, 0, sizeof(serv_addr));
	//imposta la famiglia (IPv4)
	serv_addr.sin_family = AF_INET;
	//copia l'indirizzo del server ottenuto da argv
	bcopy((void*) server->h_addr, (void*) &serv_addr.sin_addr.s_addr, server->h_length);
	//assegna il numero di porta
	serv_addr.sin_port = htons(portno);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		error("Error on socket");
	}
	puts("socket created...");

	connectno = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if(connectno < 0){
		error("Error on connect");
	}
	puts("client connected...");

	//memset(&number, 0, sizeof(int));
	//memset(&sum, 0, sizeof(int));

	printf("Insert the first number:");
	scanf("%d", &number1);
	n = write(sockfd, &number1, sizeof(int));
	if(n < 0){
		error("Error on write");
	}

	printf("Insert the second number:");
	scanf("%d", &number2);
	n = write(sockfd, &number2, sizeof(int));
	if(n < 0){
		error("Error on write");
	}

	n = read(sockfd, &sum, sizeof(int));
	if(n < 0){
		error("Error on read");
	}
	printf("Sum is %d\n", sum);

	close(sockfd);
	return 0;
}

// functions

void error(char* msg){
	perror(msg);
	exit(1);
}