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

// Creiamo la socket server 

#include <stdio.h>    // librerie di supporto necessarie
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 80 
#define PORT 8080  // la porta 8080 sarà quella utilizzata
#define SA struct sockaddr // la nostra socket

// Funzione che mette in comunicazione  client e server
void comunicazione(int sockfd) // sockfd = descrittore di file (piccolo valore intero)
{ 
	char buff[MAX]; //Max 80 caratteri in chat
	int n; 

	// loop per la chat tra client e server 
	for (;;) { 

		bzero(buff, MAX); //funzione che utilizziamo per "pulire" il buffer(chat)

		// leggo il client e copio la stringa sul buffer
		read(sockfd, buff, sizeof(buff)); 

		// stampo contenuto del client su server e la richiesta di risposta
		printf("Da Client: %s\t Risposta: ", buff); 

		if ((strncmp(buff, "exit", 4)) == 0) { //il client ha terminato la sessione 
			printf("Connessione terminata\n"); 
			break; 
		} 

		bzero(buff, MAX); 
		n = 0; 

		//copio la risposta del server sul buffer per inviarla al client
		while ((buff[n++] = getchar()) != '\n')  // copio fino alla fine
			;

		// invio la risposta al client 
		write(sockfd, buff, sizeof(buff)); 

		// se il messaggio contiene exit allora termino la connessione 
		if (strncmp("exit", buff, 4) == 0) {  // confronto le stringhe con la funzione dalla libreria string.h
			printf("Connessione terminata\n"); 
			break; 
		} 
	
	}
}


int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, client; 

	// creo la socket e verifico se sia in funzione
	sockfd = socket(AF_INET, SOCK_STREAM, 0); //IP di tipo v4 con AF_INET(si poteva utilizzare AF_LOCAL) e di tipo TCP con SOCK_STREAM

	if (sockfd == -1) {  //sockfd deve restituire un int >0
		printf("creazione fallita\n"); 
		exit(0); 
	} 

	else
		printf("Socket creata correttamente\n"); 

	bzero(&servaddr, sizeof(servaddr)); 

	// assegno l'indirizzo IP e la porta
	servaddr.sin_family = AF_INET; // tipo v4
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // indirizzo IP assegnato tramite INADDR_ANY 
	servaddr.sin_port = htons(PORT); // utilizziamo la porta 8080

	// Collego la socket all'indirizzo IP dato e verifico se funziona
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {  // se il binding non è andato a buon fine
		printf("binding fallito \n"); 
		exit(0); 
	} 
	else
		printf("Binding effettuato\n"); 

	// Se il binding è stato effettuato, il server può "ascoltare" 
	if ((listen(sockfd, 5)) < 0) { // funzione che va chiamata dopo bind() e prima di accept()
		printf("errore nell'ascolto\n"); // il secondo parametro detto backlog mi specifica quante richieste possono essere accodate(MAX 8 ad oggi)
		exit(0); 
	} 
	else
		printf("Server in ascolto\n"); 
	len = sizeof(client); 

	// Ricevo i pacchetti di dati dal client e verifico
	connfd = accept(sockfd, (SA*)&client, &len); //con accept rimuovo una richiesta di connessione dalla coda o ne aspetto un'altra
	if (connfd < 0) { 
		printf("errore nell'accettazione\n"); 
		exit(0); 
	} 
	else
		printf("Il server ha accettato il client\n"); 

	// Adesso server e client sono pronti per comunicare e chiamo la funzione di chat
	comunicazione(connfd); 

	// Terminata la sessione di comunicazione tramite la parola exit da parte del server o client chiudo la socket
	close(sockfd); 
} 

