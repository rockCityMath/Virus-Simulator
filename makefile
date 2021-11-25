#Compiler Variables
CC=g++
CFLAGS := -std=c++11 -W

#Compile all Object Files
output : init.o main.o sim.o distributions.o
	$(CC) $(CFLAGS) init.o main.o sim.o distributions.o -o output

#Source Object Files
init.o : src/init.cpp include/init.h
	$(CC) -c src/init.cpp

main.o : src/main.cpp
	$(CC) -c src/main.cpp

sim.o : src/sim.cpp include/sim.h include/init.h
	$(CC) -c src/sim.cpp

distributions.o : src/distributions.cpp include/sim.h
	$(CC) -c src/distributions.cpp

#Remove output file and .o files
clean:
	rm output *.o
