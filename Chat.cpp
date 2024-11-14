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
}

Chat::~Chat() {
	writeUsers(); // метод для записи данных зарегистрированных пользователей в файл
	writeMessage(); // метод для записи личных и публичных сообщений в отдельные файлы
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
	char c = 'y';
	while (c != 'n')
	{
		try
		{
			cout << "\nДля входа введите логин: \n";
			cin >> _login;
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
				cout << "Введите пароль: \n";
				cin >> _password;
				
				if (user._password != _password)
				{
					throw BadPassword();
				}
				else
				{
					_status = true;
					c = 'n';
					printMessage(_login);
				}
			}
		}
		catch (BadLogin& e)
		{
			cout << e.what() << endl;
			cin >> c;
		}
		catch (BadPassword& e)
		{
			cout << e.what() << endl;
			cin >> c;
		}
	}
}

void Chat::registration()
{
	char c = 'y';
		while (c != 'n')
		{
			cout << "\nРегистрация нового пользователя\n";
			Users user;
			user.setUser();
			vector<Users>::iterator result = find(allUsers.begin(), allUsers.end(), user);
			if (result != allUsers.end())
			{
				cout << "\nПользователь с таким логином уже существует!\nХотите повторить попытку?(y/n)";
				cin >> c;
				cout << endl;
			}
			else
			{
				allUsers.push_back(user);
				c = 'n';
			}
		}
}

void Chat::sendPrivateMessage()
{
	char c = 'y';
	_sender = _login;
	while (c != 'n')
	{
		cout << "Кому: ";
		cin >> _recipient;
		Users user;
		user._login = _recipient;
		vector<Users>::iterator result = find(allUsers.begin(), allUsers.end(), user);
		if (result == allUsers.end())
		{
			cout << "\nПолучатель не найден!\nХотите повторить попытку?(y/n)";
			cin >> c;
		}

		else
		{
			Message message;
			message._recipient = _recipient;
			message._sender = _sender;
			cout << "\nВведите сообщение:\n";
			message.setMessage();
			allMessage.push_back(message);
			c = 'n';
		}
	}
}

void Chat::sendPublicMessage()
{
	Message message;
	message._sender = _login;
	cout << "\nВведите групповое сообщение:\n";
	message.setMessage();
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
				cout << "\n------------------------------------------------------\n";
				cout << "У вас есть новые личные сообщения" << ": ";
			}
			cout << "\nОтправитель: " << it->_sender << endl <<
				"Получатель: " << it->_recipient << endl <<
				"Сообщение: " << it->_message << endl;
		}
	}
	if (count != 0)
	{
		cout << "\n------------------------------------------------------\n";
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
				std::cout << "\n------------------------------------------------------\n";
				std::cout << "У вас есть новые общие сообщения" << ": ";
			}
			cout << "\nОтправитель: " << it->_sender << endl <<
				"Сообщение: " << it->_message << endl;
		}
	}
	if (count != 0)
	{
		cout << "\n------------------------------------------------------\n";
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
		cout << "\nОтправитель: " << it->_sender << endl <<
			"Получатель: " << it->_recipient << endl <<
			"Сообщение: " << it->_message << endl;
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
                cout << "No file" << endl;
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
                cout << "No file" << endl;
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
                cout << "No file" << endl;
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

 char c = 'y'; // условие выхода из цикла

	 if (getReadUsersStatus() == 1) { // если есть файл с данными о ранее зарегистрированных пользователях,
					 // то сначала спрашиваем о регистрации нового пользователя и в зависимости от ответа выполняем регистрацию
		getChat(); // вывод пользователей на экран, чтобы было видно логины и пароли 
		cout << "\nХотите зарегистрировать ещё одного пользователя?(y/n)" << endl;
		cin >> c;
	}
	while (c == 'y') {
		registration();
		cout << "\nХотите зарегистрировать ещё одного пользователя?(y/n)" << endl;
		cin >> c;
		cout << endl;
	}

        getChat();
        enter(); // авторизация
        c = 'y';
        while (c != 'n')
        {
                if (getstatus()) // проверяем был ли выполнен вход
                {
                        char message;
                        cout << "\nХотите отправить сообщение?(y/n)\n";
                        cin >> c;

                        if (c == 'y')
                        {
                                cout << "\nВыберите тип отправляемого сообщения: 1-private, 2-public\n";
                                cin >> message;
                                switch (message)
                                {
                                case '1':
                                        sendPrivateMessage();
                                        break;
                                case '2':
                                        sendPublicMessage();
                                        break;
                                default:
                                        cout << "\nНекорректный ввод!\n";
                                        break;
                                }
                        }
                        if (c == 'n')
                        {
                                cout << "\nХотите выполнить вход под другой учетной записью?(y/n)\n";
                                cin >> c;
                                if (c == 'y')
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
                        c = 'n';
                        cout << "\nВход не выполнен!\n";
                }
        }
}
