#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

#define MESSAGE_LENGTH 1024
#define PORT 7777

int socket_file_descriptior, connection;
struct sockaddr_in serveraddress, client;
char message[MESSAGE_LENGTH];

int main() {
    socket_file_descriptior = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptior == -1) {
        cout << "Creation of socket failed!" << endl;
        exit (1);
    }
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    connection = connect(socket_file_descriptior, (struct sockaddr*)&serveraddress,
    sizeof(serveraddress));
    if (connection == -1) {
        cout << "Connection with server failed!" << endl;
        exit(1);
    }
    // Взаимодействие с сервером
    while (1){
        bzero(message, sizeof(message));
        cout << "Enter the message you want to send to the server:" << endl;
        cin.getline(message, sizeof(message));
        if (strncmp(message, "end", 3) == 0) {
            write (socket_file_descriptior, message, sizeof(message));
            cout << "Client exit!" << endl;
            break;
        }
        ssize_t bytes = write (socket_file_descriptior, message, sizeof(message));
        if (bytes >= 0) {
            cout << "data send to the server successfully!" << endl;
        }
        bzero (message, sizeof(message));
        // Ждем ответа от сервера
        read(socket_file_descriptior, message, sizeof(message));
        cout << "Data received from server: " << message << endl; 
    }
    close(socket_file_descriptior);
    return 0;
}