#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <unistd.h>
#include <sys/types.h>  /* tipi di dati di sistema */
#include <sys/socket.h> /* definizioni utili per le socket() */
#include <netinet/in.h>
#define SIZE 128

// prototypes

void error(char* msg);

// main

int main(int argc, char** argv) {
	int sockfd, newsockfd;
	int portno, bindno;
	struct sockaddr_in serv_addr, cli_addr;
	int clilen;
	pid_t pid;
	int n;
	char* msg = malloc(SIZE*sizeof(char));

	if(argc < 2) {
		error("Error: No port provided");
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		error("Error on socket");
	}
	puts("socket created...");

	//azzera i bit di serv_addr
	memset((void*) &serv_addr, 0, sizeof(serv_addr));
	//assegnazione numero di porta
	portno = atoi(argv[1]);
	//assegnamento famiglia
  serv_addr.sin_family = AF_INET;
  //assegnaento indirizzo per ricevere da ogni IP
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  //impostazione della porta
  serv_addr.sin_port = htons(portno);	

  bindno = bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
  if(bindno < 0){
  	error("Error on bind");
  }
	puts("bind done...");

  listen(sockfd, 5);
  puts("waiting for connections...");

  while(1){
  	clilen = sizeof(cli_addr);
  	newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, (socklen_t*) &clilen);
  	if(newsockfd < 0){
  		error("Error on accept");
  	}
  	puts("connection established...");

  	pid = fork();

  	if(pid == 0){
  		//child process
  		close(sockfd); 		
  		do{
  			memset(msg, 0, SIZE);

  			n = read(newsockfd, msg, SIZE-1);
  			if(n < 0){
  				error("Error on read");
  			}
  			printf("Receveid:%s", msg);

  			if(strcmp(msg, "QUIT\n") != 0){
  				n = write(newsockfd, msg, SIZE-1);
  				if(n < 0){
  					error("Error on write");
  				}
  				printf("Send:%s", msg);
  			}else{
  				n = write(newsockfd, msg, SIZE-1);
  				if(n < 0){
  					error("Error on write");
  				}
  				printf("Send \"QUIT\" message\n");
  			}
  		}while(1);
  		close(newsockfd);
  		return 0;
  	}
  	//father process
  	close(newsockfd);
  }

  close(sockfd);
	return 0;
}

// functions

void error(char* msg) {
	perror(msg);
	exit(1);
}