//  main.cpp
//  Cole Boyer, Patrick Landis, David Sherbinin
//  cojboyer@ucsc.edu, pdlandis@ucsc.edu, dsherbin@ucsc.edu
//  CMPS 109 Winter '17 Group Project
//
//  Entry point for program.

#include "main.h"


int main(int argc, const char* argv[]) {
    
    std::cout << "Program started. Type 'q' to quit.\n";
    SRI_CLI cli;
    cli.start();
    std::cout << "Program ending.\n";
    
    int sock; // Socket Handler
    struct sockaddr_in serverAddr; // Server Address Structure
    struct sockaddr_in clientAddr; // Client Address Structure
    socklen_t sin_size = sizeof(struct sockaddr_in); // Address Structur Size
    // Try to allocate a socket and if fail we terminate with an error.
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cout << "Failed to get socket.\n";
        return 0;
    }
    int sock_opt = 1; // An integer to be used with sock opt
    // Setting socket reuse option so after the program terminates we can acquire the socket again on the same port without going into a grace period.
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(void *)&sock_opt,sizeof (sock_opt));
    serverAddr.sin_family = AF_INET; // Set the communication family
    serverAddr.sin_port = htons(9999); // Set the port number
    serverAddr.sin_addr.s_addr = INADDR_ANY;// Set the socket to operate on all IP address and network interfaces available
    bzero(&(serverAddr.sin_zero), 8); // We zero out the rest of the sockaddr_in to make it identical to sockaddr
    
    bind(sock, (struct sockaddr *) &serverAddr, sizeof(struct sockaddr));
    // Create the socket listen queue with size 10.
    std::cout << "Listening on port 9999\n";
    if (listen(sock, 10) == -1) {
        perror("Error listening.\n");
        return 0;
    }
    
    int newsock = accept(sock, (struct sockaddr *) &clientAddr, &sin_size); // Wait for connection
    if ( newsock < 1 ) {
        perror("Error Accepting Socket\n"); // if failed exit.
        return 0;
    }
    else {
        int maxBytes = 1024; // size of buffer
        char buffer[maxBytes]; // alocat buffer of 1 K
        memset(buffer,0,maxBytes); // initialize it.
        int bytes_read = recv (newsock,buffer,maxBytes,0); // read data from the socket
        // if returned number of bytes is bigger that zero then print information about client and the message
        if ( bytes_read > 0 ) printf ("Received Message from %s:%d\n%s\n",(char*)inet_ntoa(clientAddr.sin_addr),clientAddr.sin_port,buffer);
        else perror("Error Receiving Message:"); // else print error through perror.
        close(newsock);// close the new socket
    }
    close(sock); // close the server socket
    
    return 0;
}
