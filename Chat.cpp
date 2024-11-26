#include "Chat.h"
#include <algorithm>
#include "bad_login.h"
#include "bad_password.h"
#include <filesystem>
#include <fstream>
#include <cstdlib>

using namespace std;

Chat::Chat()
{
	vector<Users> allUsers;
	// если есть файл с данными ранее зарегистрированных пользователей, то вызвать методы для считывания данных из файлов
	if (getReadUsersStatus() == 1) {
		readUsers();
		readPrivateMessage();
		readPublicMessage();
	}
	if (server.init() == 0){
		std::cout << "Server successfully connected!" << std::endl;
	}
	else {
		std::cout << "Server not connected!" << std::endl;
	}
}

Chat::~Chat() {
	writeUsers(); // метод для записи данных зарегистрированных пользователей в файл
	writeMessage(); // метод для записи личных и публичных сообщений в отдельные файлы
	server.exit();
}


void Chat::getChat()
{
	for (vector<Users>::iterator it = allUsers.begin(); it < allUsers.end(); it++)
	{
		cout << *it << endl;
	}
}

void Chat::enter()
{
	string c = "y";
	while (c != "n")
	{
		try
		{
			server.Write("Введите логин:");
			_login = server.Read();
			Users user;
			user._login = _login;
			vector<Users>::iterator result = find(allUsers.begin(), allUsers.end(), user);

			if (result == allUsers.end())
			{
				throw BadLogin();
			}

			else
			{
				user = *result;
				server.Write("Введите пароль:");
				_password = server.Read();
				
				if (user._password != _password)
				{
					throw BadPassword();
				}
				else
				{
					_status = true;
					c = "n";
					printMessage(_login);
				}
			}
		}
		catch (BadLogin& e)
		{
			server.Write(e.what());
			c = server.Read();
		}
		catch (BadPassword& e)
		{
			server.Write(e.what());
			c = server.Read();
		}
	}
}

void Chat::registration()
{
	string c = "y";
		while (c != "n")
		{
			server.Write("\nРегистрация нового пользователя:\n");
			Users user;
			server.Write("Введите логин: ");
			user.setLogin(server.Read());
			server.Write("Введите пароль: ");
			user.setPassword(server.Read());
			server.Write("Введите имя: ");
			user.setName(server.Read());
			vector<Users>::iterator result = find(allUsers.begin(), allUsers.end(), user);
			if (result != allUsers.end())
			{
				server.Write("\nПользователь с таким логином уже существует!\nХотите повторить попытку?(y/n)");
				c = server.Read();
			}
			else
			{
				allUsers.push_back(user);
				c = "n";
			}
		}
}

void Chat::sendPrivateMessage()
{
	string c = "y";
	_sender = _login;
	while (c != "n")
	{
		server.Write("Кому: ");
		_recipient = server.Read();
		Users user;
		user._login = _recipient;
		vector<Users>::iterator result = find(allUsers.begin(), allUsers.end(), user);
		if (result == allUsers.end())
		{
			server.Write("\nПолучатель не найден!\nХотите повторить попытку?(y/n)");
			c = server.Read();
		}

		else
		{
			Message message;
			message._recipient = _recipient;
			message._sender = _sender;
			server.Write("\nВведите сообщение:\n");
			message.setMessage(server.Read());
			allMessage.push_back(message);
			c = "n";
		}
	}
}

void Chat::sendPublicMessage()
{
	Message message;
	message._sender = _login;
	server.Write("\nВведите групповое сообщение:\n");
	message.setMessage(server.Read());
	for (vector<Users>::iterator it = allUsers.begin(); it < allUsers.end(); it++)
	{
		if (it->_login != _login)
		{
			message._recipient = it->_login;
			allPublicMessage.push_back(message);
		}
	}

	message._recipient = "all";
	viewedMessage.push_back(message);
}


void Chat::printMessage(string recipient)
{
	int count = 0;
	for (vector<Message>::iterator it = allMessage.begin(); it < allMessage.end(); it++)
	{
		if (it->_recipient == recipient)
		{
			count++;
			if (count == 1)
			{
				server.Write("\n------------------------------------------------------\n");
				server.Write("У вас есть новые личные сообщения: ");
			}
			server.Write("\nОтправитель: " );
			server.Write(it->_sender);
			server.Write("\nПолучатель: " );
			server.Write(it->_recipient);
			server.Write("\nСообщение: ");
			server.Write(it->_message);
			server.Write("\n");
		}
	}
	if (count != 0)
	{
		server.Write("\n------------------------------------------------------\n");
		deletePrivateMessage(recipient);
	}
	count = 0;
	for (vector<Message>::iterator it = allPublicMessage.begin(); it < allPublicMessage.end(); it++)
	{
		if (it->_recipient == recipient)
		{
			count++;
			if (count == 1)
			{
				server.Write("\n------------------------------------------------------\n");
				server.Write("У вас есть новые общие сообщения: ");
			}
			server.Write("\nОтправитель: ");
			server.Write(it->_sender);
			server.Write("\nСообщение: ");
			server.Write(it->_message);
			server.Write("\n");
		}
	}
	if (count != 0)
	{
		server.Write("\n------------------------------------------------------\n");
		deletePublicMessage(recipient);
	}
}

