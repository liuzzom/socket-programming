#include <stdio.h>
#include <stdlib.h>
#include <string.h>     /* per strlen() */
#include <string.h>    /* per bzero() e bcopy() */
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>  /* tipi di dati di sistema */
#include <sys/socket.h> /* definizioni utili per le socket() */
#define SIZE 128

// prototypes

void error(char* msg);

// main

int main(int argc, char** argv){
	int sockfd;
  int portno, clilen, bindno;
  int bytesread;
	struct sockaddr_in serv_addr, cli_addr;
	char buffer[SIZE];

	if(argc < 2){
		error("error: please provide a port number");
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0){
		error("error on socket opening");
	}
	puts("opening success...");

	memset((void*) &serv_addr, 0, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY; /*indica che i dati sono accettati da qualsiasi indirizzo IP*/
	serv_addr.sin_port = htons(portno);

	bindno = bind(sockfd, (struct sockaddr*) & serv_addr, sizeof(serv_addr));
	if(bindno < 0){
		error("error on binding");
	}
	puts("binding success...");

	clilen = sizeof(cli_addr);

	while(1){
		memset(buffer, 0, SIZE);
		bytesread = recvfrom(sockfd, buffer, SIZE-1, 0, (struct sockaddr*) &cli_addr, (socklen_t*) &clilen);
		if(bytesread < 0){
			puts("Error on reading. Sending error message");
			bytesread = sendto(sockfd, "Error", 6, 0, (struct sockaddr*) &cli_addr, clilen);
			if(bytesread < 0){
				error("error on writing");
			}
		}else{
			printf("Receveid message:%s", buffer);
			bytesread = sendto(sockfd, buffer, SIZE-1, 0, (struct sockaddr*) &cli_addr, clilen);
			if(bytesread < 0){
				error("error on writing");
			}
		}
	}

	close(sockfd);

	return 0;
}

// functions

void error(char* msg){
	perror(msg);
	exit(1);
}