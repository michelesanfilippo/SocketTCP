/*

Progetto Reti di Calcolatori A.A. 2019/2020 di Michele Sanfilippo - prof.re Biagio Lenzitti
Creazione comunicazione tra server e client attraverso protocollo TCP/IP 
Tale protocollo sfrutta la comunicazione del protocollo TCP usufruendo al tempo stesso di un indirizzo IP
per l'invio  e ricezione di pacchetti dati.
Quindi, grazie al TCP siamo sicuri che i dati rimarranno intatti e verranno trasferiti nello stesso ordine in cui
sono stati inviati.


Utilizzeremo le Socket sia per Server che Client:
- Per il server dovremo settare opportunamente la socket, utilizzare bind() per assegnare un indirizzo locale
implementare listen() per avere una socket che "ascolti", implementare accept() per stabilire una connessione

- Lato client dovremo creare una socket TCP e connetterci alla server socket

In questo modo potremo utilizzare la nostra connessione per avere un client ed un server che possono comunicare tra di loro


*/

// Creiamo socket client


#include <netdb.h>  // librerie di supporto necessarie
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 80 // lunghezza max scrittura buffer
#define PORT 8080 // porta di connessione 8080
#define SA struct sockaddr // socket



void comunicazione(int sockfd) // vedi commenti lato server per più dettagli
{ 
	char buff[MAX]; 
	int n; 

	for (;;) {  // loop chat

		bzero(buff, sizeof(buff)); // pulisco buffer 
		printf("Inserire messaggio: "); 
		n = 0; 

		while ((buff[n++] = getchar()) != '\n') // copio il messaggio
			;

		write(sockfd, buff, sizeof(buff)); // lo invio

		if ((strncmp(buff, "exit", 4)) == 0) { // il client puo decidere se terminare la sessione
			printf("\tConnessione terminata\n"); 
			break; 
		} 

		bzero(buff, sizeof(buff)); 

		read(sockfd, buff, sizeof(buff)); // copio risposta server
		printf("\tDa Server: %s", buff); 

		if ((strncmp(buff, "exit", 4)) == 0) { //il server termina la connessione
			printf("Connessione terminata\n"); 
			break; 
		} 
	} 
} 

int main() // per più dettagli vedi lato server
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	//creo la socket e verifico se il tutto sia andato a buon fine
	sockfd = socket(AF_INET, SOCK_STREAM, 0); // ip v4 tipo TCP 
	if (sockfd == -1) { 
		printf("creazione fallita\n"); 
		exit(0); 
	} 
	else
		printf("Socket creata correttamente\n"); 

	bzero(&servaddr, sizeof(servaddr)); 

	// assegno indirizzo IP e porta  
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //setto in locale
	servaddr.sin_port = htons(PORT); 

	// connetto il client alla porta server
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0) { 
		printf("connessione fallita\n"); 
		exit(0); 
	} 
	else
		printf("Connessione al server\n"); 

	//richiamo la funzione per la chat
	comunicazione(sockfd); 

	// terminata la chat chiudo la socket
	close(sockfd); 
} 

