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

void * tcp_handle_conn(void * client_socket);

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c ,read_size;
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

    //accept connection from an incoming client
    while(client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))
    {
	    if (client_sock < 0)
    	    {
        	    perror("accept failed");
        	    return 1;
    	    }

   	    puts("Connection accepted");

	    pthread_t t;
	    int *pclient = malloc(sizeof(int));
	    *pclient = client_sock;
        PRINTF
	    pthread_create(&t, NULL, tcp_handle_conn, pclient);
    }

    close(socket_desc);
    close(client_sock);

    return 0;
}

void * tcp_handle_conn(void * client_socket)
{
    int read_size;
    char client_message[DEFAULT_BUFLEN];
    int sock = *((int*)client_socket);	
    free(client_socket);
    
    //Receive a message from client
    while( (read_size = recv(sock , client_message , DEFAULT_BUFLEN , 0)) > 0 )
    {
	    client_message[read_size]= '\0';
            printf("Message:%s , bytes received:%d\n", client_message, read_size);

	    if(send(sock , client_message , strlen(client_message), 0) < 0)	
	    {
		    puts("Send failed");
        	    return 1;
	    }
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


