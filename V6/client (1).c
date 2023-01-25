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

#include<stdio.h>      //printf
#include<string.h>     //memset
#include<arpa/inet.h>  //inet_aton
#include<sys/socket.h> //socket
#include <unistd.h>    //close
#include <netdb.h>     //gethostbyname

#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 27015  //The port on which to send data

int main(void)
{
    struct sockaddr_in sock_server;
    struct hostent *he;
    int s, slen = sizeof(sock_server);
    char buf[BUFLEN];
    char message[BUFLEN];

    // Create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        puts("Could not create socket");
        return 1;
    }

    memset((char *) &sock_server, 0, sizeof(sock_server));
    sock_server.sin_family = AF_INET;
    sock_server.sin_port = htons(PORT);

    // Check if address is valid
    if (inet_aton(SERVER, &sock_server.sin_addr) == 0)
    {
        printf("Enter message : ");
    }
       
    //inet_aton(SERVER, &server_addr.sin_addr);
    
    while(1)
    {
        printf("Enter message : ");
        fgets(message, BUFLEN, stdin);

        // Send the message to server
        if (sendto(s, message, strlen(message), 0, (struct sockaddr *) &sock_server, (socklen_t)slen)==-1)
        {
            perror("sendto failed. Error");
            return 1;
        }

        // Clear old buffer
        memset(buf,'\0', BUFLEN);
        // Wait for server response (blocking call)
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &sock_server, (socklen_t*)&slen) == -1)
        {
            perror("recvfrom failed. Error");
            return 1;
        }
    }

    close(s);
    return 0;
}