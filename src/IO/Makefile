tply:	tply.o PlyLoader.o
	g++ -std=c++11 -o tply tply.o PlyLoader.o

tply.o:	tply.cpp PlyLoader.hpp
	g++ -std=c++11 -c tply.cpp

PlyLoader.cpp:	PlyLoader.cpp PlyLoader.hpp
	g++ -std=c++11 -c PlyLoader.cpp

clean:
	rm -r *.o tply
