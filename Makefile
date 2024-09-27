all:
	make build && make start

build:
	g++ -std=c++20 ./src/GameController.cpp ./src/main.cpp -o zoltraak.exe -lsfml-graphics -lsfml-window -lsfml-system

start:
	./zoltraak.exe