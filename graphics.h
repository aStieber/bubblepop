#pragma once
#include <SFML\Graphics.hpp>
#include "nodeGraph.h"


class graphics {
public:
	graphics();
	graphics(game* _game);

	void runGraphicsLoop();

	game* mGame;
	

private:
	sf::RenderWindow window;

	std::vector<sf::Keyboard::Key> collectInputsFromDevices();
	
	void graphics::updateWindow(sf::Time& windowRefreshTimeAcc);

	const sf::Time gameInterval = sf::milliseconds(8);
	const sf::Time windowRefreshInterval = sf::milliseconds(8);
};

