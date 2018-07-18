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

	int writeno, readno;

	if(argc < 3){
		error("Error on ports provided");
	}

	//creazioe della socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		error("Error on socket");
	}

	//preparazione dati da inserire in connect
	//assegnazione numero di porta 
	portno = atoi(argv[2]);
	//quesry dns per ottenere ip server
	server = gethostbyname(argv[1]);
	if(server == NULL){
		error("Error on host name provided");
	}
	//azzera i bit dell'indirizzo del server
	memset((void*) &serv_addr, 0, sizeof(serv_addr));
	// assegnazione famiglia
  serv_addr.sin_family = AF_INET;
	//copia l'indirizzo del server ottenuto da argv
	bcopy((void*) server->h_addr, (void*) &serv_addr.sin_addr.s_addr, server->h_length);
	//assegna il numero di porta
	serv_addr.sin_port = htons(portno);

	//connect: invia il messaggio di syn al server che è in listen
	connectno = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if(connectno < 0){
		error("Error on conect");
	}

	// writeno = write(socket, stream sorgente, size to write)
	// error if writeno is < 0
	// readno = read(socket, stream dest, size to read)
	// error if readno is < 0

	return 0;
}

// functions

void error(char* msg){
	perror(msg);
	exit(1);
}