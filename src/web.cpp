#include "web.hpp"
#ifdef _WIN32
#include <Winsock2.h>
#else 
#include <arpa/inet.h>
#endif

#include <mutex>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
 


#ifdef __cplusplus
extern "C" {
#endif



void diep(const char* s)
{
    perror(s);
    exit(1);
}

void recieve(Client& server, Message& message)
{
    struct sockaddr_in si_other;
    unsigned int slen=sizeof(si_other);
    if (recvfrom(server.socket, &message, sizeof(message), 0, 
                (struct sockaddr*)(&si_other), &slen)==-1)
    {
        diep("failed to recieve from socket");
    }
    message.host = si_other.sin_addr.s_addr;
    message.port = (short)si_other.sin_addr.s_addr;
}

Client init(char *IP, int port)
{
    struct sockaddr_in si_me, si_other;
    struct Client server;
    unsigned int slen=sizeof(si_other);
    struct Message b;

    if ((server.socket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
        diep("socket");
 
    // Our own endpoint data
    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(port); // This is not really necessary, we can also use 0 (any port)
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
 
    // The server's endpoint data
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(port);
    if (inet_aton(IP, &si_other.sin_addr)==0)
        diep("aton");
 
    // Store the server's endpoint data so we can easily discriminate between server and peer datagrams.
    server.host = si_other.sin_addr.s_addr;
    server.port = si_other.sin_port;
 
    // Send a simple datagram to the server to let it know of our public UDP endpoint.
    // Not only the server, but other clients will send their data through this endpoint.
    // The datagram payload is irrelevant, but if we wanted to support multiple
    // clients behind the same NAT, we'd send our won private UDP endpoint information
    // as well.
    if (sendto(server.socket, "hi", 2, 0, (struct sockaddr*)(&si_other), slen)==-1)
        diep("sendto");

    return server;
}  




#ifdef __cplusplus
}
#endif

UDP::UDP(char* IP, int port)
{
    
}
Message UDP::read()
{
    const std::lock_guard<std::mutex> lock(last_guard);
    return copy(last);
}

Message copy(Message& in)
{
    Message b;
    b.host = in.host;
    b.port = in.port;
    for(int i=0; i < 100; i++)
    {
        b.buf[i] = in.buf[i];
    }
    return b;
}

void* UDP::poll()
{
    while(1)
    {
        const std::lock_guard<std::mutex> lock(last_guard);
        recieve(server, last);
        if (server.host == last.host && server.port == last.port)
        {
            //The server sent us a communication
        }
        else
        {
            //A client sent a communication
        }
    }

}
