/* 
    ********************************************************************
    Odsek:          Elektrotehnika i racunarstvo
    Departman:      Racunarstvo i automatika
    Katedra:        Racunarska tehnika i racunarske komunikacije (RT-RK)
    Predmet:        Osnovi Racunarskih Mreza 1
    Godina studija: Treca (III)
    Skolska godina: 2021/22
    Semestar:       Zimski (V)
    
    Ime fajla:      server.c
    Opis:           TCP/IP server
    
    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/

#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> // tredovi

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27015
static int igraci = 0;
static int klijent1brojevi[7];
static int klijent2brojevi[7];

void * tcp_handle_conn(void * client_socket);

int main(int argc , char *argv[])
{
	int loto[7];
    int klijent = 0 ; 				 // brojac za klijente
    int socket_desc , client_sock1, client_sock2 , c ,read_size;
    struct sockaddr_in server , client;
   
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
	return 1; 
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULT_PORT);

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    
    //POVEZIVANJE SA KLIJENTIMA 1 i 2
    client_sock1 = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if(klijent < 2 ) //max 2 klijenta u suprotnom se prekida igra
	{	
		klijent++;
		if (client_sock1 < 0)
	    	{
	        	perror("accept failed");
	        	return 1;
	
			}
	}
	else
    {
		puts("Dozvoljeno je max 2 igraca");
		return 1;
    }
    puts("Connection accepted");
    
    client_sock2 = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if(klijent < 2 ) //max 2 klijenta u suprotnom se prekida igra
	{	
		klijent++;
		if (client_sock2 < 0)
	    	{
	        	perror("accept failed");
	        	return 1;
			}
	}
	else
    {
		puts("Dozvoljeno je max 2 igraca");
		return 1;
    }
	puts("Connection accepted"); 
	
	
	//SLANJE PORUKA ZA START IGRE
	char *start_message = "START";
	
	if(send(client_sock1, start_message, strlen(start_message), 0) < 0)
	{
		puts("ERROR, send failed");
		return 1;
	}
	if(send(client_sock2, start_message, strlen(start_message), 0) < 0)
	{
		puts("ERROR, send failed");
		return 1;
	}
	
	
	// NITI ZA KLIJENTE
	pthread_t t1;
	int *pclient1 = malloc(sizeof(int));
	*pclient1 = client_sock1;
	pthread_create(&t1, NULL, tcp_handle_conn, pclient1);
	pthread_join(t1, NULL);
	
	
	pthread_t t2;
	int *pclient2 = malloc(sizeof(int));	
	*pclient2 = client_sock2;
	pthread_create(&t2, NULL, tcp_handle_conn, pclient2);
	pthread_join(t1, NULL);
	
	// DOBITNI BROJEVI
	int dobitnibrojevi[7];
	srand(time(0));
	for(int i = 0 ; i < 7 ; i++) dobitnibrojevi[i] = (rand()%39+1);
	printf("Izvuceni brojevi su: %d, %d, %d, %d, %d, %d, %d\n", dobitnibrojevi[0], dobitnibrojevi[1], dobitnibrojevi[2], dobitnibrojevi[3], dobitnibrojevi[4], dobitnibrojevi[5], dobitnibrojevi[6]);
	
	// PROVERA DOBITKA
	int pogodak1 = 0, pogodak2 = 0, i = 0 ;

	for(i = 0 ; i < 7 ; i++)
	{
		for(int k=0; k<7; k++)
		{
			if(loto[i] == klijent1brojevi[k]) pogodak1++;
			if(loto[i] == klijent2brojevi[k]) pogodak2++;
		}
	}

	// SLANJE PORUKE KLIJENTU
	char *tri_pogotka = "Imali ste 3 pogotka";
    char *cetiri_pogotka = "Imali ste 4 pogotka";
    char *pet_pogodaka = "Imali ste 5 pogodaka";
    char *sest_pogodaka = "Imali ste 6 pogodaka";
    char *sedam_pogodaka = "Imali ste 7 pogodaka";
    char *nema_pogodaka = "Niste imali dovoljno pogodaka za dobitak";
    
    if(pogodak1 == 0 || pogodak1 == 1 || pogodak1 == 2){
	if(send(client_sock1, nema_pogodaka, strlen(nema_pogodaka), 0) < 0)
	{
	    puts("Send failed");
            return 1;
	}
	puts("Igrac 1 nije imao dovoljno pogodaka za dobitak.");
    }
    else if(pogodak1 == 3){
	if(send(client_sock1, tri_pogotka, strlen(tri_pogotka), 0) < 0)
	{
	    puts("Send failed");
            return 1;
	}
	puts("Igrac 1 je imao tri pogotka.");
    }
    else if(pogodak1 == 4){
	if(send(client_sock1, cetiri_pogotka, strlen(cetiri_pogotka), 0) < 0)
	{
	    puts("Send failed");
            return 1;
	}
	puts("Igrac 1 je imao cetiri pogotka.");
    }
    else if(pogodak1 == 5){
	if(send(client_sock1, pet_pogodaka, strlen(pet_pogodaka), 0) < 0)
	{
	    puts("Send failed");
            return 1;
	}
	puts("Igrac 1 je imao pet pogodaka.");
    }
    else if(pogodak1 == 6){
	if(send(client_sock1, sest_pogodaka, strlen(sest_pogodaka), 0) < 0)
	{
	    puts("Send failed");
            return 1;
        }
	puts("Igrac 1 je imao sest pogodaka.");
    }
    else {
	if(send(client_sock1, sedam_pogodaka, strlen(sedam_pogodaka), 0) < 0)
	{
	    puts("Send failed");
            return 1;
	}
	puts("Igrac 1 je imao sedam pogodaka.");
    }



    if(pogodak2 == 0 || pogodak2 == 1 || pogodak2 == 2){
	if(send(client_sock2, nema_pogodaka, strlen(nema_pogodaka), 0) < 0)
	{
	    puts("Send failed");
            return 1;
	}
	puts("Igrac 2 nije imao dovoljno pogodaka za dobitak.");
    }
    else if(pogodak2 == 3){
	if(send(client_sock2, tri_pogotka, strlen(tri_pogotka), 0) < 0)
	{
	    puts("Send failed");
            return 1;
	}
	puts("Igrac 2 je imao tri pogotka.");
    }
    else if(pogodak2 == 4){
	if(send(client_sock2, cetiri_pogotka, strlen(cetiri_pogotka), 0) < 0)
	{
  	    puts("Send failed");
            return 1;
	}
	puts("Igrac 2 je imao cetiri pogotka.");
    }
    else if(pogodak2 == 5){
	if(send(client_sock2, pet_pogodaka, strlen(pet_pogodaka), 0) < 0)
	{
	    puts("Send failed");
            return 1;
	}
	puts("Igrac 2 je imao pet pogodaka.");
    }
    else if(pogodak2 == 6){
	if(send(client_sock2, sest_pogodaka, strlen(sest_pogodaka), 0) < 0)
	{
	    puts("Send failed");
            return 1;
	}
	puts("Igrac 2 je imao sest pogodaka.");
    }
    else {
	if(send(client_sock2, sedam_pogodaka, strlen(sedam_pogodaka), 0) < 0)
	{
	    puts("Send failed");
            return 1;
	}
	puts("Igrac 2 je imao sedam pogodaka.");
    }
   

    close(socket_desc);
    close(client_sock1);
    close(client_sock2);

    return 0;
}

void * tcp_handle_conn(void * client_socket)
{
    igraci++;
    int read_size;
    int sock = *((int*)client_socket);	
    free(client_socket);
    int client_message[7];
    int i;

    //Receive a message from client
    if( (read_size = recv(sock , client_message , 7*sizeof(int) , 0)) < 0 )
    {
        printf("ERROR, recieve failed");
        return 1;
	}
	
	printf("Klijent%d:", igraci);
	//PREUZIMANJE LOTO BROJEVA OD KLIJENTA
	if(igraci == 1)
	{
		for(i=0 ; i < 7 ; i++)
		{
			printf("%d ", client_message[i]);
			klijent1brojevi[i] = client_message[i];
		} 
		printf("\n");

	}
	if(igraci == 2)
	{
		for(i=0 ; i < 7 ; i++)
		{
			printf("%d ", client_message[i]);
			klijent2brojevi[i] = client_message[i];
		} 
		printf("\n");

	}
		
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

}


