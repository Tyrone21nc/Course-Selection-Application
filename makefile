CXX = g++
CXXFLAGS = -Wall


run: Student.o main.cpp
        $(CXX) $(CXXFLAGS) Student.cpp main.cpp -o run

clean:
        rm *.o
        rm *~
val:
        valgrind ./run