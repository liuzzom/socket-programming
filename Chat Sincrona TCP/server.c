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

int main(int argc, char** argv){
	int sockfd, newsockfd;
	int portno, bindno;
	struct sockaddr_in serv_addr, cli_addr;
	int clilen;
	pid_t pid;
	int n;
	char* msg = malloc(SIZE*sizeof(char));  

	//verifica l'inserimento del numero di porta
	if(argc < 2){
		error("Error: no port number provided");
	}

	//socket e controllo
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		error("Error on socket");
	}

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

  //bind e controllo
	bindno = bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if(bindno < 0){
		error("Error on bind");
	}

	//listen
	listen(sockfd, 5);

	while(1){
		//accept e controllo
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, (socklen_t*) &clilen);
		if(newsockfd < 0){
			error("Error on accept");
		}
		puts("Connection Established");

		pid = fork();

		if(pid == 0){
			close(sockfd);
			do{
				memset((void*) msg, 0, sizeof(msg));
			
				n = read(newsockfd, msg, SIZE-1);
				if(n < 0){
					error("Error on read");
				}
				printf("Client:");
				printf("%s", msg);

				printf("Server:");
				fgets(msg, SIZE-1, stdin);
				n = write(newsockfd, msg, SIZE-1);
				if(n < 0){
					error("Error on write");
				}			
			}while(1);
		}

		close(newsockfd);
	}

	return 0;
}

// functions

void error(char* msg){
	perror(msg);
	exit(0);
}