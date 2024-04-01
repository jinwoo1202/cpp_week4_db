FLAGS = -Werror -std=c++11

main.exe: main.o database.o database.h
	g++ -o main.exe main.o database.o

main.o: main.cpp
	g++ ${FLAGS} -c main.cpp

database.o: database.cpp database.h
	g++ ${FLAGS} -c database.cpp

clean:
	rm *.o
	rm main