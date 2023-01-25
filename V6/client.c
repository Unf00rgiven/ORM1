/* 
    ********************************************************************
    Odsek:          Elektrotehnika i racunarstvo
    Departman:      Racunarstvo i automatika
    Katedra:        Racunarska tehnika i racunarske komunikacije (RT-RK)
    Predmet:        Osnovi Racunarskih Mreza 1
    Godina studija: Treca (III)
    Skolska godina: 2021/22
    Semestar:       Zimski (V)
    
    Ime fajla:      client.c
    Opis:           TCP/IP klijent
    
    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/

#include <stdio.h>      //printf
#include <stdlib.h>
#include <string.h>     //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //inet_addr
#include <fcntl.h>     //for open
#include <unistd.h>    //for close

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27015

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server = {0};
    char *message = "Test message.\n";

    //Create socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock == -1)
    {
        perror("Could not create socket.");
		close(sock);
		exit(1);
	}
    puts("Socket created.");

    server.sin_family = AF_INET;
    server.sin_port = htons(DEFAULT_PORT);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");

    int len = sendto(sock, (const char*)message, strlen(message), 0, (const struct sockaddr*)&server, sizeof(server));
	if(len == -1)
	{
		perror("Couldn't send to server.");
		close(sock);
		exit(1);
	}

	printf("Length: %d\n", len);

    printf("Client message: ");
    puts(message);

    close(sock);
    
	return 0;
}

