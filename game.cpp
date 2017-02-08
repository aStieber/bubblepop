#include "game.h"

shooter::shooter() {
}

void shooter::update(std::vector<sf::Keyboard::Key>& _keys, std::vector<node>& _activeTriggerBalls) {

}

game::game() {
	mNUM_NODES = mBOARD_HEIGHT * mBOARD_WIDTH;
	mNodeGraph = nodeGraph(mBOARD_WIDTH, mBOARD_HEIGHT);
	
}


void game::runPhysicsFrame(std::vector<sf::Keyboard::Key>& _keys) {
	//if triggerBall collided: mNodeGraph.checkForDestruction(triggerBall.mPos);
	
	mShooter.update(_keys, mActiveTriggerBalls);
}