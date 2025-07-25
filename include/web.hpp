#ifndef WEB_HPP_INCLUDED
#define WEB_HPP_INCLUDED

#include "glm/fwd.hpp"
#include <mutex>

#ifdef __cplusplus
extern "C" {
#endif

struct Client
{
    int socket;
    int host;
    short port;
};

struct Message
{
    int host;
    short port;
    char buf[100];
};
 
Client init(char *IP, int port);

Message copy(Message&);


#ifdef __cplusplus
}
#endif

struct Data
{
public:
    Data(glm::vec3 position);
};

class UDP
{
public:
    UDP() = delete;
    UDP(char* IP, int port);
    ~UDP();
    Client server;

    void send();
    Message read();

private:
    std::mutex last_guard;
    Message last;

    void* poll();

};



#endif
