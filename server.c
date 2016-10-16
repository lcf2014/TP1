#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/sendfile.h>



void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{


     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     int sent_bytes = 0; // bytes de envio de arquivo
     char file_size[256]; //tamanho do arquivo
     struct stat file_stat;
     off_t offset;
     int remain_data;
     ssize_t len;
     int fd;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     if (newsockfd < 0)
          error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");

    if(strcmp("list",buffer)==0){

     DIR           *d;
     struct dirent *dir;
     d = opendir(".");
     if (d)
     {
         while ((dir = readdir(d)) != NULL)
        {
        n=write(newsockfd,dir->d_name,strlen(dir->d_name));
        n=write(newsockfd, "\n",1);
        if (n < 0) error("ERROR writing to socket\n");
       }

    closedir(d);
  }
}
else {
        n = read(newsockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");
        int i;
        for(i=0;i<strlen(buffer);i=i+1){
            buffer[i]=buffer[3+i];
        }
        printf("%s\n",buffer);

        char* FILE_TO_SEND = buffer;

        fd = open(FILE_TO_SEND, O_RDONLY);
        if (fd == -1)
        {
                fprintf(stderr, "Error opening file --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Get file stats */
        if (fstat(fd, &file_stat) < 0)
        {
                fprintf(stderr, "Error fstat --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }
        int num_filesize = file_stat.st_size;
        fprintf(stdout, "File Size: \n%d bytes\n", num_filesize);


        /* Sending file size */
        len = send(newsockfd, file_size, sizeof(file_size), 0);
        if (len < 0)
        {
              fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));

              exit(EXIT_FAILURE);
        }

        printf("teste1/n");

        int num_bytes_sent= len;
        fprintf(stdout, "Server sent %d bytes for the size\n", num_bytes_sent);

        offset = 0;
        remain_data = file_stat.st_size;
        /* Sending file data */

        printf("teste2/n");
        while (((sent_bytes = sendfile(newsockfd, fd, &offset, 256)) > 0) && (remain_data > 0))
        {
                fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
                remain_data -= sent_bytes;
                fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
        }



}
     close(newsockfd);
     close(sockfd);
     return 0;
}
