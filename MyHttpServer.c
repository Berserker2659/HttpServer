#include <cstdint>



//in this structure the sa_family will be the AF_inet(IPv4) or AF_inet(IPv6)
//and the sa_data will hold the destination address and port number for the socket
struct sockaddr{
    unsigned short sa_family;//address family, AF_xxx
    char sa_data[14]; //14 bytes of protocol address
};
//This structure is only for ipv4, this is basically a replacement for the above struct
// but since it is annoying to have to set sa_Data[14] this will give a much more refined
//control it is the same size as the sockaddr structure.
struct sockaddr_ini{
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
struct addrinfo{
    int ai_flags; 
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    size_t ai_addrlen;
    struct sockaddr *ai_addr;
    char *ai_canonname;
    struct addrinfo *ai_next;
};
