#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MESSAGE_LENGTH 1024 // max size of bufer
#define PORT 7777



class Server {
private:
    struct sockaddr_in serveraddress;
    struct sockaddr_in client;
    socklen_t length;
    int socket_file_descriptior;
    int connection;
    int bind_status;
    int  connection_status;
    std::string msg;
public:
    Server();
    ~Server();
    int init();
    void Write(std::string text);
    std::string Read();
    void exit();
};