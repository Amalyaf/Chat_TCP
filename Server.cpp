#include "Server.h"

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

void Server::Write(std::string msg)
{
     //msg.clear();
        // Ввод сообщения от сервера
        //std::cout << "Enter the message you want to send to the client: " << std::endl;
        //std::getline(std::cin, msg);  // Используем getline для ввода строки с пробелами
        
        // Отправка данных клиенту
        ssize_t bytes_sent = write(connection, msg.c_str(), msg.size());
        if (bytes_sent < 0) {
            std::cout << "Failed to send data to the client!" << std::endl;
        }
        //msg.clear();
}

std::string Server::Read()
{
    msg.clear();
        // Чтение данных от клиента
        char buffer[MESSAGE_LENGTH];
        bzero(buffer, sizeof(buffer));
        ssize_t bytes_received = read(connection, buffer, sizeof(buffer));
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';  // Завершаем строку нулевым символом
            msg = buffer;
            std::cout << msg << std::endl;
        } else {
            return "Error reading from client!";
        }
    return msg;
}

void Server::exit()
{
    close(socket_file_descriptior);
}
