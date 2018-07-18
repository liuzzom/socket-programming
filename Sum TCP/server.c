#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <unistd.h>
#include <sys/types.h>  /* tipi di dati di sistema */
#include <sys/socket.h> /* definizioni utili per le socket() */
#include <netinet/in.h>

// prototypes

void error(char* msg);

// main

int main(int argc, char** argv){
	int sockfd, newsockfd;
	int portno, bindno;
	struct sockaddr_in serv_addr, cli_addr;
	int clilen;
	pid_t pid;
	int n, number1, number2, sum = 0;

	if(argc < 2){
		error("Error: Any port number provided");
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

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		error("Error on socket");
	}
	puts("socket created...");

	bindno = bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if(bindno < 0){
		error("Error on bind");
	}
	puts("bind done...");

	listen(sockfd, 5);
	puts("waiting connections...");

	while(1){
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, (socklen_t*) &clilen);
		if(newsockfd < 0){
			error("Error on accept");
		}
		puts("connection established...");

		pid = fork();

		if(pid == 0){
			// child process
			close(sockfd);
			
			//memset(&number1, 0, sizeof(int));
			//memset(&number2, 0, sizeof(int));
			//memset(&sum, 0, sizeof(int));
			n = read(newsockfd, &number1, sizeof(int));
			if(n < 0){
				error("Error on read");
			}
			printf("Receveid:%d\n", number1);

			n = read(newsockfd, &number2, sizeof(int));
			if(n < 0){
				error("Error on read");
			}
			printf("Receveid:%d\n", number2);
			sum = number1 +number2;

			n = write(newsockfd, &sum, sizeof(int));
			if(n < 0){
				error("Error on write");
			}
			printf("%d+%d=%d\n", number1, number2, sum);
			
			close(newsockfd);
			puts("connection closed...");
			return 0;	
		}
		// father process
		close(newsockfd);
	}

	close(sockfd);
	return 0;
}

// functions

void error(char* msg){
	perror(msg);
	exit(1);
}