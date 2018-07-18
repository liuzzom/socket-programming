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

int main(int argc, char**argv){
	int sockfd, newsockfd;
	int portno, bindno;
	struct sockaddr_in serv_addr, cli_addr;
	int clilen;
	pid_t pid1, pid2;
	int n;
	char* msg = malloc(SIZE*sizeof(char));

	if(argc < 2){
		error("Error: no port provided");
	}

	// socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		error("Error on socket");
	}
	puts("socket success");

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

  //bind
  bindno = bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
  if(bindno < 0){
  	error("Error on bind");
  }
  puts("bind success");

  //listen
  listen(sockfd, 5);
  puts("listen success");

  while(1){
  	//accept
  	clilen = sizeof(cli_addr);
  	newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, (socklen_t*) &clilen);
  	if(newsockfd < 0){
  		error("Error on accept");
  	}
  	puts("accept success");

  	pid1 = fork();

  	if(pid1 == 0){
  		//child process
			close(sockfd);
			pid2 = fork();	
			while(1){
				if(pid2 == 0){
					//reading process
					memset(msg, 0, sizeof(msg));
					n = read(newsockfd, msg, SIZE-1);
					if(n < 0){
						error("Error on read");
					}
					printf("\t\t\t\t%s", msg);
				}else{
					//writing process
					memset(msg, 0, SIZE-1);
					fgets(msg, SIZE-1, stdin);		
					n = write(newsockfd, msg, SIZE-1);
					if(n < 0){
						error("Error on read");
					}
				}
			}
  	}

  	//father process
  	close(newsockfd);  	
  }

	return 0;
}

// functions

void error(char* msg){
	perror(msg);
	exit(1);
}