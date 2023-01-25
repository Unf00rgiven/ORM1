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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27015

int main(int argc , char *argv[])
{
    int socket_desc;
    struct sockaddr_in server = {0};
    char client_message[DEFAULT_BUFLEN];
   
    //Create socket
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socket_desc == -1)
    {
        perror("Could not create socket.");
		exit(1);
    }
    puts("Socket created.");

    //Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_port = htons(DEFAULT_PORT);
	server.sin_addr.s_addr = INADDR_ANY;

    //Bind
    if(bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) == -1)
    {
        //print the error message
        perror("Bind failed. Error.\n");
		close(socket_desc);
        exit(1); 
    }
    puts("Bind done.");

    socklen_t len = 0;

	if(recvfrom(socket_desc, (char*)client_message, DEFAULT_BUFLEN, MSG_WAITALL, 0, &len) == -1)
	{
		perror("Failed to receive.");
		close(socket_desc);
	}

    puts(client_message);

	close(socket_desc);

    return 0;
}
