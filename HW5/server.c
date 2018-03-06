// Chris Fietkiewicz. Demonstrates C sockets. Designed to work with client.c.
// Usage: server port
// Example: server 8000
// Based on socket example from http://www.linuxhowtos.org/C_C++/socket.htm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

// Helper function to conveniently print to stderr AND exit (terminate)
void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
     // Check for proper number of commandline arguments
     // Expect program name in argv[0], port # in argv[1]
     if (argc < 2) { 
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     // Setup phase
     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     struct sockaddr_in serv_addr; // Server address struct
     bzero((char *) &serv_addr, sizeof(serv_addr));
     int portno = atoi(argv[1]); // Port number is commandline argument
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
          error("ERROR on binding");
     listen(sockfd, 5);
     
     char buffer[256];
     // Service phase
     struct sockaddr_in cli_addr;
     socklen_t clilen = sizeof(cli_addr); // Address struct length
     int newsockfd = accept(sockfd, 
                     (struct sockaddr *) &cli_addr, 
                     &clilen);
     
     bzero(buffer, sizeof(buffer));
     //int n = read(newsockfd, buffer, sizeof(buffer));

     
     printf("listening for client...\n");
     sprintf(buffer, "How many fingers am I holding up?"); // send message
     int n = write(newsockfd, buffer, sizeof(buffer));
     if (n < 0) 
          error("ERROR writing to socket");

     if (newsockfd < 0) // recieve message
          error("ERROR on accept");



     bzero(buffer, strlen(buffer));
     n = read(newsockfd, buffer, sizeof(buffer));
         if (n < 0) 
          error("ERROR reading from socket");


     // print and write message
     if(atoi(buffer) == 11){
        printf("Client was Correct.\n");
        sprintf(buffer, "Correct. I was holding up 11 fingers.");
     }else{
        printf("Client was Incorrect.\n");
        sprintf(buffer, "Incorrect. I was holding up 11 fingers.");
     }
     


      // send message
     n = write(newsockfd, buffer, sizeof(buffer));
     if (n < 0) 
          error("ERROR writing to socket");
     close(newsockfd);
     close(sockfd);

     return 0; 
}