void Chat::deletePrivateMessage(std::string recipient)
{
	for (vector<Message>::iterator it = allMessage.begin(); it < allMessage.end();)
	{
		if (it->_recipient == recipient)
		{
			viewedMessage.push_back(*it);
			it = allMessage.erase(it);
		}
		else
			++it;
	}
}

void Chat::deletePublicMessage(std::string recipient)
{
	for (vector<Message>::iterator it = allPublicMessage.begin(); it < allPublicMessage.end();)
	{
		if (it->_recipient == recipient)
		{
			it = allPublicMessage.erase(it);
		}
		else
			++it;
	}
}

void Chat::printAllMessage()
{
	for (vector<Message>::iterator it = viewedMessage.begin(); it != viewedMessage.end(); it++)
	{
			server.Write("\nОтправитель: " );
			server.Write(it->_sender);
			server.Write("\n");
			server.Write("Получатель: " );
			server.Write(it->_recipient);
			server.Write("\n");
			server.Write("Сообщение: ");
			server.Write(it->_message);
			server.Write("\n");
	}
	cout << endl;
}

bool Chat::getstatus()
{
	return _status;
}

void Chat::exit()
{
	_status = false;
	_login.clear();
	_password.clear();
}

void Chat::readUsers() {
        
	fstream file = fstream(userData, ios::in);

        if (!file)
        {
                server.Write("No file");
                return;
        }
      	
        const string delimiter = ":";
        
	if (file.is_open()) {
		string line;
		string title;
                string value;
		int user_cnt = 0;

		fstream user_cnt_file (user_count, ios::in);
		if (user_cnt_file.is_open()) {
			string line;
			getline(user_cnt_file, line);
			user_cnt = stoi(line);
		}
		for (int i = 0; i < user_cnt; i++) {
			Users user;
			int count = 0;
        	while (count < 3 && getline(file, line)) {
				size_t delimiterPosition = line.find(delimiter);
                		if (delimiterPosition > 0) {
                       			title = line.substr (0, delimiterPosition);
                       			value = line.substr (delimiterPosition + 1);

		       			if (title == "Name") {
                           			user._name = value;
                        		}
                       			else if (title == "Login") {
                           			user._login = value;
                        		}
                       			else if (title == "Password") {
                           			user._password = value;
                        		}
				       count++;
				}
			}
			allUsers.push_back(user);
        	}
        	file.close();
	}
}

int Chat::getReadUsersStatus() {
	
	fstream file (userData, ios::in);
	if (!file) {
        	return -1;
	}
	
	fstream user_cnt_file (user_count, ios::in);
	if (!user_cnt_file) {
		return -1;
	}

	else if (user_cnt_file.is_open()) {
		string line;
		int user_cnt = 0;
                getline(user_cnt_file, line);
                user_cnt = stoi(line);
		if (user_cnt == 0) {
			return -1;
		}
	}
		
	return 1;
}

void Chat::writeUsers() const {

	fstream file = fstream(userData, ios::in | ios::out | ios::trunc);

        auto permissions = filesystem::perms::group_all | filesystem::perms::others_all; // права, которые хотим отобрать
        filesystem::permissions (userData, permissions, filesystem::perm_options::remove);

	for (int i = 0; i < allUsers.size(); i++)
        {
                file << "Name:" << allUsers[i]._name << "\n";
        	file << "Login:" << allUsers[i]._login << "\n";
        	file << "Password:" << allUsers[i]._password << "\n";
        }

        file.close();
	
	fstream user_cnt_file = fstream(user_count, ios::in | ios::out | ios::trunc);
        filesystem::permissions (user_count, permissions, filesystem::perm_options::remove);

	user_cnt_file << allUsers.size();
	user_cnt_file.close();
}

void Chat::writeMessage() const {
	
	fstream file_prvt_msg = fstream(private_message, ios::in | ios::out | ios::trunc);
	
	auto permissions = filesystem::perms::group_all | filesystem::perms::others_all; // права, которые хотим отобрать
	filesystem::permissions (private_message, permissions, filesystem::perm_options::remove);

	for (int i = 0; i < allMessage.size(); i++) {
		file_prvt_msg << "Recipient:" << allMessage[i]._recipient << "\n";
		file_prvt_msg << "Sender:" << allMessage[i]._sender << "\n";
		file_prvt_msg << "Message:" << allMessage[i]._message << "\n";
	}
	file_prvt_msg.close();


	fstream file_pblc_msg = fstream(public_message, ios::in | ios::out | ios::trunc);
	filesystem::permissions (public_message, permissions, filesystem::perm_options::remove);
	
	for (int i = 0; i < allPublicMessage.size(); i++) {
		
		file_pblc_msg << "Recipient:" << allPublicMessage[i]._recipient << "\n";
                file_pblc_msg << "Sender:" << allPublicMessage[i]._sender << "\n";
                file_pblc_msg << "Message:" << allPublicMessage[i]._message << "\n";
	}

	file_pblc_msg.close();

	fstream prvt_msg_cnt_file = fstream(prvt_msg_count, ios::in | ios::out | ios::trunc);
	filesystem::permissions (prvt_msg_count, permissions, filesystem::perm_options::remove);

        prvt_msg_cnt_file << allMessage.size();
        prvt_msg_cnt_file.close();

	fstream pblc_msg_cnt_file = fstream(pblc_msg_count, ios::in | ios::out | ios::trunc);
        filesystem::permissions (pblc_msg_count, permissions, filesystem::perm_options::remove);

        pblc_msg_cnt_file << allPublicMessage.size();
        pblc_msg_cnt_file.close();	
}

