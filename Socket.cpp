#include "Socket.h"

Server::Server()
{
}

Server::~Server()
{
}

int Server::init()
{
    socket_file_descriptior = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptior == -1){
        std::cout << "Socket creation failed!" << std::endl;
        return 1;
    }
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // Зададим номер порта для связи
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Привяжем сокет
    bind_status = bind(socket_file_descriptior, (struct sockaddr*)&serveraddress,
    sizeof(serveraddress));
    if (bind_status == -1) {
        std::cout << "Socket binding failed!" << std::endl;
        return 1;
    }
    // Поставим сервер на прием данных 
    connection_status = listen(socket_file_descriptior, 5);
    if (connection_status == -1) {
        std::cout << "Socket is unable to listen for new connections!" << std::endl;
        return 1;
    }
    else {
        std::cout << "Server is listening for new connection:" << std::endl;
    }
    length = sizeof(client);
    connection = accept(socket_file_descriptior, (struct sockaddr*)&client, &length);
    if (connection == -1) {
        std::cout << "Server is unable to accept the data from client!" << std::endl;
        return 1;
    }
    return 0;
}

void Server::Write(char text[], int size)
{
     //bzero(msg, sizeof(MESSAGE_LENGTH));
        ssize_t bytes = write(connection, text, sizeof(size));
        // Если передали >= 0  байт, значит пересылка прошла успешно
        if(bytes >= 0)  {
           std::cout << "Data successfully sent to the client.!" << std::endl;
        }

}

std::string Server::Read()
{
   bzero(msg, sizeof(msg));
        read(connection, msg, sizeof(msg));
            if (strncmp("end", msg, 3) == 0) {
                std::cout << "Client Exited." << std::endl;
                std::cout << "Server is Exiting..!" << std::endl;
                exit();
            }
        std::cout << "Data received from client: " <<  msg << std::endl;
        result = msg;
    return result;
}

void Server::exit()
{
    close(socket_file_descriptior);
}
