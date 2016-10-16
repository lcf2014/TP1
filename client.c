#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>





void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int file_size;
    FILE *received_file;
    int remain_data = 0;
    ssize_t len;

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
     int TamBuffer;
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    if(strcmp(argv[1],"list")==0){

    TamBuffer=atoi(argv[4]);
    char *comando = argv[1];
    portno = atoi(argv[3]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[2]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    n=write(sockfd,comando,strlen(comando));
    if (n < 0)
    error("ERROR writing to socket");

    bzero(buffer,TamBuffer);



    bzero(buffer,TamBuffer);
    n = read(sockfd,buffer,TamBuffer-1);
    if (n < 0)
         error("ERROR reading from socket");
    printf("%s\n",buffer);

    close(sockfd);
    }

    if (strcmp(argv[1],"get")==0){
    TamBuffer=atoi(argv[5]);
    char *comando = argv[1];
    portno = atoi(argv[4]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[3]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    strcat(comando,argv[2]);
    n=write(sockfd,comando,strlen(comando));

    if (n < 0)
    error("ERROR writing to socket");

/*
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
         error("ERROR reading from socket");
    printf("%s\n",buffer);
  */

     /* Receiving file size */
        recv(sockfd, buffer, TamBuffer, 0);
        file_size = atoi(buffer);
        fprintf(stdout, "\nFile size : %d\n", file_size);

        received_file = fopen("newfile", "w");
        if (received_file == NULL)
        {
                fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }

        remain_data = file_size;

        while (((len = recv(sockfd, buffer, TamBuffer, 0)) > 0) && (remain_data > 0))
        {
                fwrite(buffer, sizeof(char), len, received_file);
                remain_data -= len;
                int received_byte = len;
                fprintf(stdout, "Receive %d bytes and we hope :- %d bytes\n", received_byte, remain_data);
        }
        fclose(received_file);



    close(sockfd);



    }
    return 0;
}