void Chat::readPrivateMessage() {
        
	fstream file = fstream(private_message, ios::in);

        if (!file)
        {
                server.Write("No file\n");
                return;
        }
        
	const string delimiter = ":";
        
	if (file.is_open()) {
                string line;
                string title;
                string value;
                int prvt_msg_cnt = 0;

                fstream prvt_msg_cnt_file (prvt_msg_count, ios::in);
                if (prvt_msg_cnt_file.is_open()) {
                        string line;
                        getline(prvt_msg_cnt_file, line);
                        prvt_msg_cnt = stoi(line);
                }
                for (int i = 0; i < prvt_msg_cnt; i++) {
                        Message msg;
                        int count = 0;
                while (count < 3 && getline(file, line)) {
                                size_t delimiterPosition = line.find(delimiter);
                                if (delimiterPosition > 0) {
                                        title = line.substr (0, delimiterPosition);
                                        value = line.substr (delimiterPosition + 1);

                                        if (title == "Recipient") {
                                                msg._recipient = value;
                                        }
                                        else if (title == "Sender") {
                                                msg._sender = value;
                                        }
                                        else if (title == "Message") {
                                                msg._message = value;
                                        }
                                       count++;
                                }
                        }
                        allMessage.push_back(msg);
                }
                file.close();
        }
}


void Chat::readPublicMessage() {

        fstream file = fstream(public_message, ios::in);

        if (!file)
        {
                server.Write("No file");
                return;
        }

        const string delimiter = ":";

        if (file.is_open()) {
                string line;
                string title;
                string value;
                int pblc_msg_cnt = 0;

                fstream pblc_msg_cnt_file (pblc_msg_count, ios::in);
                if (pblc_msg_cnt_file.is_open()) {
                        string line;
                        getline(pblc_msg_cnt_file, line);
                        pblc_msg_cnt = stoi(line);
                }
                for (int i = 0; i < pblc_msg_cnt; i++) {
                        Message msg;
                        int count = 0;
                while (count < 3 && getline(file, line)) {
                                size_t delimiterPosition = line.find(delimiter);
                                if (delimiterPosition > 0) {
                                        title = line.substr (0, delimiterPosition);
                                        value = line.substr (delimiterPosition + 1);

                                        if (title == "Recipient") {
                                                msg._recipient = value;
                                        }
                                        else if (title == "Sender") {
                                                msg._sender = value;
                                        }
                                        else if (title == "Message") {
                                                msg._message = value;
                                        }
                                       count++;
                                }
                        }
                        allPublicMessage.push_back(msg);
                }
                file.close();
        }
}


void Chat::start() {

 string c = "y"; // условие выхода из цикла

	 if (getReadUsersStatus() == 1) { // если есть файл с данными о ранее зарегистрированных пользователях,
					 // то сначала спрашиваем о регистрации нового пользователя и в зависимости от ответа выполняем регистрацию
		getChat(); // вывод пользователей на экран, чтобы было видно логины и пароли 
		server.Write("\nХотите зарегистрировать ещё одного пользователя?(y/n)\n");
		c = server.Read();
	}
	while (c == "y") {
		registration();
		server.Write("\nХотите зарегистрировать ещё одного пользователя?(y/n)\n");
		c = server.Read();
	}

        getChat();
        enter(); // авторизация
        c = "y";
        while (c != "n")
        {
                if (getstatus()) // проверяем был ли выполнен вход
                {
                        char message;
						string m;
                        server.Write("\nХотите отправить сообщение?(y/n)\n");
                        c = server.Read();

                        if (c == "y")
                        {
                                server.Write("\nВыберите тип отправляемого сообщения: 1-private, 2-public\n");
                                m = server.Read();
								if (m == "1") {
									message = '1';
								}
								if (m == "2") {
									message = '2';
								}

                                switch (message)
                                {
                                case '1':
                                        sendPrivateMessage();
                                        break;
                                case '2':
                                        sendPublicMessage();
                                        break;
                                default:
                                        server.Write("\nНекорректный ввод!\n");
                                        break;
                                }
                        }
                        if (c == "n")
                        {
                                server.Write("\nХотите выполнить вход под другой учетной записью?(y/n)\n");
                                c = server.Read();
                                if (c == "y")
                                {
                                        exit();
                                        enter();
                                }
                                else
                                {
                                        break;
                                }
                        }
                }
                else
                {
                        c = "n";
                        server.Write("\nВход не выполнен!\n");
                }
        }
}
