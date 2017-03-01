#pragma once
#include "nodeGraph.h"

enum {STATUS_ALIVE, STATUS_WON, STATUS_LOST};

class shooter;

class bullet {
public:
    bullet();

    sf::Color mColor;
    short mColorIndex;
    sf::Vector2f mMeterPos;
    sf::Vector2f mVector;

    bool mWasFired = false;
private:
    static std::vector<sf::Color> colorVec;
    void setRandomColor();
};
class shooter {
public:
    shooter();

    void setOrigin(sf::Vector2f _pos);
    void update(std::vector<sf::Keyboard::Key>& _keys, std::vector<bullet>& _activeBullets);
    sf::Sprite getArrowSprite();
    sf::Vector2f getOrigin();
    float getCurrentAngle();
    sf::Vector2f mGameDimensions;

    bullet mLoadedBullet;
    bullet mUpcomingBullet;


private:
    float mCurrentAngle = 0.0f;
    float mAimSpeed = .5f;
    float mAngleLimit = 86.0f;
    float mBulletSpeed = .18f;

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
    char mStatus;
    nodeGraph mNodeGraph;
	shooter mShooter;
    

    std::vector<bullet> mActiveBullets;
    void runPhysicsFrame(std::vector<sf::Keyboard::Key>& _keys);
    std::vector<int> getDestroyedNodeIndexes();
    void clearDestroyedNodes();
    void reset();

    //neuralNet stuff
    std::vector<short> getGraphState();
    bool getIsBulletInFlight();
    
private:
    std::vector<int> mDestroyedNodes;
    std::vector<node*> mFaceNodes;
    void updateFaceNodes();
    void updateBulletGraphInteraction();
    const sf::Vector2f mBoardMeterSize = sf::Vector2f(mBOARD_WIDTH + .5f, (float)mBOARD_HEIGHT);
    float getDistanceBetweenPoints(sf::Vector2f _a, sf::Vector2f _b);
    int translateBulletToClosestAdjacentNode(bullet& b, node& n);
    int translateBulletToClosestCeilingNode(bullet& b);
    void updateDestroyedNodes();
    bool checkForVictory();
};

