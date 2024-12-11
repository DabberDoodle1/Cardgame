all: all.o
	g++ cardgame.o game.o resource_manager.o shader.o texture.o sprite_renderer.o glad.o stb_image.o cards.o -o cardgame -ldl -lglfw && rm *.o && ./cardgame

all.o: src/cardgame.cpp src/game.cpp src/resource_manager.cpp src/shader.cpp src/texture.cpp src/sprite_renderer.cpp src/glad.c src/stb_image.c src/cards.cpp
	g++ -c src/cardgame.cpp src/game.cpp src/resource_manager.cpp src/shader.cpp src/texture.cpp src/sprite_renderer.cpp src/glad.c src/stb_image.c src/cards.cpp

clear: cardgame
	rm cardgame
