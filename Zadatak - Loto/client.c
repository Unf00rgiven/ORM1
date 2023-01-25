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
#include<string.h>     //strlen
#include<sys/socket.h> //socket
#include<arpa/inet.h>  //inet_addr
#include <fcntl.h>     //for open
#include <unistd.h>    //for close

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27015


int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[DEFAULT_BUFLEN];
    char server_message[DEFAULT_BUFLEN];

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
    server.sin_port = htons(DEFAULT_PORT);

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");
    
    //Send some data
    memset(server_message, '\0', DEFAULT_BUFLEN);

    if(recv(sock , server_message , DEFAULT_BUFLEN , 0) < 0)
    {
        puts("Receive failed");
        return 1;
    }

    puts("Server message:");
    puts(server_message);

	// IZBOR ZA KOMBINACIJU BROJEVA
    int kombinacija[7];
    puts("Ako zelite da unesete sami brojeve upisite 1, ako zelite da dobijete random brojeve upisite 2: ");
    int izbor;
    scanf("%d", &izbor);
    
    int i = 0;
    srandom(time(0));
    
    // STVARANJE KOMBINACIJE
    if(izbor == 1)
    {
		for(i ; i < 7 ; i++)
		{
			puts("Unesite svoju kombinaciju:");
		    scanf("%d", &kombinacija[i]);	
		}	
	}else if(izbor == 2)
    {
		for(i ; i < 7 ; i++)
		{
			kombinacija[i] = (rand()%39+1);
		}
		
	}else 
	{
		puts("Pogresan unos izbora");
		return 1;
	}
	
	printf("Vasi brojevi su: %d, %d, %d, %d, %d, %d, %d\n", kombinacija[0], kombinacija[1], kombinacija[2], kombinacija[3], kombinacija[4], kombinacija[5], kombinacija[6]);
	
	if(send(sock, kombinacija, 7*sizeof(int), 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	
	memset(server_message, '\0', DEFAULT_BUFLEN);

    if(recv(sock , server_message , DEFAULT_BUFLEN , 0) < 0)
    {
        puts("Receive failed");
        return 1;
    }

    puts("Server message:");
    puts(server_message);
	
	
    close(sock);

    return 0;
}

