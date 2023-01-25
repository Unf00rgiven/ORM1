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
    Opis:           HTTP Server
    
    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/

#include<stdio.h>      //printf
#include<string.h>     //strlen
#include<sys/socket.h> //socket
#include<arpa/inet.h>  //inet_addr
#include <fcntl.h>     //for open
#include <unistd.h>    //for close

#define DEFAULTBUFLEN 512
#define DEFAULTPORT   27015


int main(int argc , char *argv[])
{
    int sock, readSize;
    struct sockaddr_in server;
    char path[DEFAULTBUFLEN];
    char serverReplay[DEFAULTBUFLEN], exit[3];

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);

    if (sock == -1)
    {
        printf("Could not create socket");
	    return 1;
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(DEFAULTPORT);

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");
	printf("Datoteka:");
	scanf("%s",path);

    //Get request
    char request[DEFAULTBUFLEN];
    sprintf(request, "GET %s ", path);

   //Send some data
    if( send(sock, request, strlen(request), 0) < 0)
    {
        puts("Send failed");
        return 1;
    }

	puts("Server message:");    	

    //Recieving data from requested path
    while ((readSize = recv(sock, serverReplay, DEFAULTBUFLEN, 0)) > 0)
    {
        serverReplay[readSize] = '\0';
        puts(serverReplay);
    }

    close(sock);

    return 0;
}
