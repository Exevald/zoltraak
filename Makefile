all:
	make build && make start

build:
	g++ -std=c++20 ./src/CGameController.cpp ./src/main.cpp -o zoltraak.exe -lsfml-graphics -lsfml-window -lsfml-system

start:
	./zoltraak.exe

graphics_task_1:
	g++ -std=c++20 ./labs/graphics/task_1.cpp -o task_1.exe -lsfml-graphics -lsfml-window -lsfml-system
	./task_1.exe

graphics_task_2:
	g++ -std=c++20 ./labs/graphics/task_2.cpp -o task_2.exe -lsfml-graphics -lsfml-window -lsfml-system
	./task_2.exe

graphics_task_3:
	g++ -std=c++20 ./labs/graphics/task_3.cpp -o task_3.exe -lsfml-graphics -lsfml-window -lsfml-system
	./task_3.exe