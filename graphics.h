#pragma once
#include <SFML\Graphics.hpp>
#include "nodeGraph.h"
#include "game.h"

class graphics {
public:
    graphics();
    graphics(game* _game, sf::RenderWindow* _rW);

    void runGraphicsLoop();

    game* mGame;


private:
    const int mMasterWindowWidth = 1000;
    const int mMasterWindowHeight = 800;

    const int mGameWindowWidth = 800;
    const int mGameWindowHeight = 600;

    const sf::Time gameInterval = sf::milliseconds(8);
    const sf::Time windowRefreshInterval = sf::milliseconds(8);

    sf::RenderWindow* mMasterWindow;
    sf::RenderTexture mStationaryNodeRenderTexture;
    sf::Vector2f mGameRenderOffsetFromMaster;

    float mRadius;

    std::vector<sf::Keyboard::Key> collectInputsFromDevices();
    void updateWindow(sf::Time& windowRefreshTimeAcc);
    void updateStationaryNodeRenderTexture();
    sf::Vector2f getStationaryCirclePixelCenter(int _pos);
    
};



