/*
   Jim Patrizi
   jdp99
   HW4 Q1 Server
   A simple server in the internet domain using TCP
   and asks holds the answer to how many quarters are in a dollar
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");

     if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) //fixes error on binding address already in use error
       error("setsockopt(SO_REUSEADDR) failed");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno =  3380;//change to this for atoi(argv[1]); fixed port for HW4
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     printf("Listening for client...please wait\n");
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
     int answer;
     answer = atoi(buffer);
     if(answer != 4)
      {
        printf("Client was incorrect, WRONG!, make sure to use integer value, from server\n");
        send(newsockfd, "incorrect", strlen("incorrect"), 0);
      }
      else
      {
        printf("Client was CORRECT! Answer was indeed, from server %d\n", answer);
        send(newsockfd, "correct", strlen("correct"), 0);
      }
     if (n < 0) error("ERROR writing to socket");

     close(newsockfd);
     close(sockfd);
     return 0;
}
