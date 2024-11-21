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

animation_task_1:
	g++ -std=c++20 ./labs/animation/task_1.cpp -o task_1.exe -lsfml-graphics -lsfml-window -lsfml-system
	./task_1.exe

animation_task_2:
	g++ -std=c++20 ./labs/animation/task_2.cpp -o task_2.exe -lsfml-graphics -lsfml-window -lsfml-system
	./task_2.exe

events_task_1:
	g++ -std=c++20 ./labs/events/task_1.cpp -o task_1.exe -lsfml-graphics -lsfml-window -lsfml-system
	./task_1.exe

events_task_2:
	g++ -std=c++20 ./labs/events/task_2.cpp -o task_2.exe -lsfml-graphics -lsfml-window -lsfml-system
	./task_2.exe

complex_tasks_task_1:
	g++ -std=c++20 ./labs/complex_tasks/task_1.cpp -o task_1.exe -lsfml-graphics -lsfml-window -lsfml-system
	./task_1.exe

complex_tasks_task_2:
	g++ -std=c++20 ./labs/complex_tasks/task_2.cpp -o task_2.exe -lsfml-graphics -lsfml-window -lsfml-system
	./task_2.exe