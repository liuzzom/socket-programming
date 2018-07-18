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
	char* msg = malloc(SIZE*sizeof(char));

	if(argc < 3){
		error("Error: please provide an adress and a port number");
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if(sockfd < 0){
		error("Error on socket");
	}	

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

  memset(msg, 0, SIZE);
  printf("Insert the website name:");
  //fgets(msg, SIZE-1, stdin);
  scanf("%s", msg); //I used this because fgets store \n character, and i'm lazy 
  n = sendto(sockfd, msg, SIZE, 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
  if(n < 0){
  	error("error on sendto");
  }

  n = recvfrom(sockfd, msg, SIZE, 0, NULL, NULL);
  if(n < 0){
  	puts("error on reception");
  }else{
  	printf("answer received:%s\n", msg);
  }

  close(sockfd);
	return 0;
}

// functions

void error(char* msg){
	perror(msg);
	exit(1);
}