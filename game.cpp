#include "game.h"
#include <stdio.h>

shooter::shooter() {
    mArrowTexture.loadFromFile("images/shooter_arrow.png");
    mArrowSprite.setTexture(mArrowTexture);
    updateUpcomingBallList();
}

bool shooter::adjustAim(float _speed) {
    if (((abs(mCurrentAngle)) < mAngleLimit) || (mCurrentAngle <= -mAngleLimit && _speed > 0) || (mCurrentAngle >= mAngleLimit && _speed < 0)) {
        mCurrentAngle += _speed;
        mArrowSprite.rotate(_speed);
        return(true);
    }
    return(false);
}

void shooter::fireTriggerBall(std::vector<node>& _activeTriggerBalls) {

}

void shooter::updateUpcomingBallList() {
    while (mUpcomingBallList.size() < 2) {
        mUpcomingBallList.emplace_back(node());
    }
}

void shooter::setLocation(sf::Vector2f _pos) {
    mArrowSprite.setOrigin(10, 80);
    mArrowSprite.setPosition(sf::Vector2f(_pos.x, _pos.y));
    printf("x: %.02f  | y: %.02f", _pos.x, _pos.y);
}

sf::Sprite shooter::getArrowSprite() { return(mArrowSprite); }

void shooter::update(std::vector<sf::Keyboard::Key>& _keys, std::vector<node>& _activeTriggerBalls) {
    for (sf::Keyboard::Key& k : _keys) {
        switch (k) {
        case sf::Keyboard::Right:
            adjustAim(mAimSpeed);
            break;
        case sf::Keyboard::Left:
            adjustAim(-mAimSpeed);
            break;
        case sf::Keyboard::Space:
            fireTriggerBall(_activeTriggerBalls);
            break;
        default:
            break;
        }
    }
}

game::game() {
    mNUM_NODES = mBOARD_HEIGHT * mBOARD_WIDTH;
    mNodeGraph = nodeGraph(mBOARD_WIDTH, mBOARD_HEIGHT);
}


void game::runPhysicsFrame(std::vector<sf::Keyboard::Key>& _keys) {
    //if triggerBall collided: mNodeGraph.checkForDestruction(triggerBall.mPos);
    
    mShooter.update(_keys, mActiveTriggerBalls);
}