#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "nodeGraph.h"
#include "graphics.h"
#include "game.h"


int main()
{
	srand(time(NULL));
	sf::RenderWindow window;
	game theGame;

	graphics g(&theGame, &window);
	g.runGraphicsLoop();
	return 0;
}

/*
window.clear();
window.draw(shape);
window.display();
}*/