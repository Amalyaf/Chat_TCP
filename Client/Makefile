TARGET = client
SRC = main.cpp
LIB = Client

main: $(SRC) lib
	g++ -o $(TARGET) $(SRC) -L. -l$(LIB)
lib: $(LIB).cpp $(LIB).h
	g++ -o $(LIB).o $(LIB).cpp -c
	ar rc lib$(LIB).a $(LIB).o
clean:
	rm *.o *.a

