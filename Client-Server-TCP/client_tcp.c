
#include <stdio.h>
#include <stdlib.h>     /* exit() */
#include <string.h>    /* bzero(), bcopy() */
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    // identificatore socket
    int sockfd;
    int portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    // controlla se sono stati inseriti hostname e numero di porta
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    // assegnazione numero di porta
    portno = atoi(argv[2]);
    // creazione della socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
	
    // avvia la query dns per ottenere l'IP del server	
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
	
    bzero((char *) &serv_addr, sizeof(serv_addr));
    // assegnazione famiglia
    serv_addr.sin_family = AF_INET;
    // copia il'IP del server
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    // assegna il numero di porta
    serv_addr.sin_port = htons(portno);
	
    // connect: invia il messaggio di SYN al server
    if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
		
    printf("Please enter the command: ");
	bzero(buffer,256);
    fgets(buffer,255,stdin);
	
	// send: scrittura sulla socket
	n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    
	bzero(buffer,256);
    // recv: lettura dalla socket
	n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
	
	close(sockfd);
	
    return 0;
}
