g++ -O2 -c search.cpp -o search.o
g++ -O2 -c position.cpp -o position.o
g++ --std=c++0x -O2 -c main.cpp -o main.o
g++ search.o position.o main.o -o 15solver
rm -f *.o
