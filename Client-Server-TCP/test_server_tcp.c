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
	// identificativi socket di benvenuto e socket dedicata
  int sockfd, newsockfd;
  int portno, clilen, bindno;
  // indirizzi server e client
  struct sockaddr_in serv_addr, cli_addr;
  // flag identificatore di processo
  pid_t pid;

	if(argc < 2){
		error("No port number provided");
	}

	//socket: creazione socket
	//la funzione torna un int e necessita di famiglia, tipo e protocollo
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0){
		error("Error in socket opening");
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

  //bind: si specifica su quale porta mettersi in ascolto
  bindno = bind(portno, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
  if(bindno < 0){
  	error("Error on bind");
  }

  // listen: mette in ascolto la socket di benvenuto
  // viene specificato il numero di client che possono stare in coda   
  listen(sockfd,5);

  do{

  	//accept: fa il pop dalla coda delle conessioni pendente
  	//crea la socket dedicata e restituisce in int per il controllo
  	clilen = sizeof(cli_addr);
  	newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, (socklen_t*) &clilen);

  	if(newsockfd < 0){
  		error("Error on bind");
  	}

  	pid = fork();

  	if(pid == 0){
  		//processo figlio
  		close(sockfd);
  		//code...
  		//read(stream da cui leggere, puntatore alla destinazione, sizeof)
  		//write(stream su cui leggere, puntatore dalla sorgente, sizeof)
  	}

  	close(newsockfd);
  }while(1);

	return 0;
}

// functions

void error(char* msg){
	perror(msg);
	exit(1);
}
