CC = g++
CFLAG = -Wall -std=c++0x -O3
DEBUG = -g

dld: dld.o fps.o dao.o
	${CC} ${CFLAG} ${DEBUG} -o dld dld.o fps.o dao.o

dld.o: dld.cpp
	${CC} ${CFLAG} ${DEBUG} -c dld.cpp

fps.o: fps.cpp
	${CC} ${CFLAG} ${DEBUG} -c fps.cpp

dao.o: dao.cpp
	${CC} ${CFLAG} ${DEBUG} -c dao.cpp

clean:
	rm -rf *.o dld
