all: obj/GameOfLife.o obj/Grid.o obj/Cell.o obj/main.o
	g++ obj/GameOfLife.o obj/Grid.o obj/Cell.o obj/main.o -lSDL2 -lSDL2_ttf -o bin/main

obj/main.o: src/main.cpp
	g++ -c -lSDL2 -lSDL2_ttf -Iinclude src/main.cpp -o obj/main.o

obj/GameOfLife.o: src/GameOfLife.cpp include/GameOfLife.hpp include/Color.hpp
	g++ -c -lSDL2 -lSDL2_ttf -Iinclude src/GameOfLife.cpp -o obj/GameOfLife.o

obj/Grid.o: src/Grid.cpp include/Grid.hpp include/Color.hpp
	g++ -c -lSDL2 -lSDL2_ttf -Iinclude src/Grid.cpp -o obj/Grid.o

obj/Cell.o: src/Cell.cpp include/Cell.hpp include/Color.hpp
	g++ -c -lSDL2 -lSDL2_ttf -Iinclude src/Cell.cpp -o obj/Cell.o
