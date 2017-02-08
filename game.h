#pragma once
#include "nodeGraph.h"


class shooter {
public:
	float currentAngle = 0;

	void update(std::vector<sf::Keyboard::Key>& _keys, std::vector<node>& _activeTriggerBalls);
private:

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

