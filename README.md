Создан сетевой чат, который содержит две программы:
1. Программа на стороне сервера, которая хранится в папке Server.
2. Программа на стороне клиента - Client.

Для начала работы необходимо в папке Server запустить синтетическую цель create (make create). Далее выполнить команду make в паке Server, а затем в папке Client.
Весь программный код чата реализован на стороне сервера. Клиент не имеет доступа к нему. От клиента сервер получает только необходимую информацию и далее сам взаимодействует с программой.
