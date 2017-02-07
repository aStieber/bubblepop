// bubblepop.cpp : Defines the entry point for the console application.
//
#include <SFML/Graphics.hpp>
#include <vector>
#include "graph.h"



int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

	graph g;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}
	return 0;
}

/*



window.clear();
window.draw(shape);
window.display();
}*/