#pragma once
#include "nodeGraph.h"


class shooter {
public:
    shooter();
	

	
    void setLocation(sf::Vector2f _pos);
	void update(std::vector<sf::Keyboard::Key>& _keys, std::vector<node>& _activeTriggerBalls);
    sf::Sprite getArrowSprite();
	
private:
	float mCurrentAngle = 0;
	float mAimSpeed = .5;
	float mAngleLimit = 70;

	sf::Sprite mArrowSprite;
	sf::Texture mArrowTexture;
	bool adjustAim(float _speed);
	void fireTriggerBall(std::vector<node>& _activeTriggerBalls);
    void updateUpcomingBallList();

    std::vector<node> mUpcomingBallList;

};




class game {
public:
    game();
    const int mBOARD_WIDTH = 16;
    const int mBOARD_HEIGHT = 12;
    int mNUM_NODES;
    nodeGraph mNodeGraph;
	shooter mShooter;

    std::vector<node> mActiveTriggerBalls;

    void runPhysicsFrame(std::vector<sf::Keyboard::Key>& _keys);
};

