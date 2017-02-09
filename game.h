#pragma once
#include "nodeGraph.h"

class node;
class shooter;

class bullet {
public:
    bullet();

    sf::Color mColor;
    sf::Vector2f mMeterPos;
    sf::Vector2f mVector;

    bool mWasFired = false;
private:
    static std::vector<sf::Color> colorVec;
    static sf::Color getRandomColor();
};
class shooter {
public:
    shooter();

    void setOrigin(sf::Vector2f _pos);
    void update(std::vector<sf::Keyboard::Key>& _keys, std::vector<bullet>& _activeBullets);
    sf::Sprite getArrowSprite();
    sf::Vector2f getOrigin();
    sf::Vector2f mGameDimensions;

    bullet mLoadedBullet;
    bullet mUpcomingBullet;

private:
    float mCurrentAngle = 0;
    float mAimSpeed = .5;
    float mAngleLimit = 70;
    float mBulletSpeed = .16;

    sf::Vector2f mOrigin;

    sf::Sprite mArrowSprite;
    sf::Texture mArrowTexture;
    bool adjustAim(float _speed);
    void fireBullet(std::vector<bullet>& _activeBullets);
    void updateMagazine();
    void updateActiveBullets(std::vector<bullet>& _activeBullets);

    

};




class game {
public:
    game();
    const int mBOARD_WIDTH = 16;
    const int mBOARD_HEIGHT = 12;
    int mNUM_NODES;
    nodeGraph mNodeGraph;
	shooter mShooter;

    const sf::Vector2f mBoardMeterSize = sf::Vector2f(mBOARD_WIDTH + .5, mBOARD_HEIGHT);

    std::vector<bullet> mActiveBullets;

    void runPhysicsFrame(std::vector<sf::Keyboard::Key>& _keys);
};

