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
	int portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server; /* struttura dati per rappresentare gli host (nome, alias, indirizzo)*/
	char buffer[SIZE];

	if(argc < 3){
		error("Error: please provide an adress and a port number");
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0){
		error("error on socket opening");
	}
	puts("socket opened");

	portno = atoi(argv[2]);
	server = gethostbyname(argv[1]);
  if(server == NULL){
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }	
	bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno); 
	/* host-to-network short: converte un intero in un formato a 16 bit, indipendente dall'artchitettura
	big indian o little indian dell'host */

  while(1){
  	memset(buffer, 0, SIZE);
  	printf("Insert some words(max %d characters):", SIZE-1);
  	fgets(buffer, SIZE-1, stdin);
  	n = sendto(sockfd, buffer, SIZE-1, 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
  	if(n < 0){
  		error("Error on writing");
  	}

  	n = recvfrom(sockfd, buffer, SIZE-1, 0, NULL, NULL);
  	if(n < 0){
  		error("Error on reading");
  	}
  	printf("Receveid message:%s", buffer);
  }

  close(sockfd);

	return 0;
}

// functions

void error(char* msg){
	perror(msg);
	exit(1);
}