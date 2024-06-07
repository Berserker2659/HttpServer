#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/*
//in this structure the sa_family will be the AF_inet(IPv4) or AF_inet(IPv6)
//and the sa_data will hold the destination address and port number for the socket
struct sockaddr{
    unsigned short sa_family;//address family, AF_xxx
    char sa_data[14]; //14 bytes of protocol address
};
//This structure is only for ipv4, this is basically a replacement for the above struct
// but since it is annoying to have to set sa_Data[14] this will give a much more refined
//control it is the same size as the sockaddr structure.
struct sockaddr_in{
    short int sin_family;
    unsigned short int sin_port;
    struct in_addr sin_addr;
    unsigned char sin_zero[8];  //this is to padd to match the size with the sockaddr struct.
};
struct in_addr{
    uint32_t s_addr;
};
//This structure is only for ipv6
struct sockaddr_in6 {
    u_int16_t       sin6_family;   // address family, AF_INET6
    u_int16_t       sin6_port;     // port number, Network Byte Order
    u_int32_t       sin6_flowinfo; // IPv6 flow information
    struct in6_addr sin6_addr;     // IPv6 address
    u_int32_t       sin6_scope_id; // Scope ID
};
struct in6_addr {
    unsigned char   s6_addr[16];   // IPv6 address
};
//this code can be ip agnostic with the help of the ai_family field(we can leave it unused to use AF_UNSPEC)
*/

//this program's intent is to take a valid HTTP request from a client and serve back a GIF to that client
int main(void) {
    struct sockaddr_storage client_addr;
    // first course of action is to set up a server socket to start listening for incoming connections
    // since i want to make this server ipv4/6 agnostic first we have to use getaddrinfo()
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // allow both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; // make this socket a TCP one and not an UDP one
    hints.ai_flags = AI_PASSIVE;

    // we need to use the socket call which has the following function prototype
    // int socket(int domain, int type, int protocol); it returns -1 on error or socket descriptor
    int serverFileDescriptor;

    // first we get the address info
    if (getaddrinfo(NULL, "9999", &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }
    // we will have to loop through all the results and bind to the first one we can and
    // as we know that the addrinfo structure is also a linked list we traverse it as that only
    for (p = res; p != NULL; p = p->ai_next) {
        // creating the socket
        if ((serverFileDescriptor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }
        // binding the socket
        if (bind(serverFileDescriptor, p->ai_addr, p->ai_addrlen) == -1) {
            perror("bind");
            close(serverFileDescriptor);
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "Failed to BIND to the socket\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);

    // NOW we have to start making the socket to start listening
    if (listen(serverFileDescriptor, 10)) { // the 10 over here stands for how many incoming 
        // connections can be allowed on the incoming queue. So 10 requests are max in queue
        perror("listen");
        close(serverFileDescriptor);
        exit(EXIT_FAILURE);
    }

    // NOW we need to use the accept call, we can put it in a loop so that it keeps on accepting as many connections as required;
    while (1) {
        socklen_t addrSize = sizeof(client_addr);
        int NewFD;
        if ((NewFD = accept(serverFileDescriptor, (struct sockaddr *)&client_addr, &addrSize)) == -1) {
            perror("accept");
            continue;
        }
        
        // Handle client logic here (not provided in your code snippet)
        
        close(NewFD);
    }
    close(serverFileDescriptor); // Close the server socket before exiting
    return 0;
}
