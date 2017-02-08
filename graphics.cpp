#include "graphics.h"
#include <time.h>
#include <stdio.h>

graphics::graphics() {}

graphics::graphics(game* _game, sf::RenderWindow* _rW) {
	mMasterWindow = _rW;
	mMasterWindow->create(sf::VideoMode(mMasterWindowWidth, mMasterWindowHeight), "SFML works!", sf::Style::Close);
	mGame = _game;
	mGameRenderTexture.create(mGameWindowWidth, mGameWindowHeight);

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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { keySet.emplace_back(sf::Keyboard::Left); }
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { keySet.emplace_back(sf::Keyboard::Right); }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) { keySet.emplace_back(sf::Keyboard::R); }

	sf::Event event;
	while (mMasterWindow->pollEvent(event)) {
		if (event.type == sf::Event::Closed) { mMasterWindow->close(); exit(0); }
	}

	return(keySet);
}

void graphics::updateWindow(sf::Time& windowRefreshTimeAcc) {
	mMasterWindow->clear(sf::Color(20, 20, 20));

	updateGameRenderTexture();
	mMasterWindow->draw(sf::Sprite(mGameRenderTexture.getTexture()));
	//window.draw();

	//window.draw();
	

	mMasterWindow->display();
	windowRefreshTimeAcc -= windowRefreshInterval;
}

void graphics::updateGameRenderTexture() {
	mGameRenderTexture.clear(sf::Color::Black);
	sf::CircleShape circle;
	circle.setRadius(mGameWindowWidth / ((mGame->mNodeGraph.mBOARD_WIDTH * 2) + 1));
	float r = circle.getRadius();
	sf::Vector2f center(r, r);
	circle.setOrigin(center);

	for (node& currentNode : mGame->mNodeGraph.mGraph) {
		if (!currentNode.mIsDisabled) {
			circle.setFillColor(currentNode.mColor);
			circle.setPosition(drawStationaryCirclePixelCenter(currentNode.mPos, circle.getRadius()));
			mGameRenderTexture.draw(circle);
		}
	}
	mGameRenderTexture.display();
}

sf::Vector2f graphics::drawStationaryCirclePixelCenter(int _pos, float _radius) {
	bool isEvenRow = ((int)(_pos / mGame->mBOARD_WIDTH) % 2 == 0);
	float horizontalOffset = (isEvenRow ? _radius : 2*_radius) + ((float)(_pos % mGame->mBOARD_WIDTH) * _radius * 2);
	float verticalOffset = _radius + (sqrt(3) * _radius * (int)(_pos / mGame->mBOARD_WIDTH)); 
	sf::Vector2f tmp = sf::Vector2f(horizontalOffset, verticalOffset);
	return(tmp);
}

