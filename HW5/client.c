// Chris Fietkiewicz. Demonstrates C sockets. Designed to work with server.c.
// Usage: client ipaddress port
// Example: client 127.0.0.1 8000
// Based on socket example from http://www.linuxhowtos.org/C_C++/socket.htm

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

// Helper function to conveniently print to stderr AND exit (terminate)
void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    // Check for proper number of commandline arguments
    // Expect program name in argv[0], IP address in argv[1], and port # in argv[2]
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    // Get socket
    int portno = atoi(argv[2]);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    // Set up for connect()
    struct hostent *server;
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // Make connection
    if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    char buffer[256];
    //int n = write(sockfd, buffer, sizeof(buffer));

    // Get question from server
    bzero(buffer, strlen(buffer));
    int n = read(sockfd, buffer, sizeof(buffer));
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n", buffer);

    // Get user input and write (send) to server
    bzero(buffer, sizeof(buffer));
    fgets(buffer, sizeof(buffer), stdin);
    n = write(sockfd, buffer, sizeof(buffer));
    if (n < 0) 
         error("ERROR writing to socket");

    // Get response from server (must give us a response!)
    bzero(buffer, strlen(buffer));
    n = read(sockfd, buffer, sizeof(buffer));
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n", buffer);

    close(sockfd);
}
