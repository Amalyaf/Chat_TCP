#include "Client.h"

Client::Client()
{
}

Client::~Client()
{
}

int Client::init()
{
     // Создадим сокет
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1) {
        std::cout << "Creation of Socket failed!" << std::endl;
        return 1;
    }

    // Установим адрес сервера
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Зададим номер порта
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;

    // Установим соединение с сервером
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (connection == -1) {
        std::cout << "Connection with the server failed!" << std::endl;
        return 1;
    }
    return 0;
}

void Client::Write()
{
    message.clear();
        std::getline(std::cin, message);  // Для ввода строки с пробелами

       if (message == "end") {
            write(socket_file_descriptor, message.c_str(), message.size());
            std::cout << "Client Exit." << std::endl;
            close_socket();
        }
        ssize_t bytes = write(socket_file_descriptor, message.c_str(), message.size());
}

void Client::Read()
{
    message.clear();
    // Буфер для получения данных от сервера
    char buffer[MESSAGE_LENGTH];
    bzero(buffer, sizeof(buffer));
    ssize_t bytes_received = read(socket_file_descriptor, buffer, sizeof(buffer));
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';  // Завершаем строку нулевым символом
        message = buffer;

        if (message == "\nВход не выполнен!\n" || message == "Exit") {
            status = "Exit";
            //close_socket();
            return;
        }
        std::cout << message;
    }
    else {
        std::cout << "Failed to receive data from the server." << std::endl;
    }
}

void Client::close_socket()
{
    std::cout << "Close socket!" << std::endl;
    close(socket_file_descriptor);
}

void Client::start()
{
    init();
    while(true) {
        Read();
        if (status == "Exit"){
            break;
        }
        Write();
    }
    close_socket();
}
