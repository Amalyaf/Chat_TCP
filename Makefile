TARGET = main
SRC = main.cpp
LIB = Chat
PREFIX = /tmp/Data

main: $(SRC) lib
	g++ -o $(TARGET) $(SRC) -L. -l$(LIB)
lib: $(LIB).cpp $(LIB).h
	g++ -o $(LIB).o $(LIB).cpp -c
	g++ -o Users.o Users.cpp -c
	g++ -o bad_login.o bad_login.cpp -c
	g++ -o bad_password.o bad_password.cpp -c
	ar rc lib$(LIB).a $(LIB).o Users.o bad_login.o bad_password.o
clean:
	rm *.o *.a
create:
	mkdir $(PREFIX) 
