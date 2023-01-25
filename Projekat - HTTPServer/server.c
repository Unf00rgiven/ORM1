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
    Opis:           HTTP Server
    
    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>       //strlen
#include<sys/socket.h>
#include<arpa/inet.h>   //inet_addr
#include<unistd.h>      //write
#include<pthread.h>     //thread

#define DEFAULTBUFLEN 512
#define DEFAULTPORT   27015

void * HttpServer(void * client_socket);

int main(int argc , char *argv[])
{
    int socketDesc , clientSock , c ,readSize;
    struct sockaddr_in server , client;
   
    //Create socket
    socketDesc = socket(AF_INET , SOCK_STREAM , 0);
    if (socketDesc == -1)
    {
        printf("Could not create socket");
	return 1; 
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULTPORT);

    //Bind
    if( bind(socketDesc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socketDesc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //Accept connection from an incoming client
    while(clientSock = accept(socketDesc, (struct sockaddr *)&client, (socklen_t*)&c))
    {
	    if (clientSock < 0)
    	    {
         	   perror("accept failed");
        	   return 1;
    	    }

   	    puts("Connection accepted");

	    pthread_t t;
	    int *pclient = malloc(sizeof(int));
	    *pclient = clientSock;
	    pthread_create(&t, NULL, HttpServer, pclient);
    }

    close(socketDesc);
    close(clientSock);

    return 0;
}

void * HttpServer(void * clientSocket)
{
    int readSize;
    char clientMessage[DEFAULTBUFLEN];
    int sock = *((int*)clientSocket);	
    free(clientSocket);  


    //Receive a message from client and string separation
    while( (readSize = recv(sock , clientMessage , DEFAULTBUFLEN , 0)) > 0 )
    {
	    clientMessage[readSize]= '\0';

	    char *fileName;
	    fileName = strtok(&clientMessage, " ");

        if (strcmp(fileName, "GET") != 0)
        {
            puts("Invalid request");
            return 1;
        }

        fileName = strtok(NULL, " ");

	    //Search for file
	    FILE* fp;
	    char fileContent[DEFAULTBUFLEN];

	    if ((fp = fopen(fileName, "r")) == NULL)
	    {
            //File not found
	        sprintf(fileContent, "HTTP/1.1 404 Page not found\nConnection: close\n");

	        if (send(sock, fileContent, strlen(fileContent), 0) < 0)
	        {
	            puts("Send failed");
	            return 1;
	        }

	        readSize = 0;
	        break;
	    }

        char *templateResponse = "\nHTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\nConnection: close\n";

        //File found 
        if (send(sock, templateResponse, strlen(templateResponse), 0) < 0)
        {
            puts("Send failed");
            return 1;
        }

	    //Reading from file
        while (fgets(fileContent, sizeof(fileContent), fp)) 
        {
            if (send(sock, fileContent, strlen(fileContent), 0) < 0)
            {
                puts("Send failed");
                return 1;
            }
  	    }
    
        readSize = 0;
    	fclose(fp);
        break;
    }
	
    if(readSize == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(readSize == -1)
    {
        perror("recv failed");
    }

}




