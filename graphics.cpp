#include "graphics.h"

#include <time.h>




graphics::graphics() {
}

graphics::graphics(game* _game) {
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	mGame = _game;
}


void graphics::runGraphicsLoop() {
	sf::Clock clock;
	while (true) {
		bool reset = false;
		sf::Time gameTimeAcc;
		sf::Time windowRefreshTimeAcc;

		while (true) {
			if (reset) { break; }

			sf::Time t = clock.restart();
			gameTimeAcc += t;
			windowRefreshTimeAcc += t;
			while (gameTimeAcc >= gameInterval) {
				std::vector<sf::Keyboard::Key> keys = collectInputsFromDevices();
				//physicsFrame
				gameTimeAcc -= gameInterval;
			}

			if (windowRefreshTimeAcc >= windowRefreshInterval) { updateWindow(windowRefreshTimeAcc); }
		}
	}
}


std::vector<sf::Keyboard::Key> graphics::collectInputsFromDevices() {
	std::vector<sf::Keyboard::Key> keySet;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { keySet.emplace_back(sf::Keyboard::Up); }
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { keySet.emplace_back(sf::Keyboard::Down); }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { keySet.emplace_back(sf::Keyboard::Left); }
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { keySet.emplace_back(sf::Keyboard::Right); }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) { keySet.emplace_back(sf::Keyboard::R); }

	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();
	}

	return(keySet);
}

void graphics::updateWindow (sf::Time& windowRefreshTimeAcc) {
	
	//window.draw();

	//window.draw();

	window.display();
	windowRefreshTimeAcc -= windowRefreshInterval;
}