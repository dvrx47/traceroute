FILE_OUT = traceroute.prog
FILE_IN = main.o obsluga.o
CC = g++
FLAGS = -std=c++11 -Wall -Werror

$(FILE_OUT): $(FILE_IN)
	$(CC) $(FILE_IN) $(FLAGS) -o $(FILE_OUT)

main.o: main.cpp
	$(CC) -c main.cpp

obsluga.o: obsluga.cpp obsluga.hpp
	$(CC) -c obsluga.cpp obsluga.hpp

clean:
	rm *.o *.gch

distclean:
	rm *.o *.gch *.prog
