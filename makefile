run:
	g++ -o game ./src/Maps.cpp ./src/Position.cpp ./src/Items.cpp ./src/Game.cpp ./src/Vector.cpp ./src/Snake.cpp -lncurses
	./game
