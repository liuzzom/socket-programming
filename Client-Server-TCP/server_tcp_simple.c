/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <time.h>       /* funzioni time() e ctime() */
#include <unistd.h>
#include <sys/types.h>  /* tipi di dati di sistema */
#include <sys/socket.h> /* definizioni utili per le socket() */
#include <netinet/in.h>


//funzione di stampa di un errore
void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
	// identificativi socket di benvenuto e socket dedicata
     int sockfd, newsockfd;
     int portno, clilen;
     char buffer[256];
     // indirizzi server e client
     struct sockaddr_in serv_addr, cli_addr;
     int n, counter;
     // flag identificatore di processo
     pid_t pid;

     counter = 0;

     // controlla se l'utente ha inserito il numero di porta 
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     // creazione della socket
     // AF_INET: familgia Ipv4
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     // controlla se la funzione socket non ha avuto problemi
     // se vi è un errore la funzione restituisce -1
     if (sockfd < 0) {
        error("ERROR opening socket");
     }

     // azzera tutti i bit partendo da &ser_addr
     // vengono azzerati sizeof(serv_addr) bit
     bzero((char *) &serv_addr, sizeof(serv_addr));
     // assegnamento numero porta scoket
     portno = atoi(argv[1]);
     //assegnamento famiglia
     serv_addr.sin_family = AF_INET;
     //assegnaento indirizzo per ricevere da ogni IP
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     //impostazione della porta
     serv_addr.sin_port = htons(portno);

     // bind: si specifica su quale porta mettersi in ascolto
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){ 
              error("ERROR on binding");
        }

     // listen: mette in ascolto la socket di benvenuto
     // viene specificato il numero di client che possono stare in attesa   
     listen(sockfd,5);

     // affinché la socket server rimanga in ascolto
     // si utilizza in questo caso un ciclo infinito
     do {
        clilen = sizeof(cli_addr);
        // fase di accept: viene ricevuto il connect del client
        // crea, partendo dalla socket di benvenuto una socket dedicata
        newsockfd = accept(sockfd, 
                    (struct sockaddr *) &cli_addr, 
                    (socklen_t *)&clilen);

        if (newsockfd < 0) {
              error("ERROR on accept");
        }

        // creazione di un processo figlio
        // assegnamento ad un flag per il cotrollo
        // tutte le variabili assegnate finora sono presenti in entrambi i processi
        pid=fork();

        // nel caso del processo figlio pid==0
        if (pid == 0) {
			
		// chiusura della copia della socket di benvenuto nel processo figlio
          close(sockfd);

          bzero(buffer,256);
          // lettura dalla socket
          n = read(newsockfd,buffer,255);
          if (n < 0) {
            error("ERROR reading from socket");
          }
  
          if(!strcmp(buffer, "T\n")) {
            counter++;
  
            time_t t = time(NULL);
  
            char* timestr = ctime(&t);
            // scrittura dalla socket 
            n = write(newsockfd, timestr, strlen(timestr)+1);
            if (n < 0) {
              error("ERROR writing to socket");
            }
          } 
		  
		  else if(!strcmp(buffer, "N\n")) {
              char cnt[5]; 
            sprintf(cnt, "%d", counter);
            n = write(newsockfd, cnt, strlen(cnt));
            if (n < 0) {
              error("ERROR writing to socket");
            }
          } 
		  
		  else {
            // delay
            printf("Please press a key...");
            char c;
            scanf("%c", &c);
            n = write(newsockfd, "Message received", 20);
            if (n < 0) {
              error("ERROR writing to socket");
            }
          }

          // chiusura della socket dedicata
          close(newsockfd);
          return 0;
        }

        // chiusura della copia della socket dedicata
        // nel processo padre
        close(newsockfd);
		 
		 
     } while (1);
     return 0; 
}
