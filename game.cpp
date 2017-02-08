#include "game.h"
#include <stdio.h>

# define M_PI           3.141592635f  /* pi */

std::vector<sf::Color> bullet::colorVec = { sf::Color::White, sf::Color::Magenta, sf::Color::Yellow,
sf::Color::Red, sf::Color::Blue, sf::Color::Green };

sf::Color bullet::getRandomColor() {
    return(colorVec[rand() % colorVec.size()]);
}
bullet::bullet() {
    mColor = getRandomColor();
}
shooter::shooter() {
    mArrowTexture.loadFromFile("images/shooter_arrow.png");
    mArrowSprite.setTexture(mArrowTexture);
    updateMagazine();
}

bool shooter::adjustAim(float _speed) {
    if (((abs(mCurrentAngle)) < mAngleLimit) || (mCurrentAngle <= -mAngleLimit && _speed > 0) || (mCurrentAngle >= mAngleLimit && _speed < 0)) {
        mCurrentAngle += _speed;
        mArrowSprite.rotate(_speed);
        return(true);
    }
    return(false);
}

void shooter::fireBullet(std::vector<bullet>& _activeBullets) {
    //create fired bullet at current vector;
    mLoadedBullet.mWasFired = true;
    mLoadedBullet.mPos = getLoadedBulletPosition();
    mLoadedBullet.mVector = sf::Vector2f(mBulletSpeed * sinf((M_PI * mCurrentAngle) / 180.0), mBulletSpeed * -cosf((M_PI * mCurrentAngle) / 180.0));
    //printf("CurrentAngle: %.02f  | x: %.02f  | y: %.02f \n", mCurrentAngle, mLoadedBullet.mVector.x, mLoadedBullet.mVector.y);
    _activeBullets.push_back(mLoadedBullet);

    updateMagazine();
}

void shooter::setLocation(sf::Vector2f _pos) {
    mArrowSprite.setOrigin(10, 80);
    mArrowSprite.setPosition(sf::Vector2f(_pos.x, _pos.y));
}

sf::Sprite shooter::getArrowSprite() { return(mArrowSprite); }

sf::Vector2f shooter::getLoadedBulletPosition() { return(mArrowSprite.getPosition()); }

void shooter::updateMagazine() {
    while (mLoadedBullet.mWasFired) {
        mLoadedBullet = mUpcomingBullet;
        mUpcomingBullet = bullet();
    }
}

void shooter::updateActiveBullets(std::vector<bullet>& _activeBullets) {
    for (bullet& b : _activeBullets) {
        b.mPos.x += b.mVector.x;
        b.mPos.y += b.mVector.y;
        if (b.mPos.x >= 1  || b.mPos.x <= 0) {
            b.mVector.x *= -1;
        }
    }
}

void shooter::update(std::vector<sf::Keyboard::Key>& _keys, std::vector<bullet>& _activeBullets) {
    updateMagazine();
    updateActiveBullets(_activeBullets );
    
    for (sf::Keyboard::Key& k : _keys) {
        switch (k) {
        case sf::Keyboard::Right:
            adjustAim(mAimSpeed);
            break;
        case sf::Keyboard::Left:
            adjustAim(-mAimSpeed);
            break;
        case sf::Keyboard::Space:
            fireBullet(_activeBullets);
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
    
    mShooter.update(_keys, mActiveBullets);
}