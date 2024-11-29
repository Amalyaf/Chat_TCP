#pragma once
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MESSAGE_LENGTH 1024 // max size of bufer
#define PORT 7777

class Client {
private:
    struct sockaddr_in serveraddress;
    struct sockaddr_in client;
    int socket_file_descriptor;
    int connection;
    std::string message;
    std::string status;
public:
    Client();
    ~Client();
    int init();
    void Write();
    void Read();
    void close_socket();
    void start();
};