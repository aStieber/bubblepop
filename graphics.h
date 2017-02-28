#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include "nodeGraph.h"
#include "game.h"

#define DEBUG 1

class destroyedNode;

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
    sf::RenderTexture mShooterRenderTexture;
    sf::RenderTexture mBackgroundRenderTexture;
    sf::Vector2f mGameRenderOffsetFromMaster;

    std::vector<destroyedNode> mDestroyedNodes;

    float mRadius;

    std::vector<sf::Keyboard::Key> collectInputsFromDevices();
    void updateWindow(sf::Time& windowRefreshTimeAcc);
    void updateStationaryNodeRenderTexture();
    void updateShooterRenderTexture();
    void updateDestroyedNodes();
    sf::CircleShape getBall();
    sf::Vector2f getStationaryCirclePixelCenter(int _pos);
    sf::Vector2f meterToGamePixels(sf::Vector2f _meterPos);
    std::vector<destroyedNode> createDestroyedNodes(std::vector<int> _nodeIndexes);
    sf::Text graphics::DEBUG_getText(std::string text);
    sf::Text graphics::getGameOverText(std::string text);
    sf::Font mFont;
};

class destroyedNode {
public:
    sf::Color mColor;
    sf::Vector2f mMeterPos;
    int mIndex;
    short mTransparencyIncreaseRate = 4;
};


