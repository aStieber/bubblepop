// bubblepop.cpp : Defines the entry point for the console application.
//
#include <SFML/Graphics.hpp>
#include <vector>
#include "nodeGraph.h"
#include "graphics.h"


class game{
public:
	game();
	const int BOARD_WIDTH = 16;
	const int BOARD_HEIGHT = 12;
	nodeGraph mNodeGraph;
};

game::game() {
	nodeGraph mNodeGraph(BOARD_WIDTH, BOARD_HEIGHT);
	mNodeGraph.populateAdjacencyMasterList(BOARD_WIDTH, BOARD_HEIGHT);
	mNodeGraph.populateInitialGraph(BOARD_WIDTH, BOARD_HEIGHT);
}

int main()
{
	game theGame;
	graphics g(&theGame);
	g.runGraphicsLoop();

	return 0;
}

/*
window.clear();
window.draw(shape);
window.display();
}*/